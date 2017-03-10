
/*
	書式設定されたテキストの描画
	https://msdn.microsoft.com/ja-jp/library/ms752098%28v=vs.110%29.aspx
*/

#include "../Internal.h"
#include <algorithm>
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/Rendering.h>
#include "../Device/GraphicsDriverInterface.h"
#include "../RendererImpl.h"
#include "../RenderingCommand.h"
#include "TextRenderer.h"
#include "FontManager.h"
#include "TextLayoutEngine.h"
#include "FontGlyphTextureCache.h"
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//==============================================================================
// TextRendererCore
//==============================================================================
	
static const byte_t g_TextRenderer_fx_Data[] =
{
#include "../Resource/TextRenderer.fx.h"
};
static const size_t g_TextRenderer_fx_Len = LN_ARRAY_SIZE_OF(g_TextRenderer_fx_Data);

//------------------------------------------------------------------------------
TextRendererCore::TextRendererCore()
	: m_manager(nullptr)
	, m_renderer(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
}

//------------------------------------------------------------------------------
TextRendererCore::~TextRendererCore()
{
	LN_SAFE_RELEASE(m_shader.shader);
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//------------------------------------------------------------------------------
void TextRendererCore::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	const int DefaultFaceCount = 512;

	auto* device = m_manager->GetGraphicsDevice();
	m_renderer = device->GetRenderer();
	m_vertexDeclaration.Attach(device->CreateVertexDeclaration(Vertex::Elements(), Vertex::ElementCount));
	m_vertexBuffer = device->CreateVertexBuffer(sizeof(Vertex) * DefaultFaceCount * 4, nullptr, ResourceUsage::Dynamic);
	m_indexBuffer = device->CreateIndexBuffer(DefaultFaceCount * 6, nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);

	m_vertexCache.Reserve(DefaultFaceCount * 4);
	m_indexCache.Reserve(DefaultFaceCount * 6);

	ShaderCompileResult r;
	m_shader.shader = device->CreateShader(g_TextRenderer_fx_Data, g_TextRenderer_fx_Len, &r);
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);
	m_shader.technique = m_shader.shader->GetTechnique(0);
	m_shader.pass = m_shader.technique->GetPass(0);
	m_shader.varWorldMatrix = m_shader.shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTone = m_shader.shader->GetVariableByName(_T("g_tone"));
	m_shader.varTexture = m_shader.shader->GetVariableByName(_T("g_texture"));
	//m_shader.varGlyphMaskSampler = m_shader.shader->GetVariableByName(_T("g_glyphMaskTexture"));
	m_shader.varPixelStep = m_shader.shader->GetVariableByName(_T("g_pixelStep"));
}

//------------------------------------------------------------------------------
void TextRendererCore::SetState(const Matrix& world, const Matrix& viewProj, const SizeI& viewPixelSize)
{
	m_shader.varWorldMatrix->SetMatrix(world);
	m_shader.varViewProjMatrix->SetMatrix(viewProj);
	if (m_shader.varPixelStep != nullptr)
		m_shader.varPixelStep->SetVector(Vector4(0.5f / viewPixelSize.width, 0.5f / viewPixelSize.height, 0, 0));
}

//------------------------------------------------------------------------------
void TextRendererCore::Render(const GlyphRunData* dataList, int dataCount, FontGlyphTextureCache* cache, Brush* fillBrush)
{
	Color color = Color::White;
	if (fillBrush != nullptr)
	{
		color = fillBrush->GetColor();
	}

	Driver::ITexture* srcTexture = cache->GetGlyphsFillTexture();
	Size texSizeInv(1.0f / srcTexture->GetRealSize().width, 1.0f / srcTexture->GetRealSize().height);
	for (int i = 0; i < dataCount; ++i)
	{
		// TODO: 以下、srcFillRect, srcOutlineRectを使った方が良い気がする
		const GlyphRunData& data = dataList[i];
		RectF uvSrcRect((float)data.srcRect.x, (float)data.srcRect.y, (float)data.srcRect.width, (float)data.srcRect.height);
		uvSrcRect.x *= texSizeInv.width;
		uvSrcRect.width *= texSizeInv.width;
		uvSrcRect.y *= texSizeInv.height;
		uvSrcRect.height *= texSizeInv.height;

		RectF dstRect(data.Position, (float)data.srcRect.width, (float)data.srcRect.height);
		InternalDrawRectangle(data.transform, dstRect, uvSrcRect, color);
	}

	Flush(cache);
}

//------------------------------------------------------------------------------
void TextRendererCore::InternalDrawRectangle(const Matrix& transform, const RectF& rect, const RectF& srcUVRect, const Color& color)
{
	if (rect.IsEmpty()) { return; }		// 矩形がつぶれているので書く必要はない

	float lu = srcUVRect.GetLeft();
	float tv = srcUVRect.GetTop();
	float ru = srcUVRect.GetRight();
	float bv = srcUVRect.GetBottom();

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);

	Vertex v;
	v.color = color;
	v.position.Set(rect.GetLeft(), rect.GetTop(), 0);	v.uv.Set(lu, tv);	// 左上
	v.position.TransformCoord(transform);
	m_vertexCache.Add(v);
	v.position.Set(rect.GetLeft(), rect.GetBottom(), 0); v.uv.Set(lu, bv);	// 左下
	v.position.TransformCoord(transform);
	m_vertexCache.Add(v);
	v.position.Set(rect.GetRight(), rect.GetTop(), 0);	v.uv.Set(ru, tv);	// 右上
	v.position.TransformCoord(transform);
	m_vertexCache.Add(v);
	v.position.Set(rect.GetRight(), rect.GetBottom(), 0); v.uv.Set(ru, bv);	// 右下
	v.position.TransformCoord(transform);
	m_vertexCache.Add(v);
}

//------------------------------------------------------------------------------
void TextRendererCore::Flush(FontGlyphTextureCache* cache)
{
	if (m_indexCache.GetCount() == 0) { return; }

	// ビットマップフォントからの描画なので、アルファブレンドONでなければ真っ白矩形になってしまう。
	// ・・・が、TextRendererCore のような低レベルでステートを強制変更してしまうのはいかがなものか・・・。
	RenderState oldState = m_renderer->GetRenderState();
	RenderState newState = oldState;
	newState.alphaBlendEnabled = true;
	newState.sourceBlend = BlendFactor::SourceAlpha;
	newState.destinationBlend = BlendFactor::InverseSourceAlpha;
	m_renderer->SetRenderState(newState);



	// 描画する
	m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	m_shader.varTone->SetVector((Vector4&)m_tone);
	m_shader.varTexture->SetTexture(cache->GetGlyphsFillTexture());
	//m_shader.varGlyphMaskSampler->SetTexture(m_glyphsMaskTexture);
	m_renderer->SetShaderPass(m_shader.pass);
	m_renderer->SetVertexDeclaration(m_vertexDeclaration);
	m_renderer->SetVertexBuffer(0, m_vertexBuffer);
	m_renderer->SetIndexBuffer(m_indexBuffer);
	m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);

	// キャッシュクリア
	m_vertexCache.Clear();
	m_indexCache.Clear();

	// 変更したステートを元に戻す
	m_renderer->SetRenderState(oldState);
}


//==============================================================================
// TextRenderer
//==============================================================================

//------------------------------------------------------------------------------
TextRenderer::TextRenderer()
	: m_manager(nullptr)
	, m_core(nullptr)
	, m_glyphLayoutDataList()
	, m_transform()
	, m_viewProj()
	, m_viewPixelSize()
	, m_font(nullptr)
	, m_fillBrush(nullptr)
	, m_stateModified(false)
	, m_flushRequested(false)
{
}

//------------------------------------------------------------------------------
TextRenderer::~TextRenderer()
{
}

//------------------------------------------------------------------------------
void TextRenderer::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_core = m_manager->GetTextRendererCore();
	m_font = m_manager->GetFontManager()->GetDefaultRawFont();
}

//------------------------------------------------------------------------------
void TextRenderer::SetTransform(const Matrix& matrix)
{
	m_transform = matrix;
	m_stateModified = true;
}

//------------------------------------------------------------------------------
void TextRenderer::SetViewInfo(const Matrix& viewProj, const SizeI& viewPixelSize)
{
	if (m_viewProj != viewProj)
	{
		m_viewProj = viewProj;
		m_stateModified = true;
	}
	if (m_viewPixelSize != viewPixelSize)
	{
		m_viewPixelSize = viewPixelSize;
		m_stateModified = true;
	}
}

//------------------------------------------------------------------------------
void TextRenderer::SetState(Font* font, Brush* fillBrush)
{
	RawFont* rawFont = (font != nullptr) ? font->ResolveRawFont() : nullptr;
	if (m_font != rawFont)
	{
		m_font = rawFont;
		m_stateModified = true;
	}
	if (m_fillBrush != fillBrush)
	{
		m_fillBrush = fillBrush;
		m_stateModified = true;
	}
}

//------------------------------------------------------------------------------
void TextRenderer::DrawGlyphRun(const Matrix& transform, const PointI& position, GlyphRun* glyphRun)
{
	DrawGlyphRun(transform, PointF((float)position.x, (float)position.y), glyphRun);
}
void TextRenderer::DrawGlyphRun(const Matrix& transform, const PointF& position, GlyphRun* glyphRun)
{
	if (glyphRun == nullptr) return;
	CheckUpdateState();
	m_font = glyphRun->GetFont();
	DrawGlyphsInternal(transform, position, glyphRun->RequestLayoutItems(), glyphRun->LookupFontGlyphTextureCache());
}

//------------------------------------------------------------------------------
void TextRenderer::DrawString(const Matrix& transform, const TCHAR* str, int length, const PointF& position)
{
	length = (length < 0) ? StringTraits::tcslen(str) : length;

	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetFontManager()->GetTCharToUTF32Converter()->Convert(str, sizeof(TCHAR) * length);

	FontGlyphTextureCache* cache = m_font->GetGlyphTextureCache();

	// 
	TextLayoutResult result;
	cache->GetTextLayoutEngine()->ResetSettings();
	cache->GetTextLayoutEngine()->LayoutText((UTF32*)utf32Buf.GetConstData(), utf32Buf.GetSize() / sizeof(UTF32), LayoutTextOptions::All, &result);

	DrawGlyphsInternal(transform, position, result.Items, cache);
}

//------------------------------------------------------------------------------
void TextRenderer::DrawString(const Matrix& transform, const TCHAR* str, int length, const RectF& rect, StringFormatFlags flags)
{
	length = (length < 0) ? StringTraits::tcslen(str) : length;

	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetFontManager()->GetTCharToUTF32Converter()->Convert(str, sizeof(TCHAR) * length);

	FontGlyphTextureCache* cache = m_font->GetGlyphTextureCache();

	// 
	TextLayoutEngine* layout = cache->GetTextLayoutEngine();
	cache->GetTextLayoutEngine()->ResetSettings();

	if (flags.TestFlag(StringFormatFlags::LeftAlignment)) {
		layout->SetTextAlignment(TextAlignment::Left);
	}
	else if (flags.TestFlag(StringFormatFlags::RightAlignment)) {
		layout->SetTextAlignment(TextAlignment::Right);
	}
	else if (flags.TestFlag(StringFormatFlags::CenterAlignment)) {
		layout->SetTextAlignment(TextAlignment::Center);
	}
	else {
	}

	cache->GetTextLayoutEngine()->SetDrawingArea(RectI(0, 0, (int)rect.width, (int)rect.height));


	TextLayoutResult result;
	cache->GetTextLayoutEngine()->LayoutText((UTF32*)utf32Buf.GetConstData(), utf32Buf.GetSize() / sizeof(UTF32), LayoutTextOptions::All, &result);

	DrawGlyphsInternal(transform, rect.GetTopLeft(), result.Items, cache);
}

//------------------------------------------------------------------------------
void TextRenderer::DrawGlyphsInternal(const Matrix& transform, const PointF& position, const List<TextLayoutResultItem>& layoutItems, FontGlyphTextureCache* cache)
{
	CheckUpdateState();

	int dataCount = layoutItems.GetCount();
	for (int i = 0; i < dataCount; ++i)
	{
		const TextLayoutResultItem& item = layoutItems[i];

		// 必要なグリフを探す。LookupGlyphInfo() の中で、テクスチャにグリフビットマップが Blt される。
		CacheGlyphInfo info;
		bool flush;
		cache->LookupGlyphInfo(item.Char, &info, &flush);
		if (flush)
		{
			FlushInternal(cache);
		}
		
		// TODO: Outline

		TextRendererCore::GlyphRunData layoutData;
		layoutData.transform = transform;
		layoutData.Position.x = position.x + (float)item.Location.OuterTopLeftPosition.x;
		layoutData.Position.y = position.y + (float)item.Location.OuterTopLeftPosition.y;
		layoutData.srcRect = info.srcRect;
		layoutData.outlineOffset = info.outlineOffset;
		m_glyphLayoutDataList.Add(layoutData);
	}
	
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void TextRenderer::Flush()
{
	if (m_flushRequested)
	{
		FontGlyphTextureCache* cache = m_font->GetGlyphTextureCache();
		FlushInternal(cache);
	}
}

//------------------------------------------------------------------------------
void TextRenderer::OnSetState(const DrawElementBatch* state)
{
	if (state != nullptr)
	{
		SetState(state->state.GetFont(), state->state.GetBrush());
	}
}

//------------------------------------------------------------------------------
void TextRenderer::FlushInternal(FontGlyphTextureCache* cache)
{
	int dataCount = m_glyphLayoutDataList.GetCount();
	RenderBulkData dataListData(&m_glyphLayoutDataList[0], sizeof(TextRendererCore::GlyphRunData) * dataCount);

	// Texture::Blit で転送されるものを Flush する
	cache->GetGlyphsFillTexture();

	LN_ENQUEUE_RENDER_COMMAND_5(
		TextRenderer_Flush, m_manager,
		TextRendererCore*, m_core,
		RenderBulkData, dataListData,
		int, dataCount,
		RefPtr<FontGlyphTextureCache>, cache,
		RefPtr<Brush>, m_fillBrush,
		{
			m_core->Render(
				(TextRendererCore::GlyphRunData*)dataListData.GetData(),
				dataCount,
				cache,
				m_fillBrush);
		});

	m_glyphLayoutDataList.Clear();
	m_flushRequested = false;
}

//------------------------------------------------------------------------------
void TextRenderer::CheckUpdateState()	// あらゆる Draw の直前にやりたいこと
{
	if (m_stateModified)
	{
		LN_ENQUEUE_RENDER_COMMAND_4(
			TextRenderer_Flush, m_manager,
			TextRendererCore*, m_core,
			Matrix, m_transform,
			Matrix, m_viewProj,
			SizeI, m_viewPixelSize,
			{
				m_core->SetState(m_transform, m_viewProj, m_viewPixelSize);
			});

		m_stateModified = false;
	}
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
		LN_THROW(0, NotImplementedException);
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
