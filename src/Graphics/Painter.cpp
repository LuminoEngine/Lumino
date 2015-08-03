
#include "Internal.h"
#include <Lumino/Base/StringTraits.h>
#include <Lumino/Graphics/Painter.h>
#include "PainterEngine.h"
#include "RenderingCommand.h"
#include "GraphicsHelper.h"

namespace Lumino
{
namespace Graphics
{

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

static ColorBrush g_ColorBrush_White(ColorF::White);
static ColorBrush g_ColorBrush_Black(ColorF::Black);
static ColorBrush g_ColorBrush_Gray(ColorF::Gray);
static ColorBrush g_ColorBrush_Red(ColorF::Red);
static ColorBrush g_ColorBrush_Green(ColorF::Green);
static ColorBrush g_ColorBrush_Blue(ColorF::Blue);
ColorBrush*	ColorBrush::White = &g_ColorBrush_White;
ColorBrush*	ColorBrush::Black = &g_ColorBrush_Black;
ColorBrush*	ColorBrush::Gray = &g_ColorBrush_Gray;
ColorBrush*	ColorBrush::Red = &g_ColorBrush_Red;
ColorBrush*	ColorBrush::Green = &g_ColorBrush_Green;
ColorBrush*	ColorBrush::Blue = &g_ColorBrush_Blue;

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
ColorBrush::~ColorBrush()
{
}

//=============================================================================
// TextureBrush
//=============================================================================

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
	m_texture.Attach(Texture::Create(filePath, TextureFormat_R8G8B8A8, 1, manager));
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
//StringLayout::StringLayout(Imaging::TextAlignment alignment)
//{
//
//}

//=============================================================================
// Painter
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
struct SetBrushCommand : public RenderingCommand
{
	PainterEngine* m_engine;
	BrushData m_brushData;

	void Create(PainterEngine* engine, const BrushData& data)
	{
		if (data.Type == BrushType_Texture) {
			MarkGC(data.TextureBrush.Texture);
		}
		m_engine = engine;
		m_brushData = data;
	}
	void Execute() { m_engine->SetBrush(m_brushData); }
};

//=============================================================================
struct SetOpacityCommand : public RenderingCommand
{
	PainterEngine* m_engine;
	float m_opacity;

	void Create(PainterEngine* engine, float opacity)
	{
		m_engine = engine;
		m_opacity = opacity;
	}
	void Execute() { m_engine->SetOpacity(m_opacity); }
};


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
	//Device::ITexture* m_srcTexture;
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
	Device::ITexture* m_glyphsTexture;
	Device::ITexture* m_strokesTexture;
	//ColorF m_foreColor;
	//ColorF m_strokeColor;

	void Create(
		PainterEngine* engine, 
		const PointF& position,
		PainterEngine::GlyphRunData* dataList,
		int dataCount,
		Device::ITexture* glyphsTexture,
		Device::ITexture* strokesTexture/*,
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
Painter::Painter(GraphicsManager* manager)
	: m_manager(manager)
	, m_internal(Helper::GetPainterEngine(manager))
{
	LN_CALL_COMMAND(Begin, BeginCommand);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Painter::~Painter()
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
void Painter::SetProjection(const Size& viewSize, float nearZ, float farZ)
{
	Matrix mat;
	perspective2DLH(&mat, (float)viewSize.Width, (float)viewSize.Height, nearZ, farZ);
	LN_CALL_COMMAND(SetViewProjMatrix, SetProjectionCommand, mat);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::SetBrush(Brush* brush)
{
	m_currentBrush = brush;

	BrushData data;
	if (brush == NULL)
	{
		data.Type = BrushType_Unknown;	// no set
	}
	else
	{
		data.Type = brush->GetType();
		if (data.Type == BrushType_SolidColor)
		{
			auto t = static_cast<ColorBrush*>(brush);
			const ColorF& c = t->GetColor();
			data.SolidColorBrush.Color[0] = c.R;		// TODO: POD 型をまとめて定義したほうがいい気がする
			data.SolidColorBrush.Color[1] = c.G;
			data.SolidColorBrush.Color[2] = c.B;
			data.SolidColorBrush.Color[3] = c.A;
		}
		else if (data.Type == BrushType_Texture)
		{
			auto t = static_cast<TextureBrush*>(brush);
			data.TextureBrush.Texture = (t->GetTexture() != NULL) ? t->GetTexture()->GetDeviceObject() : NULL;
			const Rect& r = t->GetSourceRect();
			data.TextureBrush.SourceRect[0] = r.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
			data.TextureBrush.SourceRect[1] = r.Y;
			data.TextureBrush.SourceRect[2] = r.Width;
			data.TextureBrush.SourceRect[3] = r.Height;
			data.TextureBrush.WrapMode = t->GetWrapMode();
		}
		else {
			LN_THROW(0, NotImplementedException);
		}
	}

	LN_CALL_COMMAND(SetBrush, SetBrushCommand, data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::SetSolidColor(const ColorF& color)
{
	m_currentBrush = NULL;

	BrushData data;
	data.Type = BrushType_SolidColor;
	data.SolidColorBrush.Color[0] = color.R;
	data.SolidColorBrush.Color[1] = color.G;
	data.SolidColorBrush.Color[2] = color.B;
	data.SolidColorBrush.Color[3] = color.A;

	LN_CALL_COMMAND(SetBrush, SetBrushCommand, data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::SetTexture(Texture* texture, const Rect& r)
{
	m_currentBrush = NULL;

	BrushData data;
	data.Type = BrushType_Texture;
	data.TextureBrush.Texture = (texture != NULL) ? texture->GetDeviceObject() : NULL;
	data.TextureBrush.SourceRect[0] = r.X;		// TODO: POD 型をまとめて定義したほうがいい気がする
	data.TextureBrush.SourceRect[1] = r.Y;
	data.TextureBrush.SourceRect[2] = r.Width;
	data.TextureBrush.SourceRect[3] = r.Height;
	data.TextureBrush.WrapMode = BrushWrapMode_Stretch;

	LN_CALL_COMMAND(SetBrush, SetBrushCommand, data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::SetOpacity(float opacity)
{
	LN_CALL_COMMAND(SetOpacity, SetOpacityCommand, opacity);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawRectangle(const RectF& rect)
{
	LN_CALL_COMMAND(DrawRectangle, DrawRectangleCommand, rect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawFrameRectangle(const RectF& rect, float frameWidth)
{
	LN_CALL_COMMAND(DrawFrameRectangle, DrawFrameRectangleCommand, rect, frameWidth);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawGlyphRun(const Point& position, GlyphRun* glyphRun)
{
	DrawGlyphRun(PointF((float)position.X, (float)position.Y), glyphRun);
}
void Painter::DrawGlyphRun(const PointF& position, GlyphRun* glyphRun)
{
	if (glyphRun == NULL) { return; }
	Painter::DrawGlyphs(position, glyphRun->m_glyphData, glyphRun->m_glyphTextureCache);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawString(const String& str, const PointF& position)
{
	DrawString(str.GetCStr(), str.GetLength(), position);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawString(const TCHAR* str, int length, const PointF& position)
{
	length = (length < 0) ? StringTraits::StrLen(str) : length;

	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetFontManager()->GetTCharToUTF32Converter()->Convert(str, sizeof(TCHAR) * length);

	// 現在のフォント設定に一致するテクスチャキャッシュを探す
	RefPtr<FontGlyphTextureCache> cache(m_manager->LookupGlyphTextureCache(m_currentFont));

	// 
	Imaging::TextLayoutResult result;
	cache->GetTextLayoutEngine()->ResetSettings();
	cache->GetTextLayoutEngine()->LayoutText((UTF32*)utf32Buf.GetConstData(), utf32Buf.GetSize() / sizeof(UTF32), &result);

	Painter::DrawGlyphs(position, result, cache);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawString(const TCHAR* str, int length, const RectF& rect, StringFormatFlags flags)
{
	length = (length < 0) ? StringTraits::StrLen(str) : length;

	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetFontManager()->GetTCharToUTF32Converter()->Convert(str, sizeof(TCHAR) * length);

	// 現在のフォント設定に一致するテクスチャキャッシュを探す
	RefPtr<FontGlyphTextureCache> cache(m_manager->LookupGlyphTextureCache(m_currentFont));

	// 
	Imaging::TextLayoutEngine* layout = cache->GetTextLayoutEngine();
	cache->GetTextLayoutEngine()->ResetSettings();

	if (flags.TestFlag(StringFormatFlags::LeftAlignment)) {
		layout->SetTextAlignment(Imaging::TextAlignment::Left);
	}
	else if (flags.TestFlag(StringFormatFlags::RightAlignment)) {
		layout->SetTextAlignment(Imaging::TextAlignment::Right);
	}
	else if (flags.TestFlag(StringFormatFlags::CenterAlignment)) {
		layout->SetTextAlignment(Imaging::TextAlignment::Center);
	}
	else {
	}

	cache->GetTextLayoutEngine()->SetDrawingArea(Rect(0, 0, rect.Width, rect.Height));


	Imaging::TextLayoutResult result;
	cache->GetTextLayoutEngine()->LayoutText((UTF32*)utf32Buf.GetConstData(), utf32Buf.GetSize() / sizeof(UTF32), &result);

	Painter::DrawGlyphs(rect.GetTopLeft(), result, cache);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawGlyphs(const PointF& position, const Imaging::TextLayoutResult& result, FontGlyphTextureCache* cache)
{
	// 一時メモリ確保
	m_tempBuffer.Resize(sizeof(PainterEngine::GlyphRunData) * result.Items.GetCount());
	auto data = (PainterEngine::GlyphRunData*)m_tempBuffer.GetData();

	// 確保したメモリにテクスチャ描画情報を作っていく
	Texture* tex1 = NULL;
	Texture* tex2 = NULL;	// TODO: ストローク
	int count = result.Items.GetCount();
	for (int i = 0; i < count; ++i)
	{
		Rect srcRect;
		const Imaging::TextLayoutResultItem& item = result.Items[i];
		cache->LookupGlyph(item.Char, &tex1, &srcRect);

		data[i].Position.Set((float)item.Location.OuterTopLeftPosition.X, (float)item.Location.OuterTopLeftPosition.Y);
		data[i].SrcPixelRect.Set(srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height);
	}

	Device::ITexture* dtex2 = (tex2 != NULL) ? tex2->GetDeviceObject() : NULL;
	
	
	LN_CALL_COMMAND(DrawGlyphRun, DrawGlyphRunCommand, position, data, count, tex1->GetDeviceObject(), dtex2/*, ColorF::Black, ColorF::Blue*/);	// TODO: 色
}

} // namespace Graphics
} // namespace Lumino
