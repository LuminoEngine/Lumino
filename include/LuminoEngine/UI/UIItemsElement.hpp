#pragma once
#include "UIControl.hpp"
#include "UIScrollView.hpp"
#include "UIItemsModel.hpp"

namespace ln {
class UIItemsControl;
class UIToggleButton;
class UITreeView;


class UICollectionItem
	: public UIControl
{
public:
    void setData(Variant* value) { m_data = value; }
    Variant* data() const { return m_data; }

protected:
    virtual void onClick(UIMouseEventArgs* e);
	virtual void onSelected(UIEventArgs* e);
	virtual void onUnselected(UIEventArgs* e);

    // base interface
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UICollectionItem();
	void init();

private:
	void setSelectedInternal(bool selected);

	UIItemsControl* m_ownerCollectionControl;
    Ref<Variant> m_data;
    bool m_isPressed;
	bool m_isSelected;

	friend class UIItemsControl;
};

class UIItemsControl	// TODO: UICollectionItem がほかにいい名前思いつかなければ、CollectionControl かなぁ・・・
	: public UIScrollViewer
{
protected:
	//virtual UIControl* generateItem(UIElement* content) = 0;

    void setItemsLayoutPanel(UILayoutPanel2* layout);

	void addItem(UICollectionItem* item);

	// base interfaces
	//virtual void onRoutedEvent(UIEventArgs* e) override;

	virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UIItemsControl();
	void init();

private:
	void notifyItemClicked(UICollectionItem* item);

    Ref<UILayoutPanel2> m_itemssHostLayout;
	List<Ref<UICollectionItem>> m_selectionTargets;
	List<UICollectionItem*> m_selectedItems;

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
class UIItemContainerElement
	: public UIScrollViewer
{
public:
	UIItemContainerElement();
	void init();

private:
};


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


} // namespace ln

