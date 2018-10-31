#pragma once

namespace ln {
namespace detail {

// https://www.freetype.org/freetype2/docs/tutorial/step2.html
struct FontGlobalMetrics
{
    // bbox // Font face 内のすべてのグリフを囲むことができる矩形
    float ascender;       // ベースラインから上側の距離
    float descender;      // ベースラインから下側の距離
    float lineSpace;		// 1行の最大高さ。次の行までの間隔。
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


    // 文字を描画したときの最大ビットマップサイズ (ピクセル単位)
    //virtual SizeI getGlyphMaxSize() { return SizeI(getLineSpacing(), getLineSpacing()); }

    //virtual FontGlyphLocation* advanceKerning(UTF32 utf32code, int strokeSize, FontGlyphLocation* prevData) = 0;
    //virtual FontGlyphBitmap* lookupGlyphBitmap(UTF32 utf32code, int strokeSize) = 0;



    virtual void getGlobalMetrics(FontGlobalMetrics* outMetrix) = 0;
    virtual bool isOutlineSupported() const = 0;
    virtual void decomposeOutline(UTF32 utf32code, VectorGlyphInfo* outInfo) = 0;
    virtual Vector2 getKerning(UTF32 prev, UTF32 next) = 0;
    virtual void getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics) = 0;

protected:
    FontCore();

private:
};

} // namespace detail
} // namespace ln

