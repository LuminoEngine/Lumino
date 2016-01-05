
#include "Internal.h"
#include <Lumino/Base/StringTraits.h>
#include <Lumino/Graphics/Painter.h>
#include "RendererImpl.h"
#include "PainterEngine.h"
#include "RenderingCommand.h"
#include "GraphicsHelper.h"
#include "TextLayoutEngine.h"
#include "FontGlyphTextureCache.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// Brush
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Brush::Brush()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Brush::~Brush()
{
}

//=============================================================================
// ColorBrush
//=============================================================================

// 以前は ColorF の static 変数を参照していたが、それだと初期化の順によってはこちらの値がすべて 0,0,0,0 になってしまうことがあった
static ColorBrush g_ColorBrush_White(1.0, 1.0, 1.0, 1.0);
static ColorBrush g_ColorBrush_Black(0.0, 0.0, 0.0, 1.0);
static ColorBrush g_ColorBrush_Gray(0.5, 0.5, 0.5, 1.0);
static ColorBrush g_ColorBrush_Red(1.0, 0.0, 0.0, 1.0);
static ColorBrush g_ColorBrush_Green(0.0, 1.0, 0.0, 1.0);
static ColorBrush g_ColorBrush_Blue(0.0, 0.0, 1.0, 1.0);
static ColorBrush g_ColorBrush_DimGray(0.25, 0.25, 0.25, 1.0);
ColorBrush*	ColorBrush::White = &g_ColorBrush_White;
ColorBrush*	ColorBrush::Black = &g_ColorBrush_Black;
ColorBrush*	ColorBrush::Gray = &g_ColorBrush_Gray;
ColorBrush*	ColorBrush::Red = &g_ColorBrush_Red;
ColorBrush*	ColorBrush::Green = &g_ColorBrush_Green;
ColorBrush*	ColorBrush::Blue = &g_ColorBrush_Blue;
ColorBrush*	ColorBrush::DimGray = &g_ColorBrush_DimGray;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColorBrush::ColorBrush(const Color& color)
	: m_color(color)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColorBrush::ColorBrush(const ColorF& color)
	: m_color(color)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColorBrush::ColorBrush(float r, float g, float b, float a)
	: m_color(r, g, b, a)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColorBrush::~ColorBrush()
{
}

//=============================================================================
// TextureBrush
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<TextureBrush> TextureBrush::Create(const StringRef& filePath)
{
	RefPtr<TextureBrush> obj(LN_NEW TextureBrush());
	obj->Create(filePath.GetBegin(), nullptr);	// TODO: getBegin
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextureBrush::TextureBrush()
	: m_srcRect(0, 0, INT_MAX, INT_MAX)
	, m_wrapMode(BrushWrapMode_Stretch)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextureBrush::~TextureBrush()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextureBrush::Create(const TCHAR* filePath, GraphicsManager* manager)
{
	m_texture.Attach(Texture2D::Create(filePath, TextureFormat_R8G8B8A8, 1));		//TODO: GraphicsManager?
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextureBrush::Create(Texture* texture)
{
	m_texture = texture;
}


//=============================================================================
// FrameTextureBrush
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FrameTextureBrushPtr FrameTextureBrush::Create(const TCHAR* filePath)
{
	RefPtr<Texture> tex(Texture2D::Create(filePath), false);
	FrameTextureBrushPtr ptr(LN_NEW FrameTextureBrush(), false);
	ptr->SetTexture(tex);
	return ptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FrameTextureBrushPtr FrameTextureBrush::Create(Texture* texture)
{
	FrameTextureBrushPtr ptr(LN_NEW FrameTextureBrush(), false);
	ptr->SetTexture(texture);
	return ptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FrameTextureBrush::FrameTextureBrush()
	: m_texture()
	, m_srcRect(0, 0, INT_MAX, INT_MAX)
	, m_frameThickness(8)
	, m_wrapMode(BrushWrapMode_Stretch)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FrameTextureBrush::~FrameTextureBrush()
{
}


//=============================================================================
// StringLayout
//=============================================================================
//static StringLayout	CenterAlignment;
//
//StringLayout::StringLayout()
//	: Alignment()
//{
//
//}
//
//StringLayout::StringLayout(TextAlignment alignment)
//{
//
//}

//=============================================================================
// RenderingContext
//=============================================================================

//=============================================================================
struct BeginCommand : public RenderingCommand
{
	PainterEngine* m_engine;
	void Create(PainterEngine* engine) { m_engine = engine; }
	void Execute() { m_engine->Begin(); }
};

//=============================================================================
struct EndCommand : public RenderingCommand
{
	PainterEngine* m_engine;
	void Create(PainterEngine* engine) { m_engine = engine; }
	void Execute() { m_engine->End(); }
};

//=============================================================================
struct SetTransformCommand : public RenderingCommand
{
	PainterEngine* m_engine;
	Matrix m_matrix;

	void Create(PainterEngine* engine, const Matrix& matrix)
	{
		m_engine = engine;
		m_matrix = matrix;
	}
	void Execute() { m_engine->SetTransform(m_matrix); }
};

//=============================================================================
struct SetProjectionCommand : public RenderingCommand
{
	PainterEngine* m_engine;
	Matrix m_matrix;

	void Create(PainterEngine* engine, const Matrix& matrix)
	{
		m_engine = engine;
		m_matrix = matrix;
	}
	void Execute() { m_engine->SetViewProjMatrix(m_matrix); }
};

//=============================================================================
struct SetPainterEngineStateCommand : public RenderingCommand
{
	PainterEngine* m_engine;
	PainterEngineState m_state;

	void Create(PainterEngine* engine, const Details::PainterState& state)
	{
		m_engine = engine;
		m_state.Create(state);

		if (m_state.Brush.Type == BrushType_Texture) {
			MarkGC(m_state.Brush.TextureBrush.Texture);
		}
		if (m_state.Brush.Type == BrushType_FrameTexture) {
			MarkGC(m_state.Brush.FrameTextureBrush.Texture);
		}
	}
	void Execute() { m_engine->SetState(m_state); }
};

//=============================================================================
//struct SetOpacityCommand : public RenderingCommand
//{
//	PainterEngine* m_engine;
//	float m_opacity;
//
//	void Create(PainterEngine* engine, float opacity)
//	{
//		m_engine = engine;
//		m_opacity = opacity;
//	}
//	void Execute() { m_engine->SetOpacity(m_opacity); }
//};
//

//=============================================================================
struct DrawRectangleCommand : public RenderingCommand
{
	PainterEngine* m_engine;
	RectF m_rect;

	void Create(PainterEngine* engine, const RectF& rect)
	{
		m_engine = engine;
		m_rect = rect;
	}
	void Execute() { m_engine->DrawRectangle(m_rect); }
};

//=============================================================================
//struct DrawFillRectangleCommand : public RenderingCommand
//{
//	PainterEngine* m_engine;
//	RectF m_rect;
//	//Device::ITexture* m_srcTexture;
//	//Rect m_srcRect;
//	//BrushWrapMode m_wrapMode;
//
//	void Create(PainterEngine* engine, const RectF& rect/*, Device::ITexture* srcTexture, const Rect& srcRect, BrushWrapMode wrapMode*/)
//	{
//		//MarkGC(srcTexture);
//		m_engine = engine;
//		m_rect = rect;
//		//m_srcTexture = srcTexture;
//		//m_srcRect = srcRect;
//		//m_wrapMode = wrapMode;
//	}
//	void Execute() { m_engine->DrawFillRectangle(m_rect, m_srcTexture, m_srcRect, m_wrapMode); }
//};

//=============================================================================
struct DrawFrameRectangleCommand : public RenderingCommand
{
	PainterEngine* m_engine;
	RectF m_rect;
	float m_frameWidth;
	//Driver::ITexture* m_srcTexture;
	//Rect m_srcRect;

	void Create(PainterEngine* engine, const RectF& rect, float frameWidth/*, Device::ITexture* srcTexture, const Rect& srcRect*/)
	{
		//MarkGC(srcTexture);
		m_engine = engine;
		m_rect = rect;
		m_frameWidth = frameWidth;
		//m_srcTexture = srcTexture;
		//m_srcRect = srcRect;
	}
	void Execute() { m_engine->DrawFrameRectangle(m_rect, m_frameWidth/*, m_srcTexture, m_srcRect*/); }
};

//=============================================================================
struct DrawGlyphRunCommand : public RenderingCommand
{
	PainterEngine* m_engine;
	PointF m_position;
	DataHandle m_dataList;
	int m_dataCount;
	Driver::ITexture* m_glyphsTexture;
	Driver::ITexture* m_strokesTexture;
	//ColorF m_foreColor;
	//ColorF m_strokeColor;

	void Create(
		PainterEngine* engine, 
		const PointF& position,
		PainterEngine::GlyphRunData* dataList,
		int dataCount,
		Driver::ITexture* glyphsTexture,
		Driver::ITexture* strokesTexture/*,
		const ColorF& foreColor, 
		const ColorF& strokeColor*/)
	{
		DataHandle dataHandle = AllocExtData(sizeof(PainterEngine::GlyphRunData) * dataCount, dataList);
		MarkGC(glyphsTexture);
		if (strokesTexture != NULL) { MarkGC(strokesTexture); }
		m_engine = engine;
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
		m_engine->DrawGlyphRun(m_position, (PainterEngine::GlyphRunData*)GetExtData(m_dataList), m_dataCount, m_glyphsTexture, m_strokesTexture/*, m_foreColor, m_strokeColor*/);
	}
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingContext::RenderingContext(GraphicsManager* manager)
	: m_manager(manager)
	, m_internal(manager->GetPainterEngine())
	, m_stateModified(true)
	, m_internalSolidColorBrush(ColorF::White)
	, m_internalTextureBrush()
{
	//LN_CALL_COMMAND(Begin, BeginCommand);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingContext::~RenderingContext()
{
	//LN_CALL_COMMAND(End, EndCommand);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::ResetState()
{
	m_currentState.Transform = Matrix::Identity;
	m_currentState.Brush = NULL;
	m_currentState.Font = NULL;
	m_currentState.Opacity = 1.0f;
	m_currentState.Tone = ToneF();
	LN_CALL_COMMAND(Begin, BeginCommand);	// TODO: Reset
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::Flush()
{
	LN_CALL_COMMAND(End, EndCommand);
}

/// ピクセル単位の2D描画に使う射影行列の作成
static void perspective2DLH(Matrix* out_, float width_, float height_, float near_, float far_)
{
	// ※このオフセット調整が必要なのは DX9 のみ。本来であればシェーダ内でたいおうするものなので、
	// 後々この処理は削除する。
#if 1
	out_->Set(
		2.0f / width_, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height_, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (far_ - near_), 0.0f,
		-1.0f, 1.0f, near_ / (near_ - far_), 1.0f);
#else
	// 平行移動成分は、画面半分 + 0.5 ピクセルの調整
	out_->set(
		2.0f / width_, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height_, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (far_ - near_), 0.0f,
		-1.0f - (0.5f / width_), 1.0f + (0.5f / height_), near_ / (near_ - far_), 1.0f);
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::SetProjection(const Size& viewSize, float nearZ, float farZ)
{
	SetProjection(SizeF((float)viewSize.Width, (float)viewSize.Height), nearZ, farZ);
}
void RenderingContext::SetProjection(const SizeF& viewSize, float nearZ, float farZ)
{
	Matrix mat;
	perspective2DLH(&mat, viewSize.Width,  viewSize.Height, nearZ, farZ);
	LN_CALL_COMMAND(SetViewProjMatrix, SetProjectionCommand, mat);
}



////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void RenderingContext::PushTransform(const Matrix& matrix)
//{
//	m_transformStack.Push(matrix);
//	LN_CALL_COMMAND(SetTransform, SetTransformCommand, matrix);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void RenderingContext::PopTransform()
//{
//	LN_CALL_COMMAND(SetTransform, SetTransformCommand, m_transformStack.GetTop());
//	m_transformStack.Pop();
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::SetTransform(const Matrix& matrix)
{
	m_currentState.Transform = matrix;
	LN_CALL_COMMAND(SetTransform, SetTransformCommand, matrix);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::SetBrush(Brush* brush)
{
	m_currentState.Brush = brush;
	m_stateModified = true;

	//BrushData data;
	//if (brush == NULL)
	//{
	//	data.Type = BrushType_Unknown;	// no set
	//}
	//else
	//{
	//	data.Type = brush->GetType();
	//	if (data.Type == BrushType_SolidColor)
	//	{
	//		auto t = static_cast<ColorBrush*>(brush);
	//		const ColorF& c = t->GetColor();
	//		data.SolidColorBrush.Color[0] = c.R;		// TODO: POD 型をまとめて定義したほうがいい気がする
	//		data.SolidColorBrush.Color[1] = c.G;
	//		data.SolidColorBrush.Color[2] = c.B;
	//		data.SolidColorBrush.Color[3] = c.A;
	//	}
	//	else if (data.Type == BrushType_Texture)
	//	{
	//		auto t = static_cast<TextureBrush*>(brush);
	//		data.TextureBrush.Texture = (t->GetTexture() != NULL) ? Helper::GetDeviceObject(t->GetTexture()) : NULL;
	//		const Rect& r = t->GetSourceRect();
	//		data.TextureBrush.SourceRect[0] = r.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
	//		data.TextureBrush.SourceRect[1] = r.Y;
	//		data.TextureBrush.SourceRect[2] = r.Width;
	//		data.TextureBrush.SourceRect[3] = r.Height;
	//		data.TextureBrush.WrapMode = t->GetWrapMode();
	//	}
	//	else if (data.Type == BrushType_FrameTexture)
	//	{
	//		auto t = static_cast<FrameTextureBrush*>(brush);
	//		data.FrameTextureBrush.Texture = (t->GetTexture() != NULL) ? Helper::GetDeviceObject(t->GetTexture()) : NULL;
	//		const Rect& r = t->GetSourceRect();
	//		const Rect& r2 = t->GetInnerAreaSourceRect();
	//		data.FrameTextureBrush.SourceRect[0] = r.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
	//		data.FrameTextureBrush.SourceRect[1] = r.Y;
	//		data.FrameTextureBrush.SourceRect[2] = r.Width;
	//		data.FrameTextureBrush.SourceRect[3] = r.Height;
	//		data.FrameTextureBrush.InnerSourceRect[0] = r2.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
	//		data.FrameTextureBrush.InnerSourceRect[1] = r2.Y;
	//		data.FrameTextureBrush.InnerSourceRect[2] = r2.Width;
	//		data.FrameTextureBrush.InnerSourceRect[3] = r2.Height;
	//		data.FrameTextureBrush.WrapMode = t->GetWrapMode();
	//		data.FrameTextureBrush.FrameThicness = t->GetThickness();
	//	}
	//	else {
	//		LN_THROW(0, NotImplementedException);
	//	}
	//}

	//LN_CALL_COMMAND(SetBrush, SetBrushCommand, data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::SetSolidColor(const ColorF& color)
{
	m_internalSolidColorBrush.SetColor(color);
	m_currentState.Brush = &m_internalSolidColorBrush;
	m_stateModified = true;

	//m_currentState.Brush = NULL;

	//BrushData data;
	//data.Type = BrushType_SolidColor;
	//data.SolidColorBrush.Color[0] = color.R;
	//data.SolidColorBrush.Color[1] = color.G;
	//data.SolidColorBrush.Color[2] = color.B;
	//data.SolidColorBrush.Color[3] = color.A;

	//LN_CALL_COMMAND(SetBrush, SetBrushCommand, data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::SetTexture(Texture* texture, const Rect& r)
{
	m_internalTextureBrush.SetTexture(texture);
	m_internalTextureBrush.SetSourceRect(r);
	m_currentState.Brush = &m_internalTextureBrush;
	m_stateModified = true;

	//m_currentState.Brush = NULL;

	//BrushData data;
	//data.Type = BrushType_Texture;
	//data.TextureBrush.Texture = (texture != NULL) ? Helper::GetDeviceObject(texture) : NULL;
	//data.TextureBrush.SourceRect[0] = r.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
	//data.TextureBrush.SourceRect[1] = r.Y;
	//data.TextureBrush.SourceRect[2] = r.Width;
	//data.TextureBrush.SourceRect[3] = r.Height;
	//data.TextureBrush.WrapMode = BrushWrapMode_Stretch;

	//LN_CALL_COMMAND(SetBrush, SetBrushCommand, data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::SetOpacity(float opacity)
{
	//LN_CALL_COMMAND(SetOpacity, SetOpacityCommand, opacity);
	m_currentState.Opacity = opacity;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::SetTone(const ToneF& tone)
{
	m_currentState.Tone = tone;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::SetFont(Font* font)
{
	m_currentState.Font = font;
	m_stateModified = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawRectangle(const RectF& rect)
{
	CheckUpdateState();
	LN_CALL_COMMAND(DrawRectangle, DrawRectangleCommand, rect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawFrameRectangle(const RectF& rect, float frameWidth)
{
	CheckUpdateState();
	LN_CALL_COMMAND(DrawFrameRectangle, DrawFrameRectangleCommand, rect, frameWidth);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawGlyphRun(const Point& position, GlyphRun* glyphRun)
{
	DrawGlyphRun(PointF((float)position.X, (float)position.Y), glyphRun);
}
void RenderingContext::DrawGlyphRun(const PointF& position, GlyphRun* glyphRun)
{
	if (glyphRun == NULL) { return; }
	CheckUpdateState();
	RenderingContext::DrawGlyphs(position, Helper::GetGlyphData(glyphRun), Helper::GetGlyphTextureCache(glyphRun));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawString(const String& str, const PointF& position)
{
	DrawString(str.c_str(), str.GetLength(), position);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawString(const TCHAR* str, int length, const PointF& position)
{
	length = (length < 0) ? StringTraits::StrLen(str) : length;

	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetFontManager()->GetTCharToUTF32Converter()->Convert(str, sizeof(TCHAR) * length);

	// 現在のフォント設定に一致するテクスチャキャッシュを探す
	RefPtr<Internal::FontGlyphTextureCache> cache(m_manager->LookupGlyphTextureCache(m_currentState.Font), false);

	// 
	TextLayoutResult result;
	cache->GetTextLayoutEngine()->ResetSettings();
	cache->GetTextLayoutEngine()->LayoutText((UTF32*)utf32Buf.GetConstData(), utf32Buf.GetSize() / sizeof(UTF32), &result);

	RenderingContext::DrawGlyphs(position, &result, cache);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawString(const TCHAR* str, int length, const RectF& rect, StringFormatFlags flags)
{
	length = (length < 0) ? StringTraits::StrLen(str) : length;

	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetFontManager()->GetTCharToUTF32Converter()->Convert(str, sizeof(TCHAR) * length);

	// 現在のフォント設定に一致するテクスチャキャッシュを探す
	RefPtr<Internal::FontGlyphTextureCache> cache(m_manager->LookupGlyphTextureCache(m_currentState.Font), false);

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

	cache->GetTextLayoutEngine()->SetDrawingArea(Rect(0, 0, (int)rect.Width, (int)rect.Height));


	TextLayoutResult result;
	cache->GetTextLayoutEngine()->LayoutText((UTF32*)utf32Buf.GetConstData(), utf32Buf.GetSize() / sizeof(UTF32), &result);

	RenderingContext::DrawGlyphs(rect.GetTopLeft(), &result, cache);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawGlyphs(const PointF& position, const TextLayoutResult* result, Internal::FontGlyphTextureCache* cache)
{
	CheckUpdateState();

	/*	Font 系は非スレッドセーフ。
		グリフとその配置はメインスレッドで作ってから PainterEngine に送る。
	*/

	// 一時メモリ確保
	m_tempBuffer.Resize(sizeof(PainterEngine::GlyphRunData) * result->Items.GetCount());
	auto data = (PainterEngine::GlyphRunData*)m_tempBuffer.GetData();

	// 確保したメモリにテクスチャ描画情報を作っていく
	Texture* tex1 = NULL;
	Texture* tex2 = NULL;	// TODO: ストローク
	int count = result->Items.GetCount();
	for (int i = 0; i < count; ++i)
	{
		Rect srcRect;
		const TextLayoutResultItem& item = result->Items[i];
		cache->LookupGlyph(item.Char, &tex1, &srcRect);

		data[i].Position.Set((float)item.Location.OuterTopLeftPosition.X, (float)item.Location.OuterTopLeftPosition.Y);
		data[i].SrcPixelRect.Set((float)srcRect.X, (float)srcRect.Y, (float)srcRect.Width, (float)srcRect.Height);
	}

	Driver::ITexture* dtex2 = (tex2 != NULL) ? tex2->GetDeviceObject() : NULL;
	
	
	LN_CALL_COMMAND(DrawGlyphRun, DrawGlyphRunCommand, position, data, count, tex1->GetDeviceObject(), dtex2/*, ColorF::Black, ColorF::Blue*/);	// TODO: 色
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::CheckUpdateState()
{
	if (m_stateModified)
	{
		if (m_manager->GetRenderingType() == RenderingType::Deferred) {
			m_manager->GetPrimaryRenderingCommandList()->AddCommand<SetPainterEngineStateCommand>(m_internal, m_currentState);
		}
		else
		{
			PainterEngineState state;
			state.Create(m_currentState);
			m_internal->SetState(state);
		}
		m_stateModified = false;
	}
}


//=============================================================================
// LocalPainter
//=============================================================================

const float LocalPainter::DefaultDepthMin = 0.0f;
const float LocalPainter::DefaultDepthMax = 1.0f;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LocalPainter::LocalPainter(const Size& renderTargetSize, GraphicsManager* manager)
	: LocalPainter(SizeF((float)renderTargetSize.Width, (float)renderTargetSize.Height), manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LocalPainter::LocalPainter(const SizeF& renderTargetSize, GraphicsManager* manager)
	: RenderingContext((manager != NULL) ? manager : GraphicsManager::Instance)
{
	LN_CALL_COMMAND(Begin, BeginCommand);
	SetProjection(renderTargetSize, DefaultDepthMin, DefaultDepthMax);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LocalPainter::~LocalPainter()
{
	LN_CALL_COMMAND(End, EndCommand);
}

//=============================================================================
// RenderTargetPainter
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//RenderTargetPainter::RenderTargetPainter(Texture* renderTarget)
//	: RenderTargetPainter(renderTarget, )
//{
//
//}

RenderTargetPainter::RenderTargetPainter(Texture* renderTarget, GraphicsManager* manager)
	: RenderingContext((manager != NULL) ? manager : GraphicsManager::Instance)
{
	if (renderTarget == NULL) {
		renderTarget = m_manager->GetRenderer()->GetRenderTarget(0);
	}
	if (renderTarget != NULL) {
		SetProjection(renderTarget->GetSize());
	}
}

RenderTargetPainter::~RenderTargetPainter()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderTargetPainter::SetRenderTarget(Texture* renderTarget)
{
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
