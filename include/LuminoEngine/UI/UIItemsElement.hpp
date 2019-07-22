#pragma once
#include "UIControl.hpp"
#include "UIScrollView.hpp"
#include "UIItemsModel.hpp"

namespace ln {
class UIToggleButton;
class UITreeView;


class UICollectionItem
	: public UIControl
{
public:

protected:
	virtual void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UICollectionItem();
	void init();

private:
};

class UIItemsControl
	: public UIControl
{
protected:
	//virtual UIControl* generateItem(UIElement* content) = 0;

	void addItem(UICollectionItem* item);

	// base interfaces
	//virtual void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UIItemsControl();
	void init();

private:
	List<Ref<UICollectionItem>> m_selectionTargets;
};








class UIItemElement
	: public UIElement
{
public:
    void setData(Variant* value) { m_data = value; }
    Variant* data() const { return m_data; }

LN_CONSTRUCT_ACCESS:
	UIItemElement();
	void init();

private:
    Ref<Variant> m_data;
};

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
	: public UIItemElement
{
public:
    void setContent(UIElement* value);  // TODO: TreeList ように column が必要かも

    void addChild(UITreeItem* item);

    UITreeView* treeView() const { return m_ownerTreeView; }

protected:
    virtual void onExpanded();
    virtual void onCollapsed();

    // UIElement interface
    virtual const String& elementName() const  override { static String name = u"UITreeItem"; return name; }
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

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
    Ref<UILayoutPanel2> m_itemsLayout;

    friend class UITreeView;
};

class UITreeView
    : public UIItemsControl
{
public:
    void setModel(UICollectionModel* model);

    bool isVirtualize() const { return m_model != nullptr; }

protected:
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

