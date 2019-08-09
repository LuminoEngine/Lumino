
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include "UIEditableTextArea.hpp"
#include "UIManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// UITextRange

void UITextRange::splitLineRanges(const String& str, List<UITextRange>* outRanges)
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
			outRanges->add(UITextRange{ lineBeginIndex, lineEndIndex });

			if(isCRLF) {
				++ch;
			}
			lineBeginIndex = (ch - begin) + 1;
		}
	}

	if (lineBeginIndex <= str.length()) {
		outRanges->add(UITextRange{ lineBeginIndex, str.length() });
	}
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
	m_dirtyPhysicalLines = true;
}

Size UITextLayout::measure()
{
	return Size::Zero;
}

void UITextLayout::arrange(const Size& area)
{

}

void UITextLayout::render(UIRenderingContext* context)
{
	context->drawText(u"test", m_baseTextColor, m_baseFont);
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

