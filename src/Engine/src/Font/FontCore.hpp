#pragma once

namespace ln {
class Bitmap2D;
namespace detail {
class FontManager;
class FontGlyphTextureCache;
class FontGlyphTextureCacheRequest;

// https://www.freetype.org/freetype2/docs/tutorial/step2.html
struct FontGlobalMetrics
{
    float ascender;       // ベースラインから上側の距離
    float descender;      // ベースラインから下側の距離
    float lineSpace;		// 1行の最大高さ。次の行までの間隔。
	//float boundingMinX;	// Font face 内のすべてのグリフを囲むことができる矩形 (ピクセル単位。ただし、整数であるとは限らない。もしビットマップサイズが欲しい場合は bitmapMaxWidth, Height)
	//float boundingMaxX;
	//float boundingMinY;
	//float boundingMaxY;
	//int bitmapMaxWidth;
	//int bitmapMaxHeight;
	bool outlineSupported;

	// descender-ascender のサイズを 1.0 に収めるためのスケール
	float virutalSpaceFactor;
};

struct FontGlyphMetrics
{
    Size size;
    float bearingX;
    float bearingY;
    Vector2 advance;		// 次の文字までのベースライン間隔 (この分だけ進めたあと、Kerning の値をさらに加算する ← TODO: 要調査)
};

// POD
struct FontOutlineVertex
{
    Vector2	pos;
    float	alpha;
    Vector2	extrusion;	// 押し出し方向
    Vector2	extrusion2;	// 押し出し方向

    FontOutlineVertex(const Vector2& pos_)
        : pos(pos_)
        , alpha(1.0f)
    {}
};

// POD
struct OutlineInfo
{
    int	startIndex;
    int	vertexCount;
};

struct BitmapGlyphInfo
{
	Bitmap2D* glyphBitmap = nullptr;	// in,out
	//RawBitmap*			GlyphBitmap;
	//RawBitmap*			OutlineBitmap;	///< 縁どり線用 (NULL でなければ先にこれをbltし、その上に GlyphBitmap を blt する)
	//int				OutlineOffset;	///< 縁どり時は、線の分だけサイズが大きくなる。
	SizeI size;	// [out] glyphBitmap に書き込まれた bitmap のピクセルサイズ。左上詰めで blit される。

	bool loadColor = false;	// in
};

struct VectorGlyphInfo
{
    List<FontOutlineVertex>	vertices;
    List<OutlineInfo>		outlines;
    List<uint16_t>			triangleIndices;	// 要素数は3の倍数となる
};


class FontCore
	: public RefObject
{
public:
	FontManager* manager() const { return m_manager; }


    // 文字を描画したときの最大ビットマップサイズ (ピクセル単位)
    //virtual SizeI getGlyphMaxSize() { return SizeI(getLineSpacing(), getLineSpacing()); }

    //virtual FontGlyphLocation* advanceKerning(UTF32 utf32code, int strokeSize, FontGlyphLocation* prevData) = 0;
    //virtual FontGlyphBitmap* lookupGlyphBitmap(UTF32 utf32code, int strokeSize) = 0;



	virtual void dispose();
    virtual void getGlobalMetrics(FontGlobalMetrics* outMetrix) = 0;
	virtual void getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics) = 0;
    virtual Vector2 getKerning(UTF32 prev, UTF32 next) = 0;

	// outInfo->bitmap が nullptr である場合、内部に持っている Bitmap2D の参照を返す。
	// この場合この Bitmap2D の参照を持ち出したり変更したりすることはできない。
	virtual void lookupGlyphBitmap(UTF32 utf32code, BitmapGlyphInfo* outInfo) = 0;
	virtual void decomposeOutline(UTF32 utf32code, VectorGlyphInfo* outInfo) = 0;

	//FontGlyphTextureCache* getFontGlyphTextureCache();
	void beginCacheUsing();
	void endCacheUsing();
	void getFontGlyphTextureCache(FontGlyphTextureCacheRequest* inout);

protected:
    FontCore();
	void init(FontManager* manager);
	virtual void finalize();

private:
	FontManager* m_manager;
	std::array<Ref<FontGlyphTextureCache>, 4> m_fontGlyphTextureCacheList;
	int m_activeCacheIndex;
	//Ref<FontGlyphTextureCache> m_fontGlyphTextureCache;
};

} // namespace detail
} // namespace ln

