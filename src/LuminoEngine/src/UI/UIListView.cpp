
#include "Internal.hpp"
#include <LuminoEngine/UI/UIListView.hpp>

namespace ln {

//==============================================================================
// UIListViewItem

UIListViewItem::UIListViewItem()
{
}

void UIListViewItem::init()
{
    UICollectionItem::init();
}

//==============================================================================
// UIListView

UIListView::UIListView()
{
}

void UIListView::init()
{
    UIItemContainerElement::init();
}

Size UIListView::measureOverride(const Size& constraint)
{
    return UIItemContainerElement::measureOverride(constraint);
}

Size UIListView::arrangeOverride(const Size& finalSize)
{
    return UIItemContainerElement::arrangeOverride(finalSize);
}

} // namespace ln

