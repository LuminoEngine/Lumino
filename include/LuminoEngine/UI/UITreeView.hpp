#pragma once
#include "UIItemsElement.hpp"

namespace ln {
class UITreeItem;

enum class UITreeViewLayoutMode
{
    // 通常のレイアウト。子 Item は、親 Item がレイアウトする。
    Hierarchical,

    // インデント付きのリストのようにレイアウトする。
    // 各 Item は、論理ツリーは通常通りだが、VisualParent は TreeView となる。
    // 
    IndentedList,
};

class IUITreeItemVisitor
{
public:
    virtual void visit(UITreeItem* item) = 0;
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
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UITreeItem();
	void init();

private:
    void traverse(IUITreeItemVisitor* visitor);
    void expander_Checked(UIEventArgs* e);
    void expander_Unchecked(UIEventArgs* e);

    UITreeView* m_ownerTreeView;
	Ref<UIToggleButton> m_expanderButton;
    Ref<UIElement> m_headerContent;
    List<Ref<UITreeItem>> m_items;
    Ref<UILayoutPanel2> m_itemsLayout;
	Ref<UICollectionItemViewModel> m_model;

    friend class UITreeView;
};

class UITreeView
    : public UIItemsControl
{
public:
    //void setModel(UICollectionViewModel* model);

    bool isVirtualize() const { return m_model != nullptr; }

protected:
    virtual void onItemClick(UITreeItem* item, UIMouseEventArgs* e);
    virtual Ref<UITreeItem> onRenderItem(UICollectionItemViewModel* viewModel);

    // base interface
    virtual void onItemAdded(UICollectionItem* item) override;
	virtual void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;
    virtual void onSourcePropertyChanged(UINotifyPropertyChangedEventArgs* e) override;
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
    UITreeView();
    void init();

private:
    void addItemInternal(UITreeItem* item);
    void makeChildItems(UITreeItem* item);
    //UITreeItem* makeTreeViewItem();

    Ref<UICollectionViewModel> m_model;
    UITreeViewLayoutMode m_layoutMode = UITreeViewLayoutMode::IndentedList;
    List<UITreeItem*> m_listLayoutCache;
    bool m_initializing = true;

    friend class UITreeItem;
};




class UITreeView2;


/**
 * TreeView の要素です。
 *
 * UIControl は直接の子要素を、UIFrameLayout2 のレイアウト仕様に従ってレイアウトします。
 *
 * Visual States
 * ----------
 *
 * | Name           | Group          | Description    |
 * |----------------|----------------|----------------|
 * | Unselected     | Selection      |  |
 * | Selected       | Selection      |  |
 *
 */
class UITreeItem2
    : public UIControl
{
public:
    bool isExpanded() const;

    //UITreeView* treeView() const { return m_ownerTreeView; }

protected:
    //virtual void onExpanded();
    //virtual void onCollapsed();
    virtual void onSelected(UIEventArgs* e);
    virtual void onUnselected(UIEventArgs* e);

    // base interface
    void setContent(UIElement* value) override;
    void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;
    //virtual void onClick(UIMouseEventArgs* e) override;

    //// UIElement interface
    virtual const String& elementName() const  override { static String name = u"UITreeItem"; return name; }
    virtual void onRoutedEvent(UIEventArgs* e) override;
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
    UITreeItem2();
    bool init();

private:
    class IVisitor
    {
    public:
        virtual void visit(UITreeItem2* item) = 0;
    };

    void expander_Checked(UIEventArgs* e);
    void expander_Unchecked(UIEventArgs* e);
    void attemptCreateChildItemsInstance();
    UITreeView2* getTreeView();
    void dirtyVisualItemCount();
    void traverse(IVisitor* visitor);

    // Selector
    void setSelectedInternal(bool selected);

    //UITreeView* m_ownerTreeView;
    Ref<UIToggleButton> m_expanderButton;
    Ref<UIElement> m_headerContent;
    //List<Ref<UITreeItem>> m_items;
    //Ref<UILayoutPanel> m_itemsLayout;
    Ref<UICollectionItemViewModel> m_model;

    // measure/arrange 中のみで使える。
    // レイアウト中に TreeView が持つ情報にアクセスするのに、いちいち祖先を
    // 再帰的に検索するのは効率悪いため設けたもの。
    UITreeView2* m_layoutingOwnerTreeView = nullptr;
    int m_layoutDepth = 0;

    bool m_isSelected = false;

    friend class UITreeView2;
};

using UIGenerateTreeItemHandler = Delegate<void(UITreeItem2* item)>;

// - WPF だと、ListView は Selector の派生だが、TreeView はそうではない。ItemsControl の直接のサブクラス。
// - いったん UICollectionControl の派生にはしないようにしてみる。ほかの ListView とかとうまいこと共通化できる部分が少ないかもしれない。
//   ListView 実装後、共通化できそうなものがあれば作っていく。
class UITreeView2
    //: public UICollectionControl
    : public UIControl
{
public:
    /** ItemSubmitted イベントの通知を受け取るコールバックを登録します。*/
    Ref<EventConnection> connectOnChecked(Ref<UIGeneralEventHandler> handler);

    void setGenerateTreeItemHandler(Ref<UIGenerateTreeItemHandler> handler);

protected:
    virtual void onSelectionChanged(UISelectionChangedEventArgs* e);

    /** Enter キーやダブルクリックなどで、Item の主要なアクションを実行するタイミングで呼び出されます。sender は UITreeViewitem です。 */
    virtual void onItemSubmitted(UIEventArgs* e);

    //virtual void onItemClick(UITreeItem* item, UIMouseEventArgs* e);
    //virtual Ref<UITreeItem> onRenderItem(UICollectionItemModel* viewModel);

    // base interface
    //virtual void onSourcePropertyChanged(UINotifyPropertyChangedEventArgs* e) override;
    //virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;
    
    // base interface
    const String& elementName() const override { static String name = u"UITreeView"; return name; }
    void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;
    void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;
    void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
    UITreeView2();
    bool init();

private:
    bool isVirtualize() const { return m_model != nullptr; }
    void rebuildTreeFromViewModel();
    void addItemAsLogicalChildren(UITreeItem2* item);
    void addItemAsVisualChildren(UITreeItem2* item);

    // Selector
    void clearSelection();
    void selectItemExclusive(UITreeItem2* item);
    void notifyItemClicked(UITreeItem2* item);

    Ref<UITreeItem2> generateTreeItem(UICollectionItemViewModel* viewModel);

    Ref<UICollectionViewModel> m_model;
    Ref<UILayoutPanel2> m_itemsHostLayout;
    Ref<UIScrollViewHelper> m_scrollViewHelper;
    bool m_dirtyItemVisualTree = true;

    // Selector
    List<UITreeItem2*> m_selectedItems;

    Event<UIGeneralEventHandler> m_onItemSubmitted;
    Ref<UIGenerateTreeItemHandler> m_onGenerateTreeItem;

    friend class UITreeItem2;
};


} // namespace ln

