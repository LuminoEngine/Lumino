#pragma once
#include "UIItemsElement.hpp"
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
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;

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

protected:
    // base interface
    const String& elementName() const override { static String name = u"UIListBoxItem"; return name; }

LN_CONSTRUCT_ACCESS:
    UIListViewItem2();
    bool init();

private:
};

class UIListView2
    : public UIListItemsControl
{
public:

protected:
    virtual Ref<UIListViewItem2> onGenerateItem();

    // base interface
    const String& elementName() const override { static String name = u"UIListBox"; return name; }
    void onAddChild(UIElement* child) override;
    void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;
    Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
    UIListView2();
    bool init();

private:
    void attemptRefreshItemInstance(UILayoutContext* layoutContext, const Size& finalSize);

    Ref<UIListViewModel2> m_listViewModel;
    float m_baseItemHeight = 0;
    bool m_dirtyItemInstances = true;
    bool m_dirtyItemContent = true;
};

} // namespace ln

