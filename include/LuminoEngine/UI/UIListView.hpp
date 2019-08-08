#pragma once
#include "UIItemsElement.hpp"

namespace ln {

class UIListViewItem
    : public UICollectionItem
{
public:
    // TODO: group

LN_CONSTRUCT_ACCESS:
    UIListViewItem();
    void init();

private:
};


class UIListView
	: public UIItemContainerElement
{
protected:
    // base interface
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UIListView();
	void init();

private:
};

} // namespace ln

