
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
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

    m_model = dynamic_cast<UICollectionItemViewModel*>(newViewModel);
    if (!m_model) {
        LN_NOTIMPLEMENTED();
    }

    UIElement::setContent(m_model->getValue(u""));
}

//==============================================================================
// UIListView

UIListView::UIListView()
{
}

void UIListView::init()
{
    UIItemsControl::init();

    auto layout = makeObject<UIStackLayout2_Obsolete>();
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
        int count = m_model->getItemCount();
        for (int i = 0; i < count; i++) {
            auto childModel = m_model->getItem(i);
           // auto itemData = m_model->getData(childModel, u"");

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

Ref<UIListViewItem> UIListView::onGenerateItem(UICollectionItemViewModel* viewModel)
{
    auto item = makeObject<UIListViewItem>();
    item->setViewModel(viewModel);
    return item;
}

void UIListView::onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel)
{
    UIItemsControl::onViewModelChanged(newViewModel, oldViewModel);

    m_model = dynamic_cast<UICollectionViewModel*>(newViewModel);
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



//==============================================================================
// UIListViewModel2

UIListViewModel2::UIListViewModel2()
{
}

bool UIListViewModel2::init()
{
    if (!UIViewModel::init()) return false;
    return true;
}

//==============================================================================
// UIListViewItem2

UIListViewItem2::UIListViewItem2()
{
}

bool UIListViewItem2::init()
{
    if (!UIListItem::init()) return false;
    return true;
}

//==============================================================================
// UIListView2

UIListView2::UIListView2()
{
}

bool UIListView2::init()
{
    if (!UIListItemsControl::init()) return false;
    return true;
}

Ref<UIListViewItem2> UIListView2::onGenerateItem()
{
    return ln::makeObject2<UIListViewItem2>();
}

void UIListView2::onAddChild(UIElement* child)
{
    LN_UNREACHABLE();
}

void UIListView2::onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel)
{
    if (oldViewModel) {
        LN_NOTIMPLEMENTED();
        return;
    }

    if (auto vm = dynamic_cast<UIListViewModel2*>(newViewModel)) {
        m_listViewModel = vm;
        m_dirtyItemInstances = true;
    }
}

Size UIListView2::arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize)
{
    attemptRefreshItemInstance(layoutContext, finalSize);
    return UIListItemsControl::arrangeOverride(layoutContext, finalSize);
}

void UIListView2::attemptRefreshItemInstance(UILayoutContext* layoutContext, const Size& finalSize)
{
    //if (m_logicalChildren->size() != m_listViewModel->getItemCount()) {
    //    // Model と View の item 数が異なっていれば refresh
    //    m_dirtyItemInstances = true;
    //}

    if (m_dirtyItemInstances) {
        if (m_listViewModel->getItemCount() > 0) {

            // 代表 Item から、Item の基本サイズを求めておく
            {
                Ref<UIListViewItem2> representative;
                if (m_logicalChildren->isEmpty()) {
                    representative = onGenerateItem();
                    addListItem(representative);
                    // arrangeOverride の中で新たに作ったものは style 更新と masure しておかないと、サイズが取れない
                    representative->updateStyleHierarchical(layoutContext->styleContext(), m_finalStyle);
                    representative->measureLayout(layoutContext, finalSize);
                }
                else {
                    // TODO: ちゃんと型チェック
                    representative = static_pointer_cast<UIListViewItem2>(m_logicalChildren->at(0));
                }
                m_baseItemHeight = representative->desiredSize().height;
                if (LN_ENSURE(m_baseItemHeight > 0.0f)) return;

                // View サイズに収まる分だけ、足りない分の Item があれば作る。
                // 内容についてはこの時点では更新しない。
                int maxRequiredCount = std::ceil(finalSize.height / m_baseItemHeight);
                int requiredCount = std::min(m_listViewModel->getItemCount(), maxRequiredCount);
                int diff = requiredCount - m_logicalChildren->size();
                for (int i = 0; i < diff; i++) {
                    addListItem(onGenerateItem());
                    m_dirtyItemContent = true;
                }
            }
        }
        else {
            removeAllItems();
        }

        m_dirtyItemInstances = true;
    }
}

} // namespace ln

