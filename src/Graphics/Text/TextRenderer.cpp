
/*
書式設定されたテキストの描画
https://msdn.microsoft.com/ja-jp/library/ms752098%28v=vs.110%29.aspx





*/

#include "../Internal.h"
#include <algorithm>
#include <Lumino/Graphics/GraphicsException.h>
#include "../Device/GraphicsDriverInterface.h"
#include "../RendererImpl.h"
#include "../RenderingCommand.h"
#include "TextRenderer.h"
#include "../GraphicsHelper.h"
#include "FontManager.h"
#include "TextLayoutEngine.h"
#include "FontGlyphTextureCache.h"

#define LN_CALL_CORE_COMMAND(func, command, ...) \
	if (m_manager->GetRenderingType() == RenderingType::Deferred) { \
		m_manager->GetPrimaryRenderingCommandList()->AddCommand<command>(m_core, __VA_ARGS__); \
	} \
	else { \
		m_core->func(__VA_ARGS__); \
	}

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//=============================================================================
struct TextRendererCore_SetStateCommand : public RenderingCommand
{
	TextRendererCore*	m_core;
	Matrix				m_transform;
	Matrix				m_viewProj;
	Size				m_viewPixelSize;
	void Create(TextRendererCore* core, const Matrix& world, const Matrix& viewProj, const Size& viewPixelSize)
	{
		m_core = core;
		m_transform = world;
		m_viewProj = viewProj;
		m_viewPixelSize = viewPixelSize;
	}
	void Execute()
	{
		m_core->SetState(m_transform, m_viewProj, m_viewPixelSize);
	}
};

#if 0
//=============================================================================
struct TextRendererCore_DrawGlyphRunCommand : public RenderingCommand
{
	TextRendererCore* m_core;
	PointF m_position;
	DataHandle m_dataList;
	int m_dataCount;
	Driver::ITexture* m_glyphsTexture;
	Driver::ITexture* m_strokesTexture;
	//ColorF m_foreColor;
	//ColorF m_strokeColor;

	void Create(
		TextRendererCore* core,
		const PointF& position,
		TextRendererCore::GlyphRunData* dataList,
		int dataCount,
		Driver::ITexture* glyphsTexture,
		Driver::ITexture* strokesTexture/*,
		const ColorF& foreColor, 
		const ColorF& strokeColor*/)
	{
		DataHandle dataHandle = AllocExtData(sizeof(TextRendererCore::GlyphRunData) * dataCount, dataList);
		MarkGC(glyphsTexture);
		if (strokesTexture != NULL) { MarkGC(strokesTexture); }
		m_core = core;
		m_position = position;
		m_dataList = dataHandle;
		m_dataCount = dataCount;
		m_glyphsTexture = glyphsTexture;
		m_strokesTexture = strokesTexture;
		//m_foreColor = foreColor;
		//m_strokeColor = strokeColor;
	}

	void Execute()
	{
		m_core->DrawGlyphRun(m_position, (TextRendererCore::GlyphRunData*)GetExtData(m_dataList), m_dataCount, m_glyphsTexture, m_strokesTexture/*, m_foreColor, m_strokeColor*/);
	}
};

//=============================================================================
struct TextRendererCore_FlushCommand : public RenderingCommand
{
	TextRendererCore* m_core;
	void Create(TextRendererCore* core) { m_core = core; }
	void Execute() { m_core->Flush(); }
};
#endif

//=============================================================================
// TextRendererCore
//=============================================================================
	
static const byte_t g_TextRenderer_fx_Data[] =
{
#include "../Resource/TextRenderer.fx.h"
};
static const size_t g_TextRenderer_fx_Len = LN_ARRAY_SIZE_OF(g_TextRenderer_fx_Data);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRendererCore::TextRendererCore()
	: m_manager(nullptr)
	, m_renderer(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
	//, m_foreTexture(nullptr)
	//, m_glyphsMaskTexture(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRendererCore::~TextRendererCore()
{
	LN_SAFE_RELEASE(m_shader.shader);
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRendererCore::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	const int DefaultFaceCount = 512;

	auto* device = m_manager->GetGraphicsDevice();
	m_renderer = device->GetRenderer();
	m_vertexBuffer = device->CreateVertexBuffer(Vertex::Elements(), Vertex::ElementCount, DefaultFaceCount * 4, nullptr, DeviceResourceUsage_Dynamic);
	m_indexBuffer = device->CreateIndexBuffer(DefaultFaceCount * 6, nullptr, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic);

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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRendererCore::SetState(const Matrix& world, const Matrix& viewProj, const Size& viewPixelSize)
{
	m_shader.varWorldMatrix->SetMatrix(world);
	m_shader.varViewProjMatrix->SetMatrix(viewProj);
	m_shader.varPixelStep->SetVector(Vector4(0.5f / viewPixelSize.width, 0.5f / viewPixelSize.height, 0, 0));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRendererCore::DrawGlyphRun(const PointF& position, const GlyphRunData* dataList, int dataCount, Internal::FontGlyphTextureCache* cache, RenderingCommandList* cmdList /*Driver::ITexture* glyphsTexture, Driver::ITexture* strokesTexture*//*, const ColorF& foreColor, const ColorF& strokeColor*/)
{
	//m_glyphsMaskTexture = glyphsTexture;

	SizeF texSizeInv(1.0f / cache->GetGlyphsTextureSize().width, 1.0f / cache->GetGlyphsTextureSize().height);
	for (int i = 0; i < dataCount; ++i)
	{
		Internal::CacheGlyphInfo info;
		if (dataList[i].fillBitmapData != 0)
		{
			m_tmpBitmap.Deserialize(cmdList->GetExtData(dataList[i].fillBitmapData));
			info.fillGlyphBitmap = &m_tmpBitmap;
		}
		else
		{
			info.fillGlyphBitmap = nullptr;
		}
		info.srcRect = dataList[i].srcRect;
		info.outlineOffset = dataList[i].outlineOffset;

		Rect srcFillRect, srcOutlineRect;
		cache->CommitCacheGlyphInfo(&info, &srcFillRect, &srcOutlineRect);

		// TODO: 以下、srcFillRect, srcOutlineRectを使った方が良い気がする

		RectF uvSrcRect((float)dataList[i].srcRect.x, (float)dataList[i].srcRect.y, (float)dataList[i].srcRect.width, (float)dataList[i].srcRect.height);
		uvSrcRect.x *= texSizeInv.width;
		uvSrcRect.width *= texSizeInv.width;
		uvSrcRect.y *= texSizeInv.height;
		uvSrcRect.height *= texSizeInv.height;

		RectF dstRect(dataList[i].Position, (float)dataList[i].srcRect.width, (float)dataList[i].srcRect.height);
		dstRect.x += position.x;
		dstRect.y += position.y;
		InternalDrawRectangle(dstRect, uvSrcRect);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRendererCore::Flush(Internal::FontGlyphTextureCache* cache)
{
	if (m_indexCache.GetCount() == 0) { return; }

	//Driver::ITexture* srcTexture = m_foreTexture;
	//if (srcTexture == nullptr) {
	//	srcTexture = m_manager->GetDummyTexture();
	//}

	// 描画する
	m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	m_shader.varTone->SetVector((Vector4&)m_tone);
	m_shader.varTexture->SetTexture(cache->GetGlyphsFillTexture());
	//m_shader.varGlyphMaskSampler->SetTexture(m_glyphsMaskTexture);
	m_shader.pass->Apply();
	m_renderer->DrawPrimitiveIndexed(m_vertexBuffer, m_indexBuffer, PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);

	// キャッシュクリア
	m_vertexCache.Clear();
	m_indexCache.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRendererCore::InternalDrawRectangle(const RectF& rect, const RectF& srcUVRect)
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
	v.color = ColorF::White;	// TODO
	v.position.Set(rect.GetLeft(), rect.GetTop(), 0);	v.uv.Set(lu, tv);	// 左上
	m_vertexCache.Add(v);
	v.position.Set(rect.GetLeft(), rect.GetBottom(), 0); v.uv.Set(lu, bv);	// 左下
	m_vertexCache.Add(v);
	v.position.Set(rect.GetRight(), rect.GetTop(), 0);	v.uv.Set(ru, tv);	// 右上
	m_vertexCache.Add(v);
	v.position.Set(rect.GetRight(), rect.GetBottom(), 0); v.uv.Set(ru, bv);	// 右下
	m_vertexCache.Add(v);
}

//=============================================================================
// TextRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRenderer::TextRenderer()
	: m_manager(nullptr)
	, m_core(nullptr)
	, m_font(nullptr)
	, m_stateModified(false)
	, m_flushRequested(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRenderer::~TextRenderer()
{
	LN_SAFE_RELEASE(m_font);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_core = m_manager->GetTextRendererCore();
	SetFont(m_manager->GetFontManager()->GetDefaultFont());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::SetTransform(const Matrix& matrix)
{
	m_transform = matrix;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::SetViewProjMatrix(const Matrix& matrix)
{
	m_viewProj = matrix;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::SetViewPixelSize(const Size& size)
{
	m_viewPixelSize = size;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::SetFont(Font* font)
{
	LN_REFOBJ_SET(m_font, font);
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawGlyphRun(const Point& position, GlyphRun* glyphRun)
{
	DrawGlyphRun(PointF((float)position.x, (float)position.y), glyphRun);
}
void TextRenderer::DrawGlyphRun(const PointF& position, GlyphRun* glyphRun)
{
	if (glyphRun == NULL) { return; }
	CheckUpdateState();
	DrawGlyphsInternal(position, glyphRun->RequestLayoutItems(), glyphRun->LookupFontGlyphTextureCache());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawString(const String& str, const PointF& position)
{
	DrawString(str.c_str(), str.GetLength(), position);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawString(const TCHAR* str, int length, const PointF& position)
{
	length = (length < 0) ? StringTraits::StrLen(str) : length;

	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetFontManager()->GetTCharToUTF32Converter()->Convert(str, sizeof(TCHAR) * length);

	// 現在のフォント設定に一致するテクスチャキャッシュを探す
	RefPtr<Internal::FontGlyphTextureCache> cache(m_manager->LookupGlyphTextureCache(m_font), false);

	// 
	TextLayoutResult result;
	cache->GetTextLayoutEngine()->ResetSettings();
	cache->GetTextLayoutEngine()->LayoutText((UTF32*)utf32Buf.GetConstData(), utf32Buf.GetSize() / sizeof(UTF32), LayoutTextOptions::All, &result);

	DrawGlyphsInternal(position, result.Items, cache);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawString(const TCHAR* str, int length, const RectF& rect, StringFormatFlags flags)
{
	length = (length < 0) ? StringTraits::StrLen(str) : length;

	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetFontManager()->GetTCharToUTF32Converter()->Convert(str, sizeof(TCHAR) * length);

	// 現在のフォント設定に一致するテクスチャキャッシュを探す
	RefPtr<Internal::FontGlyphTextureCache> cache(m_manager->LookupGlyphTextureCache(m_font), false);

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

	cache->GetTextLayoutEngine()->SetDrawingArea(Rect(0, 0, (int)rect.width, (int)rect.height));


	TextLayoutResult result;
	cache->GetTextLayoutEngine()->LayoutText((UTF32*)utf32Buf.GetConstData(), utf32Buf.GetSize() / sizeof(UTF32), LayoutTextOptions::All, &result);

	DrawGlyphsInternal(rect.GetTopLeft(), result.Items, cache);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawGlyphsInternal(const PointF& position, const Array<TextLayoutResultItem>& layoutItems, Internal::FontGlyphTextureCache* cache)
{
	// TODO: ↓いまは Flush でやるようなことをしている。後で変更したい。

	CheckUpdateState();

	// 一時メモリ確保
	m_tempBuffer.Resize(sizeof(TextRendererCore::GlyphRunData) * layoutItems.GetCount());
	auto dataList = (TextRendererCore::GlyphRunData*)m_tempBuffer.GetData();

	RenderingCommandList* cmdList = m_manager->GetPrimaryRenderingCommandList();

	int dataCount = layoutItems.GetCount();
	for (int i = 0; i < dataCount; ++i)
	{
		const TextLayoutResultItem& item = layoutItems[i];

		Internal::CacheGlyphInfo info;
		bool flush;

		// Fill
		cache->LookupGlyphInfo(item.Char, &info, &flush);
		if (flush)
		{
			LN_NOTIMPLEMENTED();
		}
		if (info.fillGlyphBitmap != nullptr)
		{
			// ビットマップデータを一時メモリへ保存する
			dataList[i].fillBitmapData = cmdList->AllocExtData(info.fillGlyphBitmap->GetSerializeSize(), nullptr);
			info.fillGlyphBitmap->Serialize(cmdList->GetExtData(dataList[i].fillBitmapData));
		}
		else
		{
			dataList[i].fillBitmapData = 0;
		}
		
		// TODO: Outline

		dataList[i].Position.Set((float)item.Location.OuterTopLeftPosition.x, (float)item.Location.OuterTopLeftPosition.y);
		dataList[i].srcRect = info.srcRect;
		dataList[i].outlineOffset = info.outlineOffset;
	}

	//RenderingCommandList::DataHandle dataHandle = cmdList->AllocExtData(sizeof(TextRendererCore::GlyphRunData) * dataCount, dataList);

	RenderBulkData dataListData(dataList, sizeof(TextRendererCore::GlyphRunData) * dataCount);

	LN_ENQUEUE_RENDER_COMMAND_6(
		DrawGlyphsInternal, m_manager,
		TextRendererCore*, m_core,
		PointF, position,
		RenderBulkData, dataListData,
		int, dataCount,
		RefPtr<Internal::FontGlyphTextureCache>, cache,
		RenderingCommandList*, cmdList,
		{
			m_core->DrawGlyphRun(
				position,
				(TextRendererCore::GlyphRunData*)dataListData.GetData(),
				dataCount,
				cache,
				cmdList);
			m_core->Flush(cache);
		});





#if 0

	/*	Font 系は非スレッドセーフ。
	グリフとその配置はメインスレッドで作ってから TextRendererCore に送る。
	*/

	// 一時メモリ確保
	m_tempBuffer.Resize(sizeof(TextRendererCore::GlyphRunData) * layoutItems.GetCount());
	auto data = (TextRendererCore::GlyphRunData*)m_tempBuffer.GetData();

	// 確保したメモリにテクスチャ描画情報を作っていく
	Texture* tex1 = NULL;
	Texture* tex2 = NULL;	// TODO: ストローク
	int count = layoutItems.GetCount();
	for (int i = 0; i < count; ++i)
	{
		Rect srcRect;
		const TextLayoutResultItem& item = layoutItems[i];
		cache->LookupGlyph(item.Char, 0, &tex1, &srcRect);	// TODO: ストローク

		data[i].Position.Set((float)item.Location.OuterTopLeftPosition.X, (float)item.Location.OuterTopLeftPosition.Y);
		data[i].SrcPixelRect.Set((float)srcRect.X, (float)srcRect.Y, (float)srcRect.Width, (float)srcRect.Height);
	}

	//ScopedTextureLock lock(tex1);
	//lock.GetBitmap()->Save(L"test.png");

	Driver::ITexture* dtex2 = (tex2 != NULL) ? tex2->GetDeviceObject() : NULL;

	LN_CALL_CORE_COMMAND(DrawGlyphRun, TextRendererCore_DrawGlyphRunCommand, position, data, count, tex1->GetDeviceObject(), dtex2/*, ColorF::Black, ColorF::Blue*/);	// TODO: 色
	m_flushRequested = true;
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::Flush()
{
#if 0
	if (m_flushRequested)
	{
		LN_CALL_CORE_COMMAND(Flush, TextRendererCore_FlushCommand);
		m_flushRequested = false;
	}
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::CheckUpdateState()	// あらゆる Draw の直前にやりたいこと
{
	m_core->ActivateFront(this);

	if (m_stateModified)
	{
		Flush();
		LN_CALL_CORE_COMMAND(SetState, TextRendererCore_SetStateCommand, m_transform, m_viewProj, m_viewPixelSize);
		m_stateModified = false;
	}
}

} // namespace detail

#if 0

//=============================================================================
// TextRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRenderer::TextRenderer(Bitmap* targetBitmap, Font* font)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRenderer::~TextRenderer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawText(const char* text, int length)
{
	LN_VERIFY_RETURN(text != NULL);
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawText(const wchar_t* text, int length)
{
	LN_VERIFY_RETURN(text != NULL);
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawTextHorizontal(const UTF32* text, int length)
{
	Rect lineArea(0, 0, m_areaBox.Width, m_font->GetLineHeight());
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawLineHorizontal(const UTF32* text, int length, const Rect& lineArea)
{
	// 描く必要がない
	if (text == NULL) { return; }
	if (length == 0) { return; }
	if (lineArea.Width <= 0 || lineArea.Height <= 0) { return; }

	Rect drawArea = lineArea;
	if (m_textAlignment == TextAlignment_Center)
	{
		// 中央揃え
		Size size = m_font->GetTextSize(text, length);
		drawArea.X += (drawArea.Width - size.Width) / 2;
	}
	else if (m_textAlignment == TextAlignment_Right)
	{
		// 右揃え
		Size size = m_font->GetTextSize(text, length);
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
		Rect destRect(
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
				Rect(0, 0, glyphData->OutlineBitmap->GetSize()),
				m_strokeColor,
				true);

			destRect.X += glyphData->OutlineOffset;
			destRect.Y += glyphData->OutlineOffset;
		}

		// グリフビットマップを描画
		m_targetBitmap->BitBlt(
			destRect,
			glyphData->GlyphBitmap,
			Rect(0, 0, glyphData->GlyphBitmap->GetSize()),
			m_foreColor,
			true);

		m_prevGlyphData = glyphData;
	}
}

#endif

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
