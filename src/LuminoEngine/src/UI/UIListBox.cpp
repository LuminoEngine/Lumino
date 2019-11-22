
#include "Internal.hpp"
#include <LuminoEngine/UI/UILayoutPanel.hpp>
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

Ref<UIListBox> UIListBox::create()
{
    return makeObject<UIListBox>();
}

UIListBox::UIListBox()
{
}

void UIListBox::init()
{
    UIItemsControl::init();

    auto layout = makeObject<UIStackLayout2>();
    layout->setOrientation(Orientation::Vertical);
    setItemsLayoutPanel(layout);
}

void UIListBox::onAddChild(UIElement* child)
{
    auto item = ln::makeObject<UIListBoxItem>();
    item->addElement(child);

    UIItemsControl::addItem(item);
}

} // namespace ln

