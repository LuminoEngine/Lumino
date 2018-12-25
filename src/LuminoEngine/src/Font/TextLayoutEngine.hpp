#pragma once
#include "FontCore.hpp"

namespace ln {
class Font;
namespace detail {
class FontCore;

class TextLayoutEngine
	: public RefObject
{
public:
    TextLayoutEngine();

    void layout(FontCore* font, const Char* text, size_t length, const Size& areaSize, TextAlignment alignment);

protected:
    // pos : upper left
    virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const FontGlyphMetrics& metrix) = 0;

private:
    UTF32 readChar();
    void layoutTextHorizontal();
    bool layoutLineHorizontal(float baselineY);

    FontCore* m_font;
    const Char* m_text;
    size_t m_length;
    size_t m_pos;
    Size m_areaSize;
    TextAlignment m_alignment;
    FontGlobalMetrics m_globalMetrics;
};

// テキスト描画領の域計算用
class MeasureTextLayoutEngine
    : public TextLayoutEngine
{
public:
    Size areaSize;

protected:
    virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const FontGlyphMetrics& metrix) override;
};

class BitmapTextRenderer
    : public TextLayoutEngine
{
public:
    void render(Bitmap2D* bitmap, const StringRef& text, const Rect& rect, Font* font, const Color& color, TextAlignment alignment);

protected:
    virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const FontGlyphMetrics& metrix) override;

private:
    Bitmap2D* m_bitmap;
    Rect m_rect;
    Color m_color;
    FontCore* m_font;
};

} // namespace detail
} // namespace ln

