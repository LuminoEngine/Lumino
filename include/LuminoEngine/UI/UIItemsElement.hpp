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


class UITreeItem
	: public UICollectionItem
{
public:
    virtual void setContent(UIElement* value) override;  // TODO: TreeList ように column が必要かも

    void addChild(UITreeItem* item);

    UITreeView* treeView() const { return m_ownerTreeView; }

protected:
    virtual void onExpanded();
    virtual void onCollapsed();

    // base interface
    virtual void onClick(UIMouseEventArgs* e) override;

    // UIElement interface
    virtual const String& elementName() const  override { static String name = u"UITreeItem"; return name; }
	virtual void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UITreeItem();
	void init();

private:
    void expander_Checked(UIEventArgs* e);
    void expander_Unchecked(UIEventArgs* e);

    UITreeView* m_ownerTreeView;
	Ref<UIToggleButton> m_expanderButton;
    Ref<UIElement> m_headerContent;
    List<Ref<UITreeItem>> m_items;
    Ref<UILayoutPanel> m_itemsLayout;
	Ref<UICollectionItemModel> m_model;

    friend class UITreeView;
};

class UITreeView
    : public UIItemsControl
{
public:
    //void setModel(UICollectionModel* model);

    bool isVirtualize() const { return m_model != nullptr; }

protected:
    virtual void onItemClick(UITreeItem* item, UIMouseEventArgs* e);

    // base interface
	virtual void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
    UITreeView();
    void init();

private:
    void addItemInternal(UITreeItem* item);
    void makeChildItems(UITreeItem* item);
    //UITreeItem* makeTreeViewItem();

    Ref<UICollectionModel> m_model;

    friend class UITreeItem;
};

} // namespace ln

