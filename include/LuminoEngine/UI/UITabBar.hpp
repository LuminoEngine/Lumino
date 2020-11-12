#pragma once
#include "UIItemsElement.hpp"

namespace ln {

class UITabItem
	: public UICollectionItem
{
protected:
	virtual const String& elementName() const  override { static String name = u"UITabItem"; return name; }

LN_CONSTRUCT_ACCESS:
	UITabItem();
	void init();
};

// bar の部分だけ。content は別。
class UITabBar
	: public UIItemsControl
{
public:
	void addTab(UITabItem* item);
	void removeTab(UITabItem* item);
    void setSelectedTab(UITabItem* tab);
    UITabItem* selectedTab() const;

LN_CONSTRUCT_ACCESS:
	UITabBar();
	void init();

protected:
    virtual const String& elementName() const  override { static String name = u"UITabBar"; return name; }
    //virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    //virtual Size measureOverride(const Size& constraint) override;
    //virtual Size arrangeOverride(const Size& finalSize) override;
    //virtual void onRoutedEvent(UIEventArgs* e) override;

private:
};


class UITabBar2;

class UITabBarItem2
	: public UIControl
{
protected:
	virtual void onSelectedChanged(UIEventArgs* e);

	const String& elementName() const override { static String name = u"UITabBarItem"; return name; }
	void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UITabBarItem2();
	bool init();

private:
	//void setSelectedInternal(bool selected);

	friend class UITabBar2;
};

// bar の部分だけ。content は別。
class UITabBar2
	: public UIControl
{
public:
	/** SelectedTabChanged イベントの通知を受け取るコールバックを登録します。*/
	Ref<EventConnection> connectOnSelectedTabChanged(Ref<UIGeneralEventHandler> handler);

	//void addTab(UITabItem* item);
	//void removeTab(UITabItem* item);
	//void setSelectedTab(UITabItem* tab);
	UITabBarItem2* selectedTab() const { return m_selectedTab; }

LN_CONSTRUCT_ACCESS:
	UITabBar2();
	bool init();

protected:
	/** selectedTab が変更された時に呼び出されます。 */
	virtual void onSelectedTabChanged(UIEventArgs* e);

	const String& elementName() const override { static String name = u"UITabBar"; return name; }
	void onAddChild(UIElement* child) override;
	Size measureOverride(UILayoutContext* context, const Size& constraint) override;
	Size arrangeOverride(UILayoutContext* context, const Rect& finalArea) override;

private:
	void selectItem(UITabBarItem2* item);

	Ref<UILayoutPanel> m_itemsHostLayout;
	UITabBarItem2* m_selectedTab;
	Event<UIGeneralEventHandler> m_onSelectedTabChanged;

	friend class UITabBarItem2;
};

} // namespace ln

