
#include "Internal.hpp"
#include <LuminoEngine/UI/UIItemsElement.hpp>

namespace ln {

//==============================================================================
// UIItemElement

UIItemElement::UIItemElement()
{
}

void UIItemElement::init()
{
	UIElement::init();
}

//==============================================================================
// UIItemContainerElement

UIItemContainerElement::UIItemContainerElement()
{
}

void UIItemContainerElement::init()
{
	UIScrollViewer::init();
}

//==============================================================================
// UIListBox

UIListBox::UIListBox()
{
}

void UIListBox::init()
{
	UIItemContainerElement::init();
}

} // namespace ln

