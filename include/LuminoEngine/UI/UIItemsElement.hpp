#pragma once
#include "Controls/UIControl.hpp"
#include "Controls/UIScrollView.hpp"
#include "UIItemsModel.hpp"

namespace ln {
class UIItemsControl;
class UIToggleButton;
class UITreeView;
class UILayoutPanel;

class UICollectionItem
	: public UIControl
{
public:
    /** Click イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
    Ref<EventConnection> connectOnClick(Ref<UIClickEventHandler> handler);

protected:
    virtual void onClick(UIMouseEventArgs* e);
	virtual void onSelected(UIEventArgs* e);
	virtual void onUnselected(UIEventArgs* e);

    // base interface
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
	virtual void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UICollectionItem();
	void init();

private:
	void setSelectedInternal(bool selected);

	UIItemsControl* m_ownerCollectionControl;
    //Ref<Variant> m_data;
    bool m_isPressed;
	bool m_isSelected;
    Event<UIClickEventHandler> m_onClick;

	friend class UIItemsControl;
};

// item 追加は addChild で。
class UIItemsControl	// TODO: UICollectionItem がほかにいい名前思いつかなければ、CollectionControl かなぁ・・・
	: public UIControl
{
public:

	/** 現在選択されている最初の項目を取得します。 */
	UICollectionItem* selectedItem() const;
    
    /** ItemClick イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnItemClick(Ref<UIClickEventHandler> handler);
    
    //LN_METHOD(Event)
	Ref<EventConnection> connectOnSelectionChanged(Ref<UISelectionChangedEventHandler> handler);

protected:
	//virtual UIControl* generateItem(UIElement* content) = 0;

    void setItemsLayoutPanel(UILayoutPanel* layout, bool setAsVisualChild = true);

    // UICollectionItem であるとわかっている 時に、addChild の代わりに使うと効率的に追加できる。
    void addItem(UICollectionItem* item);

	void removeItem(UICollectionItem* item);
    void removeAllItems();

    // 新しく item が追加された。
    virtual void onItemAdded(UICollectionItem* item);
    virtual void onItemClick(UICollectionItem* item, UIClickEventArgs* e);
    virtual void onSelectionChanged(UISelectionChangedEventArgs* e);

	// base interfaces
    void onAddChild(UIElement* child) override;
	virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

LN_CONSTRUCT_ACCESS:
	UIItemsControl();
	void init();

private:

public: // TODO:
    void selectItemExclusive(UICollectionItem* item);
	void notifyItemClicked(UICollectionItem* item);

    Ref<UILayoutPanel> m_itemssHostLayout;
	List<Ref<UICollectionItem>> m_selectionTargets;
	List<UICollectionItem*> m_selectedItems;
    Event<UIClickEventHandler> m_onItemClick;
    Event<UISelectionChangedEventHandler> m_onSelectionChanged;
	bool m_layoutItemsHostLayoutEnabled = true;

	friend class UICollectionItem;
};








//class UIItemElement
//	: public UIElement
//{
//public:
//    void setData(Variant* value) { m_data = value; }
//    Variant* data() const { return m_data; }
//
//protected:
//    virtual void onClick(UIMouseEventArgs* e);
//
//    // base interface
//    virtual void onRoutedEvent(UIEventArgs* e) override;
//
//LN_CONSTRUCT_ACCESS:
//	UIItemElement();
//	void init();
//
//private:
//    Ref<Variant> m_data;
//    bool m_isPressed;
//};

// Item を ItemElement でラップして扱う。
// ただ単に子要素を並べるだけであれば、ScrollView に addChild するだけでよいが、
// こちらは ItemElement を介して選択中であるとか、枠線を付けたりとかする。
//class UIItemContainerElement
//	: public UIScrollViewer
//{
//public:
//	UIItemContainerElement();
//	void init();
//
//private:
//};
//

class UIMenuItem
    : public UIControl  // TODO: HeaderdItemsControl
{
public:
    void setIcon(UIElement* value) { m_icon = value; }
    UIElement* icon() const { return m_icon; }

    // TODO: measure, arrange
    
LN_CONSTRUCT_ACCESS:
	UIMenuItem();
	void init();

private:
    Ref<UIElement> m_icon;
};


// item 追加は addChild で。
class UICollectionControl
    : public UIControl
{
public:

protected:
    // base interfaces
    void onAddChild(UIElement* child) override;
    
LN_CONSTRUCT_ACCESS:
	UICollectionControl();
	bool init();

private:
};

} // namespace ln

