
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include "../Platform/TextInputMethodSystem.hpp"
#include "../../../LuminoCore/src/Text/UnicodeUtils.hpp"
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
    // TODO: シングルラインであることを前提に計測を高速化する
    Font* font = (m_font) ? m_font : defaultFont;
    return font->measureRenderSize(m_ownerLine->m_text.substr(m_range.beginIndex, m_range.length()), 1.0f);  // TODO: dpi
}

Vector2 UILogicalRun::measure(const UITextRange& range) const
{
    // TODO: シングルラインであることを前提に計測を高速化する
    Font* font = (m_font) ? m_font : m_ownerLine->m_ownerLayout->m_baseFont;
    return font->measureRenderSize(substr(range), 1.0f);  // TODO: dpi
}

StringRef UILogicalRun::substr(const UITextRange& range) const
{
    return m_ownerLine->m_text.substr(m_range.beginIndex + range.beginIndex, range.length());
}

//==============================================================================
// UILogicalLine

UILogicalLine::UILogicalLine(UITextLayout* owner, const String& text)
    : m_ownerLayout(owner)
    , m_text(text)
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

float UIPhysicalBlock::getLocalOffsetAt(int charIndex) const
{
    auto size = m_run->measure(UITextRange(m_range.beginIndex, charIndex));
    return (m_run->m_ownerLine->m_ownerLayout->isHorizontalFlow()) ? size.x : size.y;
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
        auto logicalLine = makeRef<UILogicalLine>(this, text.substr(line.beginIndex, line.endIndex));
        auto logicalRun = makeRef<UILogicalRun>(logicalLine, UITextRange(0, logicalLine->m_text.length()));
        logicalLine->m_runs.add(logicalRun);
        m_logicalLines.add(logicalLine);
    }

	m_dirtyPhysicalLines = true;
}

void UITextLayout::insertAt(const UITextLocation& loc, const StringRef& text)
{
    if (m_logicalLines.isOutOfRange(loc.lineIndex)) return;
    UILogicalLine* logicalLine = m_logicalLines[loc.lineIndex];
    
    logicalLine->m_text = logicalLine->m_text.insert(loc.offset, text);
    // TODO: dirty All
    
    bool afterLoc = false;
    for (int iRun = 0; iRun < logicalLine->m_runs.size(); iRun++) {
        auto& run = logicalLine->m_runs[iRun];
        auto& runRange = run->m_range;

        bool lastRun = (iRun == logicalLine->m_runs.size() - 1);
        if (runRange.contains(loc.offset) || (lastRun && !afterLoc))
        {
            LN_CHECK(!afterLoc);

            if (true)   // TODO: 今は text 用の run だけ
            {
                run->m_range = UITextRange(runRange.beginIndex, runRange.endIndex + text.length());
            }
            else
            {
                // TODO: image run とかにテキストは入れられないので、新しく作る
            }

            // Adjust from here to the back
            afterLoc = true;
        }
        else if (afterLoc)
        {
            UITextRange newRange = runRange;
            newRange.offset(text.length());
            run->m_range = newRange;
        }
    }

    m_dirtyPhysicalLines = true;    // TODO: Layout
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
            physicalLine->logicalRange = UITextRange(0, logicalLine->m_text.length());
            m_physicalLines.add(physicalLine);

            lineOffset += physicalLine->lineHeight;
        }

        updatePreferredCursorScreenOffsetInLine();

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

    context->setTransfrom(Matrix::Identity);
    context->drawBoxBackground(Rect(m_cursorInfo.preferredCursorScreenOffsetInLine, 0, 2, 20), CornerRadius(), BrushImageDrawMode::Image, Rect(0, 0, 1, 1), Color::Black);
}

bool UITextLayout::handleKeyDown(UIKeyEventArgs* e)
{
    if (e->getKey() == Keys::Left)
    {
        moveCursor(
            UICursorMoveMethod::Cardinal,
            testFlag(e->getModifierKeys(), ModifierKeys::Control) ? UICursorMoveGranularity::Word : UICursorMoveGranularity::Character,
            Vector2(-1, 0),
            testFlag(e->getModifierKeys(), ModifierKeys::Shift) ? UICursorAction::SelectText : UICursorAction::MoveCursor
        );
        return true;
    }
    else if (e->getKey() == Keys::Right)
    {
        moveCursor(
            UICursorMoveMethod::Cardinal,
            testFlag(e->getModifierKeys(), ModifierKeys::Control) ? UICursorMoveGranularity::Word : UICursorMoveGranularity::Character,
            Vector2(1, 0),
            testFlag(e->getModifierKeys(), ModifierKeys::Shift) ? UICursorAction::SelectText : UICursorAction::MoveCursor
        );
        return true;
    }

    return false;
}

bool UITextLayout::handleTypeChar(Char ch)
{
    if (true)   // TODO: readonly など
    {
        insertAt(m_cursorInfo.position, StringRef(&ch, 1));

        m_cursorInfo.position = translateLocationToCharDirection(m_cursorInfo.position, 1);
        updatePreferredCursorScreenOffsetInLine();

        return true;
    }
    return false;
}

void UITextLayout::updatePreferredCursorScreenOffsetInLine()
{
    auto offset = getLocalOffsetFromLogicalLocation(m_cursorInfo.position);
    m_cursorInfo.preferredCursorScreenOffsetInLine = offset.x;
}

Vector2 UITextLayout::getLocalOffsetFromLogicalLocation(const UITextLocation& loc) const
{
    int physicalLineIndex = getPhysicalLineIndexFromLogicalLocation(loc);
    if (m_logicalLines.isOutOfRange(loc.lineIndex)) return Vector2::Zero;

    auto& physicalLine = m_physicalLines[physicalLineIndex];
    for (auto& block : physicalLine->m_runBlocks) {
        auto range = block->rangeInLogicalLine();
        if (range.inclusiveContains(loc.offset)) {
            float offset = block->getLocalOffsetAt(loc.offset - range.beginIndex);
            return Vector2(offset, block->m_offset.y);
        }
    }

    return Vector2::Zero;
}

int UITextLayout::getPhysicalLineIndexFromLogicalLocation(const UITextLocation& loc) const
{
    bool performInclusiveBoundsCheck = false; // TODO:

    if (m_logicalLines.isOutOfRange(loc.lineIndex)) return -1;
    auto& logicalLine = m_logicalLines[loc.lineIndex];

    for (int i = 0; i < m_physicalLines.size(); i++) {
        auto& physicalLine = m_physicalLines[i];
        if (physicalLine->logicalIndex == loc.lineIndex) {
            if (loc.offset == 0 || logicalLine->m_text.isEmpty() || physicalLine->logicalRange.contains(loc.offset)) {
                return i;
            }

            // last line
            bool isLastLineForModel = (i == (m_physicalLines.size() - 1) || m_physicalLines[i + 1]->logicalIndex != loc.lineIndex);
            if ((isLastLineForModel || performInclusiveBoundsCheck) && physicalLine->logicalRange.endIndex == loc.offset) {
                return i;
            }
        }
    }

    return -1;
}

void UITextLayout::moveCursor(UICursorMoveMethod method, UICursorMoveGranularity granularity, const Vector2& dirOrPos, UICursorAction action)
{
    UITextLocation newCursorPosition;

    if (method == UICursorMoveMethod::Cardinal) {
        if (granularity == UICursorMoveGranularity::Character) {
            if (dirOrPos.x != 0.0f) {   // Horizontal movement
                newCursorPosition = translateLocationToCharDirection(m_cursorInfo.position, dirOrPos.x);
            }
            else {  // Vertical movement
                LN_NOTIMPLEMENTED();
            }
        }
        else {
            LN_NOTIMPLEMENTED();
        }
    }
    else {
        LN_NOTIMPLEMENTED();
    }

    m_cursorInfo.position = newCursorPosition;

    updatePreferredCursorScreenOffsetInLine();
}

// 文字方向の移動 (行方向ではなく)
UITextLocation UITextLayout::translateLocationToCharDirection(const UITextLocation& loc, int dir)
{
    auto& logicalLine = m_logicalLines[loc.lineIndex];
    int newOffsetInLine = moveToCandidate(logicalLine->m_text, loc.offset, dir);
    if (newOffsetInLine < 0) {
        // move line
        if (dir > 0) {   // to next line
            if (loc.lineIndex < m_logicalLines.size() - 1) {
                return UITextLocation(loc.lineIndex + 1, 0);
            }
        }
        else if (loc.lineIndex > 0) {
            int newLineIndex = loc.lineIndex - 1;
            return UITextLocation(newLineIndex, m_logicalLines[newLineIndex]->m_text.length());
        }

        // not move
        return loc;
    }

    assert(newOffsetInLine >= 0 && newOffsetInLine <= m_logicalLines[loc.lineIndex]->m_text.length());
    return UITextLocation(loc.lineIndex, newOffsetInLine);
}

int UITextLayout::moveToCandidate(const StringRef& text, int begin, int offset)
{
    if (begin + offset < 0) return -1;
    if (text.length() <= begin + offset) return -1;

    UTF16 ch = static_cast<UTF16>(*(text.data() + begin + offset));
    if (offset > 0) {
        // forward
        if (UnicodeUtils::checkUTF16LowSurrogate(ch)) {
            offset++;
        }
    }
    else {
        // backword
        if (UnicodeUtils::checkUTF16HighSurrogate(ch)) {
            offset--;
        }
    }
    return begin + offset;
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
    focus();    // TODO: test
}

void UITextArea::setText(const StringRef& value)
{
	m_textLayout->setText(value);
}

void UITextArea::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::KeyDownEvent) {
        if (m_textLayout->handleKeyDown(static_cast<UIKeyEventArgs*>(e))) {
            invalidateVisual();

            auto hwnd = PlatformSupport::getWin32WindowHandle(static_cast<UIFrameWindow*>(getFrameWindow())->platformWindow());
            detail::TextInputMethodSystem::SetInputScreenPos((intptr_t)hwnd, m_textLayout->m_cursorInfo.preferredCursorScreenOffsetInLine, 100);

            e->handled = true;
            return;
        }
    }
    else if (e->type() == UIEvents::TextInputEvent) {
        if (m_textLayout->handleTypeChar(static_cast<UIKeyEventArgs*>(e)->getCharCode())) {
            invalidateLayout();
            e->handled = true;
            return;
        }
    }
    

    UIElement::onRoutedEvent(e);
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

