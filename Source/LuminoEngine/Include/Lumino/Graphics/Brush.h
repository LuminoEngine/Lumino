
#pragma once
#include "Common.h"
#include "../Graphics/Text/Font.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

enum class BrushWrapMode
{
	Stretch,		/**< イメージ全体を引き延ばす。*/
	Tile,			/**< イメージを並べて表示する。*/
};

enum class BrushImageDrawMode
{
	Image,
	BoxFrame,		/**< 3x3 のボーダーを描画する。*/
	BorderFrame,	/**< 3x3 のボーダーを描画する。中央は描画しない。*/
};

/**
	@brief		
*/
class Brush
	: public Object
{
public:
	static Brush*	White;
	static Brush*	Black;
	static Brush*	Gray;
	static Brush*	Red;
	static Brush*	Green;
	static Brush*	Blue;
	static Brush*	DimGray;

public:
	//static RefPtr<Brush> Create(const StringRef& filePath);




	void SetColor(const Color& color) { m_color = color; }
	const Color& GetColor() const { return m_color; }





	void SetTexture(Texture* texture);
	Texture* GetTexture() const;

	/** ピクセル単位で指定します。規定値は NaN です。これは、テクスチャ全体を転送することを示します。*/
	void SetSourceRect(const RectF& rect) { m_srcRect = rect; }
	void SetSourceRect(float x, float y, float width, float height) { m_srcRect.Set(x, y, width, height); }
	const RectF& GetSourceRect() const { return m_srcRect; }


	void SetWrapMode(BrushWrapMode mode) { m_wrapMode = mode; }	// TODO: name: ImageWrapMode
	BrushWrapMode GetWrapMode() const { return m_wrapMode; }

	void SetImageDrawMode(BrushImageDrawMode mode) { m_imageDrawMode = mode; }
	BrushImageDrawMode GetImageDrawMode() const { return m_imageDrawMode; }

	void SetBorderThickness(const ThicknessF& thickness) { m_borderThickness = thickness; }
	void SetBorderThickness(float left, float top, float right, float bottom) { m_borderThickness.Set(left, top, right, bottom); }
	const ThicknessF& GetBorderThickness() const { return m_borderThickness; }

LN_CONSTRUCT_ACCESS:
	Brush();
	Brush(const Color& color);
	virtual ~Brush();
	void Initialize();

LN_INTERNAL_ACCESS:
	bool IsSolidColor() const { return m_texture.IsNull(); }
	bool IsTextureBrush() const { return !m_texture.IsNull(); }

private:
	Color				m_color;
	RefPtr<Texture>		m_texture;
	RectF				m_srcRect;
	BrushWrapMode		m_wrapMode;
	BrushImageDrawMode	m_imageDrawMode;
	ThicknessF			m_borderThickness;
};

/**
	@brief		
*/
class TextureBrush
	: public Brush
{
public:
	static RefPtr<TextureBrush> Create(const StringRef& filePath);
	static RefPtr<TextureBrush> Create(Texture* texture);
	static RefPtr<TextureBrush> Create(Texture* texture, BrushImageDrawMode drawMode, const RectF& sourceRect, const ThicknessF& borderThickness, BrushWrapMode wrapMode);

LN_CONSTRUCT_ACCESS:
	TextureBrush();
	virtual ~TextureBrush();
	void Initialize();
	void Initialize(const StringRef& filePath);
	void Initialize(Texture* texture);
	RectF GetActualSourceRect() const;
	Size GetSize() const;
};


#if 0
/**
	@brief		
*/
class ColorBrush	// TODO: SolidColorBrush
	: public Brush
{
public:

public:
	ColorBrush(const Color32& color);
	ColorBrush(const Color& color);
	ColorBrush(float r, float g, float b, float a);
	virtual ~ColorBrush();

public:

	virtual BrushType GetType() const { return BrushType_SolidColor; }

private:
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
	void Create(const TCHAR* filePath, detail::GraphicsManager* manager);
	void Create(Texture* texture);
	

	virtual BrushType GetType() const { return BrushType_Texture; }

private:
};
#endif

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

