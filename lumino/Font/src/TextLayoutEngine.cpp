
#include "Internal.hpp"
#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoFont/Font.hpp>
#include <LuminoCore/Base/UnicodeUtils.hpp>
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
    , m_targetArea()
    , m_alignment(TextAlignment::Forward)
    , m_globalMetrics()
{
}

void TextLayoutEngine::layout(FontCore* font, const Char* text, size_t length, const Rect& targetArea, float strokeSize, TextAlignment alignment)
{
    if (LN_REQUIRE(font)) return;
    if (LN_REQUIRE(text)) return;
    if (length == 0) return;
    m_font = font;
    m_text = text;
    m_length = length;
    m_pos = 0;
    m_targetArea = targetArea;
	m_strokeSize = strokeSize;
    m_alignment = alignment;
    m_layoutLines.clear();
    m_font->getGlobalMetrics(&m_globalMetrics);
	m_renderAreaSize = Size::Zero;

	resetStream();
    layoutTextHorizontal(LayoutMode::Measure);


	placementTextHorizontal();
}

// "\r\n" => '\n'
UTF32 TextLayoutEngine::readChar()
{
    if (m_pos >= m_length) {
        return 0;   // EOF
    }

#if LN_USTRING32
    auto str = reinterpret_cast<const UTF32*>(m_text);
    const UTF32* begin = str + m_pos;
    const UTF32* end = str + m_length;

    if (begin[0] == '\r' && (begin < end - 1 && begin[1] == '\n')) {
        m_pos += 2;
        return '\n';
    }
    
    m_pos++;
    return *begin;
#else
    auto str = static_cast<const UTF16*>(m_text);
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
#endif
}

void TextLayoutEngine::resetStream()
{
	m_pos = 0;
}

//void TextLayoutEngine::onMeasureGlyph(UTF32 ch, const Vector2& pos, const FontGlyphMetrics& metrix, Size* outSizeOffset)
//{
//}

void TextLayoutEngine::layoutTextHorizontal(LayoutMode mode)
{
    float baselineY = m_globalMetrics.ascender;

    while (layoutLineHorizontal(baselineY, mode))
    {
        baselineY += m_globalMetrics.lineSpace;
    }
}

bool TextLayoutEngine::layoutLineHorizontal(float baselineY, LayoutMode mode)
{
	LayoutLine layoutLine;
    UTF32 prev = 0;
    Vector2 pos;// (0, baselineY - m_globalMetrics.ascender);
	//pos.x = m_renderAreaOffset.x;
    while (UTF32 ch = readChar())
    {
        if (ch == '\r' || ch == '\n') {
            return true;    // end with newline
        }

        if (prev)
        {
            Vector2 delta = m_font->getKerning(prev, ch);
            pos.x += delta.x;
        }

        FontGlyphMetrics metrics;
        m_font->getGlyphMetrics(ch, &metrics);

        pos.y = /*m_renderAreaOffset.y + */(baselineY - metrics.bearingY);

        pos.x += metrics.bearingX;

		//if (mode == LayoutMode::Measure) {
			m_renderAreaSize.width = std::max(m_renderAreaSize.width, (pos.x + m_strokeSize * 2) + metrics.size.width);
			m_renderAreaSize.height = std::max(m_renderAreaSize.height, (pos.y + m_strokeSize * 2) + metrics.size.height);
			layoutLine.glyphs.add({ ch, pos, metrics.size });
		//}
		//else {
		//	onPlacementGlyph(ch, pos, metrics);
		//}

        pos.x += metrics.advance.x + (m_strokeSize * 2);

        prev = ch;
    }

	//if (mode == LayoutMode::Measure) {
		m_layoutLines.add(std::move(layoutLine));
	//}

    return false;   // end with EOF or Error
}

void TextLayoutEngine::placementTextHorizontal()
{
	for (auto& layoutLine : m_layoutLines) {
		calculateRenderAreaHorizontalOffset(&layoutLine);
		placementLineHorizontal(layoutLine);
	}
}

void TextLayoutEngine::placementLineHorizontal(const LayoutLine& layoutLine)
{
	for (auto& glyph : layoutLine.glyphs) {
		onPlacementGlyph(glyph.ch, glyph.pos + m_renderAreaOffset, glyph.size);
	}
}

void TextLayoutEngine::calculateRenderAreaHorizontalOffset(LayoutLine* layoutLine)
{
	TextAlignment alignment = m_alignment;
	if (alignment == TextAlignment::Justify) {
		if (layoutLine->glyphs.size() == 0) {
			alignment = TextAlignment::Forward;
		}
		else if (layoutLine->glyphs.size() == 1) {
			alignment = TextAlignment::Center;
		}
	}

	switch (alignment)
	{
	case TextAlignment::Forward:
			m_renderAreaOffset.x = 0;
			break;
		case TextAlignment::Center:
			m_renderAreaOffset.x = (m_targetArea.width - m_renderAreaSize.width) / 2;
			break;
		case TextAlignment::Backward:
			m_renderAreaOffset.x = m_targetArea.width - m_renderAreaSize.width;
			break;
		case TextAlignment::Justify:
		{
			// "A B C" などの時の空白数
			int blank = layoutLine->glyphs.size() - 1;

			// 余りの空白量
			float remain = m_targetArea.width - m_renderAreaSize.width;

			float sw = remain / blank;

			for (int i = 1; i < layoutLine->glyphs.size() - 1; i++) {
				layoutLine->glyphs[i].pos.x += sw * i;
			}

			// 最後の一つは右詰 (加算誤差で微妙に見切れないようにする)
			layoutLine->glyphs.back().pos.x = m_targetArea.width - layoutLine->glyphs.back().size.width;

			break;
		}
	}

}

//==============================================================================
// MeasureTextLayoutEngine

void MeasureTextLayoutEngine::onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size)
{
    areaSize.width = std::max(areaSize.width, pos.x + size.width);
    areaSize.height = std::max(areaSize.height, pos.y + size.height);
}


//==============================================================================
// BitmapTextRenderer

void BitmapTextRenderer::render(Bitmap2D* bitmap, const StringRef& text, const Rect& rect, Font* font, const Color& color, TextAlignment alignment)
{
    m_bitmap = bitmap;
    m_rect = rect;
    m_color = color;
    m_font = FontHelper::resolveFontCore(font, 1.0f);
    layout(m_font, text.data(), text.length(), rect, 0, alignment);
}

void BitmapTextRenderer::onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size)
{
    BitmapGlyphInfo info;
    info.glyphBitmap = nullptr; // 内部ビットマップをもらう
    m_font->lookupGlyphBitmap(ch, &info);


    m_bitmap->blit(
        RectI(m_rect.x + pos.x, m_rect.y + pos.y, info.size),
        info.glyphBitmap,
        RectI(0, 0, info.size),
        ColorI::fromLinearColor(m_color), BitmapBlitOptions::AlphaBlend);
}

} // namespace detail
} // namespace ln

