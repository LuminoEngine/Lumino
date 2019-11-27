
#include "Internal.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>
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
	m_layoutItemsHostLayoutEnabled = false;

	auto itemsHost = makeObject<UIStackLayout2>();
	itemsHost->setOrientation(Orientation::Vertical);
	m_itemsHost = itemsHost;
	setItemsLayoutPanel(m_itemsHost, false);

	// TODO: test
	m_itemsHost->setBackgroundColor(Color::BlueViolet);
	m_itemsHost->setMargin(5);

    m_popup = ln::makeObject<ln::UIPopup>();
    m_popup->setWidth(200);
    m_popup->setHeight(300);
    m_popup->setBackgroundColor(Color::AliceBlue);
    m_popup->setPlacementTarget(this);
	m_popup->addElement(m_itemsHost);
    addVisualChild(m_popup);


	// TODO: test
	addChild(u"test");
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

	UIItemsControl::onRoutedEvent(e);
}

void UIComboBox::onRender(UIRenderingContext* context)
{
	UIItemsControl::onRender(context);

	UIElement* test = m_logicalChildren->at(0);
	context->drawVisual(test);
}

} // namespace ln

