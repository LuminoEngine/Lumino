
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include "../../LuminoCore/src/Text/UnicodeUtils.hpp"
#include "TextLayoutEngine.hpp"

namespace ln {
namespace detail {

//==============================================================================
// TextLayoutEngine
	
TextLayoutEngine::TextLayoutEngine()
    : m_font(nullptr)
    , m_text(nullptr)
    , m_length(0)
    , m_pos(0)
    , m_areaSize()
    , m_options(TextLayoutOptions::None)
    , m_globalMetrics()
{
}

void TextLayoutEngine::layout(FontCore* font, const Char* text, size_t length, const Size& areaSize, TextLayoutOptions options)
{
    if (LN_REQUIRE(font)) return;
    if (LN_REQUIRE(text)) return;
    if (length == 0) return;
    m_font = font;
    m_text = text;
    m_length = length;
    m_pos = 0;
    m_areaSize = areaSize;
    m_options = options;
    m_font->getGlobalMetrics(&m_globalMetrics);

    layoutTextHorizontal();
}

// "\r\n" => '\n'
UTF32 TextLayoutEngine::readChar()
{
    if (m_pos >= m_length) {
        return 0;   // EOF
    }

    auto str = (const UTF16*)m_text;
    const UTF16* begin = str + m_pos;
    const UTF16* end = str + m_length;

    if (begin[0] == '\r' && (begin < end - 1 && begin[1] == '\n')) {
        m_pos += 2;
        return '\n';
    }

    UTF32 ch = 0;
    UTFConversionOptions options;
    options.ReplacementChar = '?';
    const UTF16* ps = begin;
    UnicodeUtils::convertCharUTF16toUTF32(&ps, end, &options, &ch);
    m_pos += (ps - begin);
    return ch;
}

void TextLayoutEngine::layoutTextHorizontal()
{
    float baselineY = m_globalMetrics.ascender;

    while (layoutLineHorizontal(baselineY))
    {
        baselineY += m_globalMetrics.lineSpace;
    }
}

bool TextLayoutEngine::layoutLineHorizontal(float baselineY)
{
    UTF32 prev = 0;
    Vector2 pos;// (0, baselineY - m_globalMetrics.ascender);
    while (UTF32 ch = readChar())
    {
        if (ch == '\r' || ch == '\n') {
            return true;    // end with newline
        }

        if (prev)
        {
            Vector2 delta = m_font->getKerning(prev, ch);
            pos.x -= delta.x;
        }

        FontGlyphMetrics metrics;
        m_font->getGlyphMetrics(ch, &metrics);

        pos.y = baselineY - metrics.bearingY;
        onPlacementGlyph(ch, pos, metrics);

        pos.x += metrics.advance.x;

        prev = ch;
    }

    return false;   // end with EOF or Error
}


//==============================================================================
// MeasureTextLayoutEngine

void MeasureTextLayoutEngine::onPlacementGlyph(UTF32 ch, const Vector2& pos, const FontGlyphMetrics& metrix)
{
    areaSize.width = std::max(areaSize.width, pos.x + metrix.size.width);
    areaSize.height = std::max(areaSize.height, pos.y + metrix.size.height);
}


//==============================================================================
// BitmapTextRenderer

void BitmapTextRenderer::render(Bitmap2D* bitmap, const StringRef& text, const Rect& rect, Font* font, const Color& color, TextLayoutOptions options)
{
    m_bitmap = bitmap;
    m_rect = rect;
    m_color = color;
    m_font = FontHelper::resolveFontCore(font);
    layout(m_font, text.data(), text.length(), rect.getSize(), options);
}

void BitmapTextRenderer::onPlacementGlyph(UTF32 ch, const Vector2& pos, const FontGlyphMetrics& metrix)
{
    BitmapGlyphInfo info;
    info.glyphBitmap = nullptr; // 内部ビットマップをもらう
    m_font->lookupGlyphBitmap(ch, &info);

    m_bitmap->blit(
        RectI(m_rect.x + pos.x, m_rect.y + pos.y, info.size),
        info.glyphBitmap,
        RectI(0, 0, info.size),
        m_color.to32BitColor(), BitmapBlitOptions::AlphaBlend);
}

} // namespace detail
} // namespace ln

