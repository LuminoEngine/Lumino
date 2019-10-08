
#include "Internal.hpp"
#include "UIEditableTextArea.hpp"
#include <LuminoEngine/UI/UITextField.hpp>

namespace ln {


//==============================================================================
// UITextField

UITextField::UITextField()
{
}

void UITextField::init()
{
	UIElement::init();
	m_textArea = makeObject<UIEditableTextArea>();
	addVisualChild(m_textArea);
}

void UITextField::setText(const StringRef& value)
{
	m_textArea->setText(value);
}

const String& UITextField::text() const
{
	return m_textArea->text();
}

Size UITextField::measureOverride(const Size& constraint)
{
	m_textArea->measureLayout(constraint);
	return Size::max(
		m_textArea->desiredSize(),
		UIElement::measureOverride(constraint));
}

Size UITextField::arrangeOverride(const Size& finalSize)
{
	m_textArea->arrangeLayout(Rect(0, 0, finalSize));
	return UIElement::arrangeOverride(finalSize);
}

void UITextField::onRender(UIRenderingContext* context)
{
	UIElement::onRender(context);
}

} // namespace ln

