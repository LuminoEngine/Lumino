
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>

namespace ln {

//==============================================================================
// UICollectionItem

UICollectionItem::UICollectionItem()
	: m_ownerCollectionControl(nullptr)
    , m_isPressed(false)
	, m_isSelected(false)
{
}

void UICollectionItem::init()
{
	UIControl::init();
	auto vsm = getVisualStateManager();
	vsm->registerState(UIVisualStates::SelectionStates, UIVisualStates::Unselected);
	vsm->registerState(UIVisualStates::SelectionStates, UIVisualStates::Selected);
    vsm->gotoState(UIVisualStates::Unselected);
}

EventConnection UICollectionItem::connectOnClick(UIClickEventHandler handler)
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

Size UICollectionItem::measureOverride(const Size& constraint)
{
    return UIControl::measureOverride(constraint);
}

Size UICollectionItem::arrangeOverride(const Size& finalSize)
{
    return UIControl::arrangeOverride(finalSize);
}

void UICollectionItem::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent) {
		m_ownerCollectionControl->notifyItemClicked(this);
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

            onClick(static_cast<UIMouseEventArgs*>(e));
            return;
        }
    }

	UIControl::onRoutedEvent(e);
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

void UIItemsControl::setItemsLayoutPanel(UILayoutPanel2* layout)
{
    if (LN_REQUIRE(layout)) return;

    if (m_itemssHostLayout) {
        for (auto& item : m_logicalChildren) {
            m_itemssHostLayout->removeVisualChild(item);
        }
    }

    m_itemssHostLayout = layout;
    m_itemssHostLayout->m_ownerItemsControl = this;
    addVisualChild(m_itemssHostLayout);

    for (auto& item : m_logicalChildren) {
        m_itemssHostLayout->addVisualChild(item);
    }
}

void UIItemsControl::addItem(UICollectionItem* item)
{
	if (LN_REQUIRE(item)) return;
	m_selectionTargets.add(item);
	item->m_ownerCollectionControl = this;
    //addElement(item);

    m_logicalChildren.add(item);
    item->setLogicalParent(this);

    m_itemssHostLayout->addVisualChild(item);
}

EventConnection UIItemsControl::connectOnItemClick(UIClickEventHandler handler)
{
    return m_onItemClick.connect(handler);
}

EventConnection UIItemsControl::connectOnSelectionChanged(UISelectionChangedEventHandler handler)
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

Size UIItemsControl::measureOverride(const Size& constraint)
{
    if (m_itemssHostLayout) {
        m_itemssHostLayout->measureLayout(constraint);
        Size layoutSize = m_itemssHostLayout->desiredSize();
        Size localSize = UIElement::measureOverride(constraint);
        return Size::max(layoutSize, localSize);
    }
    else {
        return UIControl::measureOverride(constraint);
    }
}

Size UIItemsControl::arrangeOverride(const Size& finalSize)
{
    Rect contentSlotRect(0, 0, finalSize);
    contentSlotRect = contentSlotRect.makeDeflate(finalStyle()->padding);

    if (m_itemssHostLayout) {
        m_itemssHostLayout->arrangeLayout(contentSlotRect);
    	return finalSize;
    }
    else {
        return UIControl::arrangeOverride(finalSize);
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

UIItemContainerElement::UIItemContainerElement()
{
}

void UIItemContainerElement::init()
{
	UIScrollViewer::init();
}

//==============================================================================
// UIMenuItem

UIMenuItem::UIMenuItem()
{
}

void UIMenuItem::init()
{
    UIControl::init();
}

} // namespace ln

