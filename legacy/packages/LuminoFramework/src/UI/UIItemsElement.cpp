#include "Internal.hpp"
#include <LuminoFramework/UI/UIStyle.hpp>
#include <LuminoFramework/UI/Layout/UILayoutPanel.hpp>
#include <LuminoFramework/UI/UIItemsElement.hpp>

namespace ln {

//==============================================================================
// UICollectionItem

UICollectionItem::UICollectionItem()
	: m_ownerCollectionControl(nullptr)
    , m_isPressed(false)
	, m_isSelected(false)
{
	m_objectManagementFlags.unset(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
}

void UICollectionItem::init()
{
	UIControl::init();
	auto vsm = getVisualStateManager();
	vsm->registerState(UIVisualStates::SelectionStates, UIVisualStates::Unselected);
	vsm->registerState(UIVisualStates::SelectionStates, UIVisualStates::Selected);
    vsm->gotoState(UIVisualStates::Unselected);
}

Ref<EventConnection> UICollectionItem::connectOnClick(Ref<UIClickEventHandler> handler)
{
    return m_onClick.connect(handler);
}

void UICollectionItem::onClick(UIMouseEventArgs* e)
{
    auto args = UIClickEventArgs::create(this, UIEvents::Click, e->getClickCount());
    m_onClick.raise(args);
    if (m_ownerCollectionControl) {
        m_ownerCollectionControl->onItemClick(this, args);
    }
}

void UICollectionItem::onSelected(UIEventArgs* e)
{
}

void UICollectionItem::onUnselected(UIEventArgs* e)
{
}

Size UICollectionItem::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    return UIControl::measureOverride(layoutContext, constraint);
}

Size UICollectionItem::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    return UIControl::arrangeOverride(layoutContext, finalArea);
}

void UICollectionItem::onRoutedEvent(UIEventArgs* e)
{
	UIControl::onRoutedEvent(e);

	if (e->type() == UIEvents::MouseDownEvent) {
		e->handled = true;


        m_isPressed = true;
        retainCapture();
        e->handled = true;

		return;
	}

    else if (e->type() == UIEvents::MouseUpEvent) {
        if (m_isPressed) {
            releaseCapture();
            e->handled = true;
            m_isPressed = false;

			m_ownerCollectionControl->notifyItemClicked(this);
            onClick(static_cast<UIMouseEventArgs*>(e));
            return;
        }
    }
}

void UICollectionItem::setSelectedInternal(bool selected)
{
	if (m_isSelected != selected) {
		m_isSelected = selected;

		if (m_isSelected) {
			onSelected(UIEventArgs::create(this, UIEvents::Selected));
			getVisualStateManager()->gotoState(UIVisualStates::Selected);
		}
		else {
			onUnselected(UIEventArgs::create(this, UIEvents::Selected));
			getVisualStateManager()->gotoState(UIVisualStates::Unselected);
		}
	}
}

//==============================================================================
// UIItemsControl
//	ただ複数の Item を追加できる、っていうだけだと、UIControl と変わらない。
//	WPF でいう Selector の機能も兼ねる。

UIItemsControl::UIItemsControl()
{
    m_enabledDirectChildrenContentAlignment = false;
}

void UIItemsControl::init()
{
    UIControl::init();

	// dummy for single select mode
	m_selectedItems.add(nullptr);
}

UICollectionItem* UIItemsControl::selectedItem() const
{
	return m_selectedItems[0];
}

void UIItemsControl::setItemsLayoutPanel(UILayoutPanel* layout, bool setAsVisualChild)
{
    if (LN_REQUIRE(layout)) return;

    if (m_itemssHostLayout) {
        for (auto& item : *m_logicalChildren) {
            m_itemssHostLayout->removeVisualChild(item);
        }
    }

    m_itemssHostLayout = layout;
    m_itemssHostLayout->m_ownerItemsControl = this;

	if (setAsVisualChild) {
		addVisualChild(m_itemssHostLayout);
	}

	for (auto& item : *m_logicalChildren) {
		m_itemssHostLayout->addVisualChild(item);
	}
}

void UIItemsControl::addItem(UICollectionItem* item)
{
	if (LN_REQUIRE(item)) return;

	m_selectionTargets.add(item);
	item->m_ownerCollectionControl = this;

    m_logicalChildren->add(item);
    item->setLogicalParent(this);

    if (m_itemssHostLayout) {
        m_itemssHostLayout->addVisualChild(item);
    }

    onItemAdded(item);
}

void UIItemsControl::onItemAdded(UICollectionItem* item)
{
}

void UIItemsControl::removeItem(UICollectionItem* item)
{
	if (LN_REQUIRE(item)) return;
	if (LN_REQUIRE(item->m_ownerCollectionControl == this)) return;

	m_selectionTargets.remove(item);
	item->m_ownerCollectionControl = nullptr;

	m_logicalChildren->remove(item);
	item->setLogicalParent(nullptr);

	m_itemssHostLayout->removeVisualChild(item);
}

void UIItemsControl::removeAllItems()
{
    for (auto& item : m_selectionTargets) {
        item->m_ownerCollectionControl = nullptr;
    }
    m_selectionTargets.clear();

    for (auto& item : *m_logicalChildren) {
        item->setLogicalParent(nullptr);
        m_itemssHostLayout->removeVisualChild(item);
    }
    m_logicalChildren->clear();
}

Ref<EventConnection> UIItemsControl::connectOnItemClick(Ref<UIClickEventHandler> handler)
{
    return m_onItemClick.connect(handler);
}

Ref<EventConnection> UIItemsControl::connectOnSelectionChanged(Ref<UISelectionChangedEventHandler> handler)
{
    return m_onSelectionChanged.connect(handler);
}

void UIItemsControl::onItemClick(UICollectionItem* item, UIClickEventArgs* e)
{
    m_onItemClick.raise(e);
}

void UIItemsControl::onSelectionChanged(UISelectionChangedEventArgs* e)
{
    m_onSelectionChanged.raise(e);
}

void UIItemsControl::onAddChild(UIElement* child)
{
    // TODO: dynamic_cast じゃなくてフラグで見たい
    auto* item = dynamic_cast<UICollectionItem*>(child);
    if (!item) {
        LN_NOTIMPLEMENTED();
        return;
    }

    addItem(item);

}

void UIItemsControl::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
    UIControl::onUpdateStyle(styleContext, finalStyle);

	//if (finalStyle->overflowX == UIOverflowBehavior::Scroll) {
	//	setHScrollbarVisible(true);
	//}
	//else {
	//	setHScrollbarVisible(false);
	//}
	//if (finalStyle->overflowY == UIOverflowBehavior::Scroll) {
	//	setVScrollbarVisible(true);
	//}
	//else {
	//	setVScrollbarVisible(false);
	//}
}

Size UIItemsControl::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    if (m_layoutItemsHostLayoutEnabled && m_itemssHostLayout) {
        m_itemssHostLayout->measureLayout(layoutContext, constraint);
        Size layoutSize = m_itemssHostLayout->desiredSize();
        Size localSize = UIElement::measureOverride(layoutContext, constraint);
        return Size::max(layoutSize, localSize);
    }
    else {
        return UIControl::measureOverride(layoutContext, constraint);
    }
}

Size UIItemsControl::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    const auto finalSize = finalArea.getSize();
	Rect contentSlotRect = detail::LayoutHelper::makePaddingRect(this, finalSize);

    if (m_layoutItemsHostLayoutEnabled && m_itemssHostLayout) {
        m_itemssHostLayout->arrangeLayout(layoutContext, contentSlotRect);
    	return finalSize;
    }
    else {
        return UIControl::arrangeOverride(layoutContext, finalArea);
    }
}

void UIItemsControl::selectItemExclusive(UICollectionItem* item)
{
    if (m_selectedItems[0]) {
        m_selectedItems[0]->setSelectedInternal(false);
    }

    m_selectedItems[0] = item;
    item->setSelectedInternal(true);

    onSelectionChanged(UISelectionChangedEventArgs::create(this, UIEvents::SelectionChanged));
}

void UIItemsControl::notifyItemClicked(UICollectionItem* item)
{
    selectItemExclusive(item);
}


//void UIItemsControl::onRoutedEvent(UIEventArgs* e)
//{
//	if (e->type() == UIEvents::MouseDownEvent) {
//		
//	}
//
//	UIControl::onRoutedEvent(e);
//}








//==============================================================================
// UIItemElement

//UIItemElement::UIItemElement()
//    : m_isPressed(false)
//{
//}
//
//void UIItemElement::init()
//{
//	UIElement::init();
//}
//
//void UIItemElement::onClick(UIMouseEventArgs* e)
//{
//}
//
//void UIItemElement::onRoutedEvent(UIEventArgs* e)
//{
//    if (e->type() == UIEvents::MouseDownEvent) {
//        m_isPressed = true;
//        retainCapture();
//        e->handled = true;
//        return;
//    }
//    else if (e->type() == UIEvents::MouseUpEvent) {
//        if (m_isPressed) {
//            releaseCapture();
//            e->handled = true;
//            m_isPressed = false;
//
//            onClick(static_cast<UIMouseEventArgs*>(e));
//            return;
//        }
//    }
//
//    UIElement::onRoutedEvent(e);
//}
//
//==============================================================================
// UIItemContainerElement

//UIItemContainerElement::UIItemContainerElement()
//{
//}
//
//void UIItemContainerElement::init()
//{
//	UIScrollViewer::init();
//}

//==============================================================================
// UIMenuItem

UIMenuItem::UIMenuItem()
{
}

void UIMenuItem::init()
{
    UIControl::init();
}


//==============================================================================
// UICollectionControl

UICollectionControl::UICollectionControl()
{

}

bool UICollectionControl::init()
{
    if (!UIControl::init()) return false;
    return true;
}

void UICollectionControl::onAddChild(UIElement* child)
{

}

} // namespace ln

