
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include "../Font/FontCore.hpp"
#include "UIEditableTextArea.hpp"
#include "UIManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// UITextRange

void UITextRange::splitLineRanges(const String& str, List<UITextRange>* outLines)
{
	int lineBeginIndex = 0;

	const Char* begin = str.c_str();
	for (const Char* ch = begin; ch && *ch; ++ch)
	{
		bool isCRLF = (*ch == '\r' && *(ch + 1) == '\n');
		if (*ch == '\r' || *ch == '\n')
		{
			int lineEndIndex = (ch - begin);
			assert(lineEndIndex >= lineBeginIndex);
            outLines->add(UITextRange{ lineBeginIndex, lineEndIndex });

			if(isCRLF) {
				++ch;
			}
			lineBeginIndex = (ch - begin) + 1;
		}
	}

	if (lineBeginIndex <= str.length()) {
        outLines->add(UITextRange{ lineBeginIndex, str.length() });
	}
}

//==============================================================================
// UILogicalRun

UILogicalRun::UILogicalRun(UILogicalLine* owner, const UITextRange& range)
    : m_ownerLine(owner)
    , m_range(range)
{
}

Vector2 UILogicalRun::measure(Font* defaultFont) const
{
    return defaultFont->measureRenderSize(m_ownerLine->m_text.substr(m_range.beginIndex, m_range.length()), 1.0f);  // TODO: dpi
}

StringRef UILogicalRun::substr(const UITextRange& range) const
{
    return m_ownerLine->m_text.substr(m_range.beginIndex + range.beginIndex, range.length());
}

//==============================================================================
// UILogicalLine

UILogicalLine::UILogicalLine(const String& text)
    : m_text(text)
{
}

//==============================================================================
// UIPhysicalBlock

UIPhysicalBlock::UIPhysicalBlock(UILogicalRun* run, const UITextRange& range, const Vector2& offset, const Vector2& size)
    : m_run(run)
    , m_range(range)
    , m_offset(offset)
    , m_size(size)
{
}

//==============================================================================
// UITextLayout

void UITextLayout::setBaseTextStyle(Font* font, const Color& textColor)
{
	assert(font);
	m_baseFont = font;
	m_baseTextColor = textColor;
	m_dirtyPhysicalLines = true;
}

void UITextLayout::setText(const StringRef& value)
{
    String text = value;

    List<UITextRange> lines;
    UITextRange::splitLineRanges(text, &lines);

    m_logicalLines.clear();
    for (auto& line : lines) {
        auto logicalLine = makeRef<UILogicalLine>(text.substr(line.beginIndex, line.endIndex));
        auto logicalRun = makeRef<UILogicalRun>(logicalLine, UITextRange(0, logicalLine->m_text.length()));
        logicalLine->m_runs.add(logicalRun);
        m_logicalLines.add(logicalLine);
    }

	m_dirtyPhysicalLines = true;
}

Size UITextLayout::measure()
{
	return Size::Zero;
}

void UITextLayout::arrange(const Size& area)
{
    float baseLineSpace;
    {
        auto core = detail::FontHelper::resolveFontCore(m_baseFont, 1.0f);  // TODO: dpi
        detail::FontGlobalMetrics metrics;
        core->getGlobalMetrics(&metrics);
        baseLineSpace = metrics.lineSpace;
    }

    if (m_dirtyPhysicalLines) {
        m_physicalLines.clear();

        // TODO: 見えるところだけ
        // TODO: 折り返し
        float lineOffset = 0.0f;
        for (int iLogicalLine = 0; iLogicalLine < m_logicalLines.size(); iLogicalLine++) {
            auto& logicalLine = m_logicalLines[iLogicalLine];
            auto physicalLine = makeRef<UIPhysicalLine>();
            physicalLine->offset = Vector2(0, lineOffset);

            Size lineSize;
            for (auto& run : logicalLine->m_runs) {
                auto size = run->measure(m_baseFont);
                auto block = makeRef<UIPhysicalBlock>(run, UITextRange(0, run->length()), physicalLine->offset, size);
                physicalLine->m_runBlocks.add(block);
                lineSize.width += size.x;
                lineSize.height = std::max(lineSize.height, size.y);
            }
            
            physicalLine->size = lineSize;
            physicalLine->lineHeight = std::max(baseLineSpace, lineSize.height);
            physicalLine->logicalIndex = iLogicalLine;
            physicalLine->range = UITextRange(0, logicalLine->m_text.length());
            m_physicalLines.add(physicalLine);

            lineOffset += physicalLine->lineHeight;
        }

        m_dirtyPhysicalLines = false;
    }
}

void UITextLayout::render(UIRenderingContext* context)
{
    for (auto& physicalLine : m_physicalLines) {
        for (auto& block : physicalLine->m_runBlocks) {
            Matrix transform = Matrix::makeTranslation(block->m_offset.x , block->m_offset.y, 0);
            context->setTransfrom(transform);
            context->drawText(block->str(), m_baseTextColor, m_baseFont);
        }
    }
}

void UITextLayout::handleKeyDown(UIKeyEventArgs* e)
{

}



} // namespace detail



//==============================================================================
// UITextArea

UITextArea::UITextArea()
{
}

void UITextArea::init()
{
	UIElement::init();
	m_textLayout = makeObject<detail::UITextLayout>();
}

void UITextArea::setText(const StringRef& value)
{
	m_textLayout->setText(value);
}

Size UITextArea::measureOverride(const Size& constraint)
{
	m_textLayout->setBaseTextStyle(finalStyle()->font, finalStyle()->textColor);
	return Size::max(m_textLayout->measure(), UIElement::measureOverride(constraint));
}

Size UITextArea::arrangeOverride(const Size& finalSize)
{
	m_textLayout->arrange(finalSize);
	return UIElement::arrangeOverride(finalSize);
}

void UITextArea::onRender(UIRenderingContext* context)
{
	m_textLayout->render(context);
	UIElement::onRender(context);
}

//==============================================================================
// UIEditableTextArea

UIEditableTextArea::UIEditableTextArea()
{
}

void UIEditableTextArea::init()
{
	UITextArea::init();
}

Size UIEditableTextArea::measureOverride(const Size& constraint)
{
	return UITextArea::measureOverride(constraint);
}

Size UIEditableTextArea::arrangeOverride(const Size& finalSize)
{
	return UITextArea::arrangeOverride(finalSize);
}

void UIEditableTextArea::onRender(UIRenderingContext* context)
{
	UITextArea::onRender(context);
}


} // namespace ln

