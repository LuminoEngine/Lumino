#pragma once
#include "UIItemsElement.hpp"

namespace ln {
class UIListView;

class UIListViewItem
    : public UICollectionItem
{
public:
    virtual void setContent(UIElement* value) override;

protected:
    // base interface
    virtual void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;

LN_CONSTRUCT_ACCESS:
    UIListViewItem();
    void init();

private:
    UIListView* m_ownerListView;
    Ref<UICollectionItemModel> m_model;

    friend class UIListView;
};


class UIListView
	: public UIItemsControl
{
protected:
    virtual Ref<UIListViewItem> onRenderItem(UICollectionItemModel* viewModel);

    // base interface
    virtual void onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel) override;
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UIListView();
	void init();

private:
    void addItemInternal(UIListViewItem* item);

    Ref<UICollectionModel> m_model;
};

} // namespace ln

