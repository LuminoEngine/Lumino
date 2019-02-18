#pragma once
#include "FontCore.hpp"

namespace ln {
class Font;
namespace detail {
class FontCore;

class TextRun
	: public RefObject
{
public:

};

// 簡易的な paragraph のイメージ。
class FormattedText
	: public RefObject
{
public:
	String text;
	Ref<Font> font;
	Size area;
	TextAlignment textAlignment;
};

// TODO: 入力は paragraph 単位がいいだろう。単一の装飾とテキスト範囲を表す TextRun のを受け取るイメージ。
class TextLayoutEngine
{
public:
    TextLayoutEngine();

    void layout(FontCore* font, const Char* text, size_t length, const Rect& targetArea, float strokeSize, TextAlignment alignment);

protected:
    // pos : upper left
	// デフォルトは Font クラスから取得したメトリクスを直接使って測定する。もし RenderingContext 側でストロークサイズ(Pen)などを考慮したい場合、これをオーバーライドする。
	// outSizeOffset : 元のサイズから、上下左右にどれだけ大きくなるか。通常はストロークサイズをそのまま指定する。
	//virtual void onMeasureGlyph(UTF32 ch, const Vector2& pos, const FontGlyphMetrics& metrix, Size* outSizeOffset);
    virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) = 0;

private:
	enum class LayoutMode
	{
		Measure,
		Placement,
	};
	
	struct LayoutItem
	{
		UTF32 ch;
		Vector2 pos;
		Size size;
	};

	struct LayoutLine
	{
		List<LayoutItem> glyphs;
	};

    UTF32 readChar();
	void resetStream();
    void layoutTextHorizontal(LayoutMode mode);
    bool layoutLineHorizontal(float baselineY, LayoutMode mode);
	void placementTextHorizontal();
	void placementLineHorizontal(const LayoutLine& layoutLine);
	void calculateRenderAreaHorizontalOffset(LayoutLine* layoutLine);

    FontCore* m_font;
    const Char* m_text;
    size_t m_length;
    size_t m_pos;
    Rect m_targetArea;
	Size m_renderAreaSize;
	Vector2 m_renderAreaOffset;
	float m_strokeSize;
    TextAlignment m_alignment;
    FontGlobalMetrics m_globalMetrics;
	List<LayoutLine> m_layoutLines;
};

// テキスト描画領の域計算用
class MeasureTextLayoutEngine
    : public TextLayoutEngine
{
public:
    Size areaSize;

protected:
    virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;
};

class BitmapTextRenderer
    : public TextLayoutEngine
{
public:
    void render(Bitmap2D* bitmap, const StringRef& text, const Rect& rect, Font* font, const Color& color, TextAlignment alignment);

protected:
    virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;

private:
    Bitmap2D* m_bitmap;
    Rect m_rect;
    Color m_color;
    FontCore* m_font;
};

} // namespace detail
} // namespace ln

