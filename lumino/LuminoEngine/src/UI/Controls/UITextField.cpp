
#include "Internal.hpp"
#include "../UIEditableTextArea.hpp"
#include <LuminoEngine/UI/Controls/UITextField.hpp>

namespace ln {


//==============================================================================
// UITextField

UITextField::UITextField()
{
}

void UITextField::init()
{
	UIElement::init();
	m_textArea = makeObject_deprecated<UIEditableTextArea>();
	addVisualChild(m_textArea);
}

void UITextField::setText(const StringView& value)
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

Size UITextField::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	m_textArea->arrangeLayout(layoutContext, finalArea);
	return UIElement::arrangeOverride(layoutContext, finalArea);
}

void UITextField::onRender(UIRenderingContext* context)
{
	UIElement::onRender(context);
}

} // namespace ln

