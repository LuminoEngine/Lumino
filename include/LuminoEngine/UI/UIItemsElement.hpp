#pragma once
#include "UIScrollView.hpp"
#include "UIItemsModel.hpp"

namespace ln {
class UIToggleButton;
class UITreeView;

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

// 任意サイズの Item をリスト形式で扱う。
// サイズがばらばらでもいいので、仮想化は非対応。少量のフレキシブルなリストに使う。
// 固定サイズで仮想化対応するのは UIListView
class UIListBox
	: public UIItemContainerElement
{
LN_CONSTRUCT_ACCESS:
	UIListBox();
	void init();

private:
};


class UITreeViewItem
	: public UIItemElement
{
public:
    void setContent(UIElement* value);  // TODO: TreeList ように column が必要かも

    void addChild(UITreeViewItem* item);

    UITreeView* treeView() const { return m_ownerTreeView; }

protected:
    virtual void onExpanded();
    virtual void onCollapsed();

    // UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UITreeViewItem();
	void init();

private:
    void expander_Checked(UIEventArgs* e);
    void expander_Unchecked(UIEventArgs* e);

    UITreeView* m_ownerTreeView;
	Ref<UIToggleButton> m_expanderButton;
    Ref<UIElement> m_headerContent;
    List<Ref<UITreeViewItem>> m_items;
    Ref<UILayoutPanel2> m_itemsLayout;

    friend class UITreeView;
};

class UITreeView
	: public UIItemContainerElement
{
public:
	void setModel(UIItemsViewModel* model);

    // TODO: ベースクラスの addElement, addChild は TreeViewItem のヘッダの content とする。
    // 直接 UITreeViewItem を使いたい場合はこのメソッドを使う必要がある。
    void addItem(UITreeViewItem* item);

LN_CONSTRUCT_ACCESS:
	UITreeView();
	void init();

private:
    //UITreeViewItem* makeTreeViewItem();

	Ref<UIItemsViewModel> m_model;
};

} // namespace ln

