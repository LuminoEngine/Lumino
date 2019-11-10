
#include "Internal.hpp"
#include <LuminoEngine/UI/UILayoutPanel.hpp>
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

void UIListViewItem::setContent(UIElement* value)
{
    addElement(value);
}

void UIListViewItem::onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel)
{
    if (oldViewModel) {
        LN_NOTIMPLEMENTED();
    }

    m_model = dynamic_cast<UICollectionItemModel*>(newViewModel);
    if (!m_model) {
        LN_NOTIMPLEMENTED();
    }

    UIElement::setContent(m_model->getData(u""));
}

//==============================================================================
// UIListView

UIListView::UIListView()
{
}

void UIListView::init()
{
    UIItemsControl::init();

    auto layout = makeObject<UIStackLayout2>();
    layout->setOrientation(Orientation::Vertical);
    setItemsLayoutPanel(layout);

    setHorizontalContentAlignment(HAlignment::Left);
    setVerticalContentAlignment(VAlignment::Center);
}

void UIListView::refresh()
{
    for (auto& child : *m_logicalChildren) {
        m_itemssHostLayout->removeVisualChild(child);
    }
    removeAllChildren();



    if (m_model) {
        int count = m_model->getRowCount(nullptr);
        for (int i = 0; i < count; i++) {
            auto childModel = m_model->getIndex(i, 0, nullptr);
            auto itemData = m_model->getData(childModel, u"");

            //auto text = makeObject<UITextBlock>();
            //text->setText(itemData);

            //auto child = makeObject<UITreeItem>();
            //child->setContent(text);
            //child->setViewModel(childModel);
            //child->setData(makeVariant(childModel));
            auto child = onGenerateItem(childModel);

            addItemInternal(child);
        }
    }
}

Ref<UIListViewItem> UIListView::onGenerateItem(UICollectionItemModel* viewModel)
{
    auto item = makeObject<UIListViewItem>();
    item->setViewModel(viewModel);
    return item;
}

void UIListView::onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel)
{
    UIItemsControl::onViewModelChanged(newViewModel, oldViewModel);

    m_model = dynamic_cast<UICollectionModel*>(newViewModel);
    if (!m_model) {
        LN_NOTIMPLEMENTED();
    }

    refresh();
}

void UIListView::onSourcePropertyChanged(UINotifyPropertyChangedEventArgs* e)
{
    UIItemsControl::onSourcePropertyChanged(e);
    if (e->name().isEmpty()) {
        refresh();
    }
}

Size UIListView::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    return UIItemsControl::measureOverride(layoutContext, constraint);
}

Size UIListView::arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize)
{
    return UIItemsControl::arrangeOverride(layoutContext, finalSize);
}

void UIListView::addItemInternal(UIListViewItem* item)
{
    assert(item);
    item->m_ownerListView = this;
    addItem(item);
}

} // namespace ln

