
#include "Internal.hpp"
#include <LuminoEngine/UI/UIListBox.hpp>

namespace ln {

//==============================================================================
// UIListBoxItem

UIListBoxItem::UIListBoxItem()
{
}

void UIListBoxItem::init()
{
    UICollectionItem::init();
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

