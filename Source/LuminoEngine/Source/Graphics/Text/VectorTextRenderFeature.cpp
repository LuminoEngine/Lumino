
/*
	書式設定されたテキストの描画
	https://msdn.microsoft.com/ja-jp/library/ms752098%28v=vs.110%29.aspx
*/

#include "../Internal.h"
#include <algorithm>
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Rendering/Rendering.h>
#include "../Device/GraphicsDriverInterface.h"
#include "../CoreGraphicsRenderFeature.h"
#include "../RenderingCommand.h"
#include "VectorTextRenderFeature.h"
#include "FontManager.h"
#include "TextLayoutEngine.h"
#include "FontGlyphTextureCache.h"
#include "../GraphicsManager.h"
#include "../../Rendering/RenderStage.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// VectorTextRendererCore
//==============================================================================
//------------------------------------------------------------------------------
VectorTextRendererCore::VectorTextRendererCore()
	: m_manager(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
}

//------------------------------------------------------------------------------
VectorTextRendererCore::~VectorTextRendererCore()
{
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//------------------------------------------------------------------------------
void VectorTextRendererCore::initialize(GraphicsManager* manager)
{
	m_manager = manager;

	auto* device = m_manager->getGraphicsDevice();
	m_renderer = device->getRenderer();

	m_vertexCache.clearAndReserve(4096);
	m_indexCache.clearAndReserve(4096);
}

//------------------------------------------------------------------------------
void VectorTextRendererCore::requestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex)
{
	assert(vb != nullptr);
	assert(ib != nullptr);
	*outBeginVertexIndex = m_vertexCache.getCount();
	*vb = m_vertexCache.request(vertexCount);
	*ib = m_indexCache.request(indexCount);
}

//------------------------------------------------------------------------------
void VectorTextRendererCore::render(const VectorGlyphData* dataList, int dataCount, VectorFontGlyphCache* cache, Brush* fillBrush)
{
	for (int i = 0; i < dataCount; i++)
	{
		Vertex* vb;
		uint16_t* ib;
		uint16_t beginVertexIndex;
		requestBuffers(
			cache->getVertexCount(dataList[i].cacheGlyphInfoHandle),
			cache->getIndexCount(dataList[i].cacheGlyphInfoHandle),
			&vb, &ib, &beginVertexIndex);
		cache->generateMesh(
			dataList[i].cacheGlyphInfoHandle, Vector3(dataList[i].origin.x, dataList[i].origin.y, 0), dataList[i].transform,
			vb, ib, beginVertexIndex);
	}

	// TODO: このへん PrimitiveRenderFeature と同じ。共通にできないか？
	{
		// サイズが足りなければ再作成
		auto* device = m_manager->getGraphicsDevice();
		if (m_vertexBuffer == nullptr || m_vertexBuffer->getByteCount() < m_vertexCache.getBufferUsedByteCount())
		{
			LN_SAFE_RELEASE(m_vertexBuffer);
			m_vertexBuffer = device->createVertexBuffer(m_vertexCache.getBufferUsedByteCount(), nullptr, ResourceUsage::Dynamic);
		}
		if (m_indexBuffer == nullptr || m_indexBuffer->getByteCount() < m_indexCache.getBufferUsedByteCount())
		{
			LN_SAFE_RELEASE(m_indexBuffer);
			m_indexBuffer = device->createIndexBuffer(m_indexCache.getBufferUsedByteCount(), nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);
		}

		// 描画する
		m_vertexBuffer->setSubData(0, m_vertexCache.getBuffer(), m_vertexCache.getBufferUsedByteCount());
		m_indexBuffer->setSubData(0, m_indexCache.getBuffer(), m_indexCache.getBufferUsedByteCount());

		{
			m_renderer->setVertexDeclaration(m_manager->getDefaultVertexDeclaration()->getDeviceObject());
			m_renderer->setVertexBuffer(0, m_vertexBuffer);
			m_renderer->setIndexBuffer(m_indexBuffer);
			m_renderer->drawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.getCount() / 3);
		}

		// キャッシュクリア
		m_vertexCache.clear();
		m_indexCache.clear();
	}
}

//==============================================================================
// VectorTextRenderer
//------------------------------------------------------------------------------
/*
	drawChar() だけでよいか？使う側でレイアウトすれば、drawString() は必要ないのでは？
	--------------------
	drawChar() だけだと、DrawingContext 側で作る描画コマンドの量が増えてしまう。
	使う側でレイアウトしたいこともあるけど、簡単に文字列を書きたいときは文字ではなく文字列をコマンドに乗せたい。
*/
//==============================================================================
//------------------------------------------------------------------------------
VectorTextRenderer::VectorTextRenderer()
{
}

//------------------------------------------------------------------------------
VectorTextRenderer::~VectorTextRenderer()
{
}

//------------------------------------------------------------------------------
void VectorTextRenderer::initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_core = Ref<VectorTextRendererCore>::makeRef();
	m_core->initialize(m_manager);
}

//------------------------------------------------------------------------------
void VectorTextRenderer::drawString(const Matrix& transform, const UTF32* str, int length, const Rect& rect, TextLayoutOptions options)
{
	TextLayoutEngine2 layout;
	layout.layout(m_currentFont, str, length, rect, options, &m_layoutResult);
	drawInternal(transform);
}

//------------------------------------------------------------------------------
void VectorTextRenderer::drawChar(const Matrix& transform, UTF32 ch, const Rect& rect, TextLayoutOptions options)
{
	TextLayoutEngine2 layout;
	layout.layout(m_currentFont, &ch, 1, rect, options, &m_layoutResult);
	drawInternal(transform);
}

//------------------------------------------------------------------------------
void VectorTextRenderer::drawInternal(const Matrix& transform)
{
	LN_NOTIMPLEMENTED();

	VectorFontGlyphCache* glyphCache = m_currentFont->GetVectorGlyphCache();

	bool needFlush = false;
	for (auto& item : m_layoutResult.items)
	{
		VectorGlyphData data;
		data.cacheGlyphInfoHandle = glyphCache->getGlyphInfo(item.ch, &needFlush);
		data.transform = transform;
		data.origin = Point(item.columnBaseline, item.lineBaseline);
		m_bufferingCache.add(data);
	}

	if (needFlush)
	{
		flush();
	}
}

//------------------------------------------------------------------------------
void VectorTextRenderer::flush()
{
	if (!m_bufferingCache.isEmpty())
	{
		int dataCount = m_bufferingCache.getCount();
		RenderBulkData dataListData(&m_bufferingCache[0], sizeof(VectorGlyphData) * dataCount);
		VectorFontGlyphCache* glyphCache = m_currentFont->GetVectorGlyphCache();

		LN_ENQUEUE_RENDER_COMMAND_5(
			VectorTextRenderer_Flush, m_manager,
			VectorTextRendererCore*, m_core,
			RenderBulkData, dataListData,
			int, dataCount,
			Ref<VectorFontGlyphCache>, glyphCache,
			Ref<Brush>, m_fillBrush,
			{
				m_core->render(
					(VectorGlyphData*)dataListData.getData(),
					dataCount,
					glyphCache,
					m_fillBrush);
			});

		m_bufferingCache.clear();
	}
}

//------------------------------------------------------------------------------
void VectorTextRenderer::onSetState(const RenderStage* state)
{
	if (state == nullptr) return;
	if (state->getFontFinal() != nullptr)
	{
		m_currentFont = state->getFontFinal()->resolveRawFont();
	}
	else
	{
		m_currentFont = nullptr;
	}
	m_fillBrush = state->getBrushFinal();
}

} // namespace detail

#if 0

//==============================================================================
// TextRenderer
//==============================================================================

//------------------------------------------------------------------------------
TextRenderer::TextRenderer(Bitmap* targetBitmap, RawFont* font)
	: m_targetBitmap(targetBitmap)
	, m_font(font)
	, m_foreColor(Graphics::Color::Black)
	, m_strokeColor(Graphics::Color::Black)
	, m_strokeSize(0)
	, m_textAlignment(TextAlignment_Left)
	, m_textTrimming(TextTrimming_None)
	, m_flowDirection(FlowDirection_LeftToRight)
	, m_areaBox()
	, m_prevGlyphData(NULL)
{
}

//------------------------------------------------------------------------------
TextRenderer::~TextRenderer()
{
}

//------------------------------------------------------------------------------
void TextRenderer::DrawText(const char* text, int length)
{
	if (length == 0) { return; }
	length = (length < 0) ? strlen(text) : length;

	// char → UTF32 に変換するのに必要なバイト数で領域確保
	size_t size = Encoding::GetConversionRequiredByteCount(Encoding::GetSystemMultiByteEncoding(), Encoding::GetUTF32Encoding(), length);
	m_utf32Buffer.Resize(size);

	// UTF32 に変換
	EncodingConversionResult result;
	Encoding::Convert(
		text, length, Encoding::GetSystemMultiByteEncoding(),
		m_utf32Buffer.GetData(), m_utf32Buffer.GetSize(), Encoding::GetUTF32Encoding(),
		&result);

	DrawTextHorizontal((UTF32*)m_utf32Buffer.GetData(), result.BytesUsed / sizeof(UTF32));
}

//------------------------------------------------------------------------------
void TextRenderer::DrawText(const wchar_t* text, int length)
{
	if (length == 0) { return; }
	length = (length < 0) ? wcslen(text) : length;

	// char → UTF32 に変換するのに必要なバイト数で領域確保
	size_t size = Encoding::GetConversionRequiredByteCount(Encoding::GetWideCharEncoding(), Encoding::GetUTF32Encoding(), length * sizeof(wchar_t));
	m_utf32Buffer.Resize(size);

	// UTF32 に変換
	EncodingConversionResult result;
	Encoding::Convert(
		text, length * sizeof(wchar_t), Encoding::GetWideCharEncoding(),
		m_utf32Buffer.GetData(), m_utf32Buffer.GetSize(), Encoding::GetUTF32Encoding(),
		&result);

	DrawTextHorizontal((UTF32*)m_utf32Buffer.GetData(), result.BytesUsed / sizeof(UTF32));
}

//------------------------------------------------------------------------------
void TextRenderer::DrawTextHorizontal(const UTF32* text, int length)
{
	RectI lineArea(0, 0, m_areaBox.Width, m_font->GetLineHeight());
	int lineBegin = 0;
	int i = 0;
	for (; i < length; ++i)
	{
		// LF
		if (text[i] == 0x0A)
		{
			DrawLineHorizontal(&text[lineBegin], i - lineBegin, lineArea);
			lineArea.Y += m_font->GetLineHeight();
			lineBegin = i;
			ResetKerning();	// 改行したのでカーニング情報をリセット
		}
		if (text[i] == 0x0D)
		{
			// CR + LF
			if (i < length - 1 && text[i + 1] == 0x0A)
			{
				DrawLineHorizontal(&text[lineBegin], i - lineBegin, lineArea);
				lineBegin = i + 1;
			}
			// CR
			else
			{
				DrawLineHorizontal(&text[lineBegin], i - lineBegin, lineArea);
				lineBegin = i;
			}
			lineArea.Y += m_font->GetLineHeight();
			ResetKerning();	// 改行したのでカーニング情報をリセット
		}
	}
	if (lineBegin != i)
	{
		DrawLineHorizontal(&text[lineBegin], i - lineBegin, lineArea);
	}
}

//------------------------------------------------------------------------------
void TextRenderer::DrawLineHorizontal(const UTF32* text, int length, const RectI& lineArea)
{
	// 描く必要がない
	if (text == NULL) { return; }
	if (length == 0) { return; }
	if (lineArea.Width <= 0 || lineArea.Height <= 0) { return; }

	RectI drawArea = lineArea;
	if (m_textAlignment == TextAlignment_Center)
	{
		// 中央揃え
		SizeI size = m_font->GetTextSize(text, length);
		drawArea.X += (drawArea.Width - size.Width) / 2;
	}
	else if (m_textAlignment == TextAlignment_Right)
	{
		// 右揃え
		SizeI size = m_font->GetTextSize(text, length);
		drawArea.X = (drawArea.X + drawArea.Width) - size.Width;
	}
	else if (m_textAlignment == TextAlignment_Justify)
	{
		// 両端揃え
		LN_NOTIMPLEMENTED();
	}

	// 描画ループ
	for (int i = 0; i < length; ++i)
	{
		FontGlyphData* glyphData = m_font->LookupGlyphData(text[i], m_prevGlyphData);

		// グリフビットマップの転送先
		RectI destRect(
			drawArea.X + glyphData->GlyphOffsetX,
			drawArea.Y + glyphData->GlyphOffsetY,
			drawArea.Width - glyphData->GlyphOffsetX,
			drawArea.Height - glyphData->GlyphOffsetY);

		// アウトライン用のビットマップがあれば先に描く
		if (glyphData->OutlineBitmap)
		{
			m_targetBitmap->BitBlt(
				destRect,
				glyphData->OutlineBitmap,
				RectI(0, 0, glyphData->OutlineBitmap->GetSize()),
				m_strokeColor,
				true);

			destRect.X += glyphData->OutlineOffset;
			destRect.Y += glyphData->OutlineOffset;
		}

		// グリフビットマップを描画
		m_targetBitmap->BitBlt(
			destRect,
			glyphData->GlyphBitmap,
			RectI(0, 0, glyphData->GlyphBitmap->GetSize()),
			m_foreColor,
			true);

		m_prevGlyphData = glyphData;
	}
}

#endif

LN_NAMESPACE_END
