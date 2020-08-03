#pragma once
#include "../UIItemsElement.hpp"
#include "UIListBox.hpp"

namespace ln {
class UIListView;

class UIListViewItem
    : public UICollectionItem
{
public:
    virtual void setContent(UIElement* value) override;

protected:
    // base interface
    virtual const String& elementName() const  override { static String name = u"UIListViewItem"; return name; }
    virtual void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;

LN_CONSTRUCT_ACCESS:
    UIListViewItem();
    void init();

private:
    UIListView* m_ownerListView;
    Ref<UICollectionItemViewModel> m_model;

    friend class UIListView;
};


class UIListView
	: public UIItemsControl
{
public:
    void refresh();

protected:
    virtual Ref<UIListViewItem> onGenerateItem(UICollectionItemViewModel* viewModel);

    // base interface
    virtual const String& elementName() const  override { static String name = u"UIListView"; return name; }
    virtual void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;
    virtual void onSourcePropertyChanged(UINotifyPropertyChangedEventArgs* e) override;
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

LN_CONSTRUCT_ACCESS:
	UIListView();
	void init();

private:
    void addItemInternal(UIListViewItem* item);

    Ref<UICollectionViewModel> m_model;
};


class UIListViewModel2
    : public UIViewModel
{
public:
    virtual int getItemCount() = 0;
    virtual Ref<Variant> getItem(int index) = 0;

LN_CONSTRUCT_ACCESS:
    UIListViewModel2();
    bool init();
};


/**
 * UIListView 内の選択可能な項目を表します。
 */
class UIListViewItem2
    : public UIListItem
{
public:
    void setContent(int columnId, UIElement* element);
    void setText(int columnId, const ln::String& text);

protected:
    // base interface
    const String& elementName() const override { static String name = u"UIListBoxItem"; return name; }
    Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
    void onLogicalChildRemoved(UIElement* removedElement) override;

LN_CONSTRUCT_ACCESS:
    UIListViewItem2();
    bool init();

private:
    List<UIElement*> m_columnContents;
};


using UIGetListItemCountCallback = Delegate<int()>;
using UIRefreshListItemContentCallback = Delegate<void(int index, UIListViewItem2* item)>;

class UIListView2
    : public UIListItemsControl
{
public:

protected:
    virtual Ref<UIListViewItem2> onGenerateItem();
    virtual void onRefreshItemContent(int index, UIListViewItem2* item);

    // base interface
    const String& elementName() const override { static String name = u"UIListBox"; return name; }
    void onAddChild(UIElement* child) override;
    void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;
    void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

LN_CONSTRUCT_ACCESS:
    UIListView2();
    bool init();
    bool init(Ref<UIGetListItemCountCallback> itemCount, Ref<UIRefreshListItemContentCallback> refreshItem);

private:
    int getModelItemCount() const;
    void attemptRefreshItemInstance(UILayoutContext* layoutContext, const Size& finalSize);
    bool attemptRefreshContents();

    Ref<UIListViewModel2> m_listViewModel;
    float m_baseItemHeight = 0;
    bool m_dirtyItemInstances = true;
    bool m_dirtyItemContent = true;
    Ref<UIGetListItemCountCallback> m_getListItemCountCallback;
    Ref<UIRefreshListItemContentCallback> m_refreshListItemContentCallback;
};

} // namespace ln

