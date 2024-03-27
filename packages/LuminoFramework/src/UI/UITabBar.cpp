#include "Internal.hpp"
#include <LuminoFramework/UI/UIStyle.hpp>
#include <LuminoFramework/UI/Layout/UILayoutPanel.hpp>
#include <LuminoFramework/UI/UITabBar.hpp>

namespace ln {

//==============================================================================
// UITabItem

UITabItem::UITabItem()
{
}

void UITabItem::init()
{
	UICollectionItem::init();
}

//==============================================================================
// UITabBar
//	WPF の TabItem は HeaderedContentControl の派生であり、ラベル部分とコンテンツ部分がひとつの Control に統合されている。
//	UE4 は SDockTab。WPF と同じく、Header と Content が統合されているようだ。
//	一方 Qt や主要な CSS フレームワークでは、Tab と Content は独立している。
//  Lumino としては分ける方向で行ってみるが、QTabWidget のように統合されたユーティリティを用意してもいいだろう。
//	分ける用途としてはカスタマイズ性だが、例えば昨今のブラウザの TabBar と Content の間に AddressBar を入れたいようなときに使う。
//
//	クローズボタンは、CloseEvent を RoutedEvent で上に投げる。
//
//	- クローズボタン
//	- ピン止めボタン
//	- アイコン
//	- 文字色変更
//	- バッジアイコン (VSCodeの PROBLEMS 数)
//

UITabBar::UITabBar()
{
}

void UITabBar::init()
{
	UIItemsControl::init();

	auto layout = makeObject_deprecated<UIStackLayout2_Obsolete>();
	layout->setOrientation(UILayoutOrientation::Horizontal);
    setItemsLayoutPanel(layout);
}

void UITabBar::addTab(UITabItem* item)
{
	addItem(item);
}

void UITabBar::removeTab(UITabItem* item)
{
	removeItem(item);
}

void UITabBar::setSelectedTab(UITabItem* tab)
{
    selectItemExclusive(tab);
}

UITabItem* UITabBar::selectedTab() const
{
    return static_cast<UITabItem*>(m_selectedItems[0]);
}


//==============================================================================
// UITabBarItem

UITabBarItem2::UITabBarItem2()
{
	m_objectManagementFlags.unset(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
}

bool UITabBarItem2::init()
{
	if (!UIControl::init()) return false;

	auto vsm = getVisualStateManager();
	vsm->registerState(UIVisualStates::SelectionStates, UIVisualStates::Unselected);
	vsm->registerState(UIVisualStates::SelectionStates, UIVisualStates::Selected);
	vsm->gotoState(UIVisualStates::Unselected);

	return true;
}

void UITabBarItem2::onSelectedChanged(UIEventArgs* e)
{
}

void UITabBarItem2::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent) {
		if (auto* t = logicalParentAs<UITabBar2>()) {
			t->selectItem(this);
			e->handled = true;
			return;
		}
	}

	UIControl::onRoutedEvent(e);
}

//void UITabBarItem2::setSelectedInternal(bool selected)
//{
//	if (auto* tabbar = logicalParentAs<UITabBar2>()) {
//
//
//	}
//
//	if (m_isSelected != selected) {
//		m_isSelected = selected;
//
//		if (m_isSelected) {
//			onSelected(UIEventArgs::create(this, UIEvents::Selected));
//			getVisualStateManager()
//		}
//		else {
//			onUnselected(UIEventArgs::create(this, UIEvents::Selected));
//			getVisualStateManager()->gotoState(UIVisualStates::Unselected);
//		}
//	}
//}

//==============================================================================
// UITabBar

UITabBar2::UITabBar2()
	: m_selectedTab(nullptr)
{
}

bool UITabBar2::init()
{
	if (!UIControl::init()) return false;

	auto layout = makeObject_deprecated<UIStackLayout>();
	layout->setOrientation(UILayoutOrientation::Horizontal);
	addVisualChild(layout);
	m_itemsHostLayout = layout;

	return true;
}

Ref<EventConnection> UITabBar2::connectOnSelectedTabChanged(Ref<UIGeneralEventHandler> handler)
{
	return m_onSelectedTabChanged.connect(handler);
}

void UITabBar2::onSelectedTabChanged(UIEventArgs* e)
{
	m_onSelectedTabChanged.raise(e);
}

void UITabBar2::onAddChild(UIElement* child)
{
	// TODO: dynamic_cast じゃなくてフラグで
	auto* item = dynamic_cast<UITabBarItem2*>(child);
	if (LN_REQUIRE(item)) return;
	if (LN_REQUIRE(!child->logicalParent())) return;

	m_logicalChildren->add(child);
	child->m_logicalParent = this;

	m_itemsHostLayout->addVisualChild(child);

	if (!m_selectedTab) {
		selectItem(item);
	}
}

Size UITabBar2::measureOverride(UILayoutContext* context, const Size& constraint)
{
	m_itemsHostLayout->measureLayout(context, constraint);
	return UIControl::measureOverride(context, constraint);
}

Size UITabBar2::arrangeOverride(UILayoutContext* context, const Rect& finalArea)
{
	m_itemsHostLayout->arrangeLayout(context, Rect(0, 0, finalArea.getSize()));
	return UIControl::arrangeOverride(context, finalArea);
}

void UITabBar2::selectItem(UITabBarItem2* item)
{
	if (m_selectedTab != item) {
		UITabBarItem2* old = m_selectedTab;
		m_selectedTab = item;
	
		if (old) {
			old->getVisualStateManager()->gotoState(UIVisualStates::Unselected);
			old->onSelectedChanged(UIEventArgs::create(this, UIEvents::Default));
		}
		if (m_selectedTab) {
			m_selectedTab->getVisualStateManager()->gotoState(UIVisualStates::Selected);
			m_selectedTab->onSelectedChanged(UIEventArgs::create(this, UIEvents::Default));
		}

		onSelectedTabChanged(UIEventArgs::create(this, UIEvents::SelectionChanged));
	}
}

} // namespace ln

