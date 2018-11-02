
#include "Internal.hpp"
#include <LuminoEngine/UI/UIContainerElement.hpp>

namespace ln {

//==============================================================================
// UIContainerElement

UIContainerElement::UIContainerElement()
{
}

void UIContainerElement::initialize()
{
    Object::initialize();
}

void UIContainerElement::addElement(UIElement* element)
{
	if (LN_REQUIRE(element)) return;
	m_children.add(element);
}

int UIContainerElement::getVisualChildrenCount() const
{
	return m_children.size();
}

UIElement* UIContainerElement::getVisualChild(int index) const
{
	return m_children[index];
}

} // namespace ln

