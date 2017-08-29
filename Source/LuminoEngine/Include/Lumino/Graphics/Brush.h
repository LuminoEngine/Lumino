
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

namespace Driver { class ITexture; }
namespace detail {

struct BrushRawData
{
	Color						color;
	Ref<Driver::ITexture>	texture;
	Rect						srcRect;
	BrushWrapMode				wrapMode;
	BrushImageDrawMode			imageDrawMode;
	Thickness					borderThickness;
};

} // namespace detail

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
	//static Ref<Brush> create(const StringRef& filePath);




	void setColor(const Color& color) { m_color = color; }
	const Color& getColor() const { return m_color; }





	void setTexture(Texture* texture);
	Texture* getTexture() const;

	/** ピクセル単位で指定します。規定値は NaN です。これは、テクスチャ全体を転送することを示します。*/
	void setSourceRect(const Rect& rect) { m_srcRect = rect; }
	void setSourceRect(float x, float y, float width, float height) { m_srcRect.set(x, y, width, height); }
	const Rect& getSourceRect() const { return m_srcRect; }


	void setWrapMode(BrushWrapMode mode) { m_wrapMode = mode; }	// TODO: name: ImageWrapMode
	BrushWrapMode getWrapMode() const { return m_wrapMode; }

	void getImageDrawMode(BrushImageDrawMode mode) { m_imageDrawMode = mode; }
	BrushImageDrawMode getImageDrawMode() const { return m_imageDrawMode; }

	void getBorderThickness(const Thickness& thickness) { m_borderThickness = thickness; }
	void getBorderThickness(float left, float top, float right, float bottom) { m_borderThickness.set(left, top, right, bottom); }
	const Thickness& getBorderThickness() const { return m_borderThickness; }

LN_CONSTRUCT_ACCESS:
	Brush();
	Brush(const Color& color);
	virtual ~Brush();
	bool initialize();

LN_INTERNAL_ACCESS:
	bool isSolidColor() const { return m_texture.isNull(); }
	bool isTextureBrush() const { return !m_texture.isNull(); }
	void getRawData(detail::BrushRawData* outData) const;

private:
	Color				m_color;
	Ref<Texture>		m_texture;
	Rect				m_srcRect;
	BrushWrapMode		m_wrapMode;
	BrushImageDrawMode	m_imageDrawMode;
	Thickness			m_borderThickness;
};

/**
	@brief		図形を純色で塗りつぶすためのブラシです。
*/
class SolidColorBrush
	: public Brush
{
public:
	static Ref<SolidColorBrush> create(const Color& color);
	static Ref<SolidColorBrush> create(const Color& rgb, float a);

LN_CONSTRUCT_ACCESS:
	SolidColorBrush();
	virtual ~SolidColorBrush();
	bool initialize(const Color& color);
	bool initialize(const Color& rgb, float a);
};

/**
	@brief		
*/
class TextureBrush
	: public Brush
{
public:
	static Ref<TextureBrush> create(const StringRef& filePath);
	static Ref<TextureBrush> create(Texture* texture);
	static Ref<TextureBrush> create(Texture* texture, BrushImageDrawMode drawMode, const Rect& sourceRect, const Thickness& borderThickness, BrushWrapMode wrapMode);

LN_CONSTRUCT_ACCESS:
	TextureBrush();
	virtual ~TextureBrush();
	bool initialize();
	bool initialize(const StringRef& filePath);
	bool initialize(Texture* texture);

LN_INTERNAL_ACCESS:
	Rect getActualSourceRect() const;
	Size getSize() const;
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

	virtual BrushType getType() const { return BrushType_SolidColor; }

private:
};

/// Bitmap はソフト的な描画処理を行うため、BitmapBrush という名前はつかわない
class TextureBrush
	: public Brush
{
public:

	static Ref<TextureBrush> create(const StringRef& filePath);

public:
	TextureBrush();
	virtual ~TextureBrush();

public:
	void create(const TCHAR* filePath, detail::GraphicsManager* manager);
	void create(Texture* texture);
	

	virtual BrushType getType() const { return BrushType_Texture; }

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

	void setBrush(Brush* brush) { m_brush = brush; }
	Brush* getBrush() const { return m_brush; }

	void setThickness(float thickness) { m_thickness = thickness; }
	float getThickness() const { return m_thickness; }

private:
	Ref<Brush>	m_brush;
	float			m_thickness;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

