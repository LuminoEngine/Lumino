
#pragma once
#include "Common.h"
#include "../Base/Thickness.h"
#include "../Graphics/Text/Font.h"
#include "Texture.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

enum BrushWrapMode
{
	BrushWrapMode_Stretch = 0,	///< イメージ全体を引き延ばす
	BrushWrapMode_Tile,			///< イメージを並べて表示する

};

enum class BrushImageDrawMode
{
	Image,
	BoxFrame,
	BorderFrame,
};

class Brush
	: public Object
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
	ColorBrush(const Color32& color);
	ColorBrush(const ColorF& color);
	ColorBrush(float r, float g, float b, float a);
	virtual ~ColorBrush();

public:
	void SetColor(const ColorF& color) { m_color = color; }
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

	static RefPtr<TextureBrush> Create(const StringRef& filePath);

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

	void SetImageDrawMode(BrushImageDrawMode mode) { m_imageDrawMode = mode; }
	BrushImageDrawMode GetImageDrawMode() const { return m_imageDrawMode; }

	void SetBorderThickness(ThicknessF thickness) { m_borderThickness = thickness; }
	const ThicknessF& GetBorderThickness() const { return m_borderThickness; }
	

	virtual BrushType GetType() const { return BrushType_Texture; }

private:
	RefPtr<Texture>		m_texture;
	Rect				m_srcRect;	///< 初期値は (0, 0, INT_MAX, INT_MAX) で、全体を転送することを表す
	BrushWrapMode		m_wrapMode;
	BrushImageDrawMode	m_imageDrawMode;
	ThicknessF			m_borderThickness;
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
	int GetThickness() const { return m_frameThickness; }	// TODO: Thickness クラスにできない？

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

/**
	@brief	図形の枠線の描画方法を表します。
*/
class Pen
	: public RefObject
{
public:
	Pen()
		: m_thickness(0)
	{}

	void SetBrush(Brush* brush) { m_brush = brush; }
	Brush* GetBrush() const { return m_brush; }

	void SetThickness(float thickness) { m_thickness = thickness; }
	float GetThickness() const { return m_thickness; }

private:
	RefPtr<Brush>	m_brush;
	float			m_thickness;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

