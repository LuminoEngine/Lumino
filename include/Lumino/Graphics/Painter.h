
#pragma once
#include "../Graphics/Font.h"
#include "Texture.h"
#include "GlyphRun.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN


enum BrushWrapMode
{
	BrushWrapMode_Stretch = 0,	///< イメージ全体を引き延ばす
	BrushWrapMode_Tile,			///< イメージを並べて表示する

};

class Brush
	: public CoreObject
{
public:
	Brush();
	virtual ~Brush();

public:
	virtual BrushType GetType() const = 0;	// TODO: 非virtual にしたい
};


/**
	@brief		
*/
class ColorBrush	// TODO: SolidColorBrush
	: public Brush
{
public:
	static ColorBrush*	White;
	static ColorBrush*	Black;
	static ColorBrush*	Gray;
	static ColorBrush*	Red;
	static ColorBrush*	Green;
	static ColorBrush*	Blue;
	static ColorBrush*	DimGray;

public:
	ColorBrush(const Color& color);
	ColorBrush(const ColorF& color);
	ColorBrush(float r, float g, float b, float a);
	virtual ~ColorBrush();

public:
	const ColorF& GetColor() const { return m_color; }

	virtual BrushType GetType() const { return BrushType_SolidColor; }

private:
	ColorF	m_color;
};

/// Bitmap はソフト的な描画処理を行うため、BitmapBrush という名前はつかわない
class TextureBrush
	: public Brush
{
public:
	TextureBrush();
	virtual ~TextureBrush();

public:
	void Create(const TCHAR* filePath, GraphicsManager* manager);
	void Create(Texture* texture);
	void SetTexture(Texture* texture) { m_texture = texture; }
	Texture* GetTexture() { return m_texture; }

	void SetSourceRect(const Rect& rect) { m_srcRect = rect; }
	const Rect& GetSourceRect() const { return m_srcRect; }

	void SetWrapMode(BrushWrapMode mode) { m_wrapMode = mode; }
	BrushWrapMode GetWrapMode() const { return m_wrapMode; }


	virtual BrushType GetType() const { return BrushType_Texture; }

private:
	RefPtr<Texture>		m_texture;
	Rect				m_srcRect;	///< 初期値は (0, 0, INT_MAX, INT_MAX) で、全体を転送することを表す
	BrushWrapMode		m_wrapMode;
};


class FrameTextureBrush
	: public Brush
{
public:
	static FrameTextureBrushPtr Create(const TCHAR* filePath);
	static FrameTextureBrushPtr Create(Texture* texture);

public:
	void SetTexture(Texture* texture) { m_texture = texture; }
	Texture* GetTexture() { return m_texture; }

	void SetSourceRect(const Rect& rect) { m_srcRect = rect; }
	const Rect& GetSourceRect() const { return m_srcRect; }

	void SetInnerAreaSourceRect(const Rect& rect) { m_innerSrcRect = rect; }
	const Rect& GetInnerAreaSourceRect() const { return m_innerSrcRect; }


	void SetWrapMode(BrushWrapMode mode) { m_wrapMode = mode; }
	BrushWrapMode GetWrapMode() const { return m_wrapMode; }


	void SetThickness(int thickness) { m_frameThickness = thickness; }
	int GetThickness() const { return m_frameThickness; }

	virtual BrushType GetType() const { return BrushType_FrameTexture; }

public:
	FrameTextureBrush();
	virtual ~FrameTextureBrush();

private:
	RefPtr<Texture>		m_texture;
	Rect				m_srcRect;	///< 初期値は (0, 0, INT_MAX, INT_MAX) で、全体を転送することを表す
	Rect				m_innerSrcRect;
	int					m_frameThickness;
	BrushWrapMode		m_wrapMode;
};


//class TransitionBrush
//	: public Brush
//{
//};

//struct StringLayout
//{
//	TextAlignment	Alignment;	///< 文字列の水平方向の配置
//
//	static StringLayout	CenterAlignment;
//
//	StringLayout();
//
//	StringLayout(TextAlignment alignment);
//};

LN_ENUM_FLAGS(StringFormatFlags)
{
	LeftAlignment	= 0x0001,
	CenterAlignment	= 0x0002,
	RightAlignment	= 0x0004,

	Default = LeftAlignment,
};
LN_ENUM_FLAGS_DECLARE(StringFormatFlags);



namespace Internal
{

struct PainterState
{
	Matrix			Transform;
	RefPtr<Brush>	Brush;
	RefPtr<Font>	Font;
};

} // namespace Internal


/**
	@brief		
*/
class Painter
	: public RefObject
{
public:
	void Begin();
	void End();

	void ResetState();

	void SetProjection(const Size& viewSize, float nearZ = 0.0f, float farZ = 1.0f);
	void SetProjection(const SizeF& viewSize, float nearZ = 0.0f, float farZ = 1.0f);


	//void PushTransform(const Matrix& matrix);
	//void PopTransform();

	void SetTransform(const Matrix& matrix);
	void SetBrush(Brush* brush);
	void SetSolidColor(const ColorF& color);
	void SetTexture(Texture* texture, const Rect& srcRect);	///< ユーティリティ。TextureBrush をセットする
	void SetOpacity(float opacity);	// 0~1
	void SetFont(Font* font);

	//void PushState();
	//void PopState();

	void DrawRectangle(const RectF& rect);
	void DrawRectangle(float x, float y, float width, float height) { DrawRectangle(RectF(x, y, width, height)); }



	//void DrawRectangle(const RectF& rect);	// TODO: 変える
	// TODO: DrawRectangle がこの機能を兼ねる。いらない。
	void DrawFrameRectangle(const RectF& rect, float frameWidth);

	/// セットされている Brush は影響しません
	//void DrawTexture(const RectF& dstRect, Texture* texture, const Rect& srcRect);

	void DrawGlyphRun(const Point& position, GlyphRun* glyphRun);
	void DrawGlyphRun(const PointF& position, GlyphRun* glyphRun);	// SetFont 無視

	void DrawString(const String& str, const PointF& position);
	void DrawString(const TCHAR* str, int length, const PointF& position);
	void DrawString(const TCHAR* str, int length, const RectF& rect, StringFormatFlags flags);

	void Flush();

protected:
	Painter(GraphicsManager* manager);
	virtual ~Painter();

private:
	void DrawGlyphs(const PointF& position, const TextLayoutResult* result, Internal::FontGlyphTextureCache* cache);

protected:	// TODO
	GraphicsManager*				m_manager;
	PainterEngine*					m_internal;
	Internal::PainterState			m_currentState;
	//Stack<Internal::PainterState>	m_stateStack;
	//Stack<Matrix>			m_transformStack;
	//RefPtr<Brush>			m_currentBrush;
	ByteBuffer				m_tempBuffer;
	//RefPtr<Font>			m_currentFont;
};


/**
	@brief		ローカル変数として使用することを想定した Painter です。
	@details	インスタンスの生成と破棄はレンダリング中に行わなければなりません。
*/
class LocalPainter
	: public Painter
{
public:
	static const float DefaultDepthMin;		// 0.0f
	static const float DefaultDepthMax;		// 1.0f

public:
	LocalPainter(const Size& renderTargetSize, GraphicsManager* manager = NULL);
	LocalPainter(const SizeF& renderTargetSize, GraphicsManager* manager = NULL);
	LocalPainter(Texture* renderTarget, GraphicsManager* manager = NULL);
	virtual ~LocalPainter();
};

// TODO
class RenderTargetPainter
	: public Painter
{
public:
	RenderTargetPainter(Texture* renderTarget = NULL, GraphicsManager* manager = NULL);
	virtual ~RenderTargetPainter();

public:
	void SetRenderTarget(Texture* renderTarget);

private:

};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
