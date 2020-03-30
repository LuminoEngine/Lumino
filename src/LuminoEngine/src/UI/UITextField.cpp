
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
	UIElement::init(nullptr);
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

Size UITextField::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	m_textArea->measureLayout(layoutContext, constraint);
	return Size::max(
		m_textArea->desiredSize(),
		UIElement::measureOverride(layoutContext, constraint));
}

Size UITextField::arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize)
{
	m_textArea->arrangeLayout(layoutContext, Rect(0, 0, finalSize));
	return UIElement::arrangeOverride(layoutContext, finalSize);
}

void UITextField::onRender(UIRenderingContext* context)
{
	UIElement::onRender(context);
}

} // namespace ln

