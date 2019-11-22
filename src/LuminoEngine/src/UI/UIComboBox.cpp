
#include "Internal.hpp"
#include <LuminoEngine/UI/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIPopup.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>

namespace ln {

//==============================================================================
// UIComboBoxItem

UIComboBoxItem::UIComboBoxItem()
{
}

void UIComboBoxItem::init()
{
    UICollectionItem::init();
}

//==============================================================================
// UIComboBox

UIComboBox::UIComboBox()
{
}

void UIComboBox::init()
{
    UIItemsControl::init();
    m_popup = ln::makeObject<ln::UIPopup>();
    m_popup->setWidth(200);
    m_popup->setHeight(300);
    m_popup->setBackgroundColor(Color::AliceBlue);
    m_popup->setPlacementTarget(this);
    addVisualChild(m_popup);
}

void UIComboBox::onAddChild(UIElement* child)
{
    auto item = ln::makeObject<UIComboBoxItem>();
    item->addElement(child);

    UIItemsControl::addItem(item);
}

void UIComboBox::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::MouseDownEvent) {
        if (m_popup->isOpend()) {
            m_popup->close();
        }
        else {
            m_popup->open();
        }
        e->handled = true;
        return;
    }
    else if (e->type() == UIEvents::MouseUpEvent) {
        return;
    }

    UIControl::onRoutedEvent(e);
}

} // namespace ln

