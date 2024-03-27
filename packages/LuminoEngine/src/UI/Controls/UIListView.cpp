
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/Layout/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIText.hpp>
#include <LuminoEngine/UI/Controls/UIListView.hpp>
#include "../UIStyleInstance.hpp"

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

    UIElement::setContent(m_model->getValue(_TT("")));
}

//==============================================================================
// UIListView

UIListView::UIListView()
{
}

void UIListView::init()
{
    UIItemsControl::init();

    auto layout = makeObject_deprecated<UIStackLayout2_Obsolete>();
    layout->setOrientation(UILayoutOrientation::Vertical);
    setItemsLayoutPanel(layout);

    setHorizontalContentAlignment(UIHAlignment::Left);
    setVerticalContentAlignment(UIVAlignment::Center);
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

            //auto text = makeObject_deprecated<UIText>();
            //text->setText(itemData);

            //auto child = makeObject_deprecated<UITreeItem>();
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
    auto item = makeObject_deprecated<UIListViewItem>();
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

Size UIListView::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    return UIItemsControl::arrangeOverride(layoutContext, finalArea);
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

void UIListViewItem2::setContent(int columnId, UIElement* element)
{
    if (LN_REQUIRE(element)) return;
    if (LN_REQUIRE(element->logicalParent() != this)) return;
    addElement(element);
    m_columnContents.add(element);
}

void UIListViewItem2::setText(int columnId, const ln::String& text)
{
    // TODO: pooling
    setContent(columnId, ln::UIText::create(text));
}

Size UIListViewItem2::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    // TODO: m_columnContents をカラムごとにレイアウトする

    return UIListItem::arrangeOverride(layoutContext, finalArea);
}

void UIListViewItem2::onLogicalChildRemoved(UIElement* removedElement)
{
    m_columnContents.remove(removedElement);
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

bool UIListView2::init(Ref<UIGetListItemCountCallback> itemCount, Ref<UIRefreshListItemContentCallback> refreshItem)
{
    if (!init()) return false;
    m_getListItemCountCallback = itemCount;
    m_refreshListItemContentCallback = refreshItem;
    m_dirtyItemInstances = true;
    return true;
}

Ref<UIListViewItem2> UIListView2::onGenerateItem()
{
    return ln::makeObject2<UIListViewItem2>();
}

void UIListView2::onRefreshItemContent(int index, UIListViewItem2* item)
{
    if (m_refreshListItemContentCallback) {
        m_refreshListItemContentCallback->call(index, item);
    }
    else {
        item->setText(0, String::fromNumber(index));
    }
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

void UIListView2::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
    attemptRefreshContents();
    UIListItemsControl::onUpdateStyle(styleContext, finalStyle);
}

Size UIListView2::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    const auto finalSize = finalArea.getSize();
    attemptRefreshItemInstance(layoutContext, finalSize);

    if (attemptRefreshContents()) {
        // Arrange で RefreshContents を実施した場合は、子要素の Style 更新と Measure を行っておく
        for (auto& child: *m_logicalChildren) {
            child->updateStyleHierarchical(layoutContext->styleContext(), m_finalStyle);
            child->measureLayout(layoutContext, finalSize);
        }
    }

    return UIListItemsControl::arrangeOverride(layoutContext, finalArea);
}

int UIListView2::getModelItemCount() const
{
    if (m_listViewModel)
        return m_listViewModel->getItemCount();
    else if (m_getListItemCountCallback)
        return m_getListItemCountCallback->call();
    else
        return 0;
}

void UIListView2::attemptRefreshItemInstance(UILayoutContext* layoutContext, const Size& finalSize)
{
    if (m_dirtyItemInstances) {
        int modelItemCount = getModelItemCount();
        if (modelItemCount > 0) {

            // 代表 Item から、Item の基本サイズを求めておく
            {
                Ref<UIListViewItem2> representative;
                if (m_logicalChildren->isEmpty()) {
                    representative = onGenerateItem();
                    addListItem(representative);
                    m_dirtyItemContent = true;
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
            }

            // View サイズに収まる分だけ、足りない分の Item があれば作る。
            // 内容についてはこの時点では更新しない。
            int maxRequiredCount = std::ceil(finalSize.height / m_baseItemHeight);
            int requiredCount = std::min(modelItemCount, maxRequiredCount);
            int diff = requiredCount - m_logicalChildren->size();
            for (int i = 0; i < diff; i++) {
                addListItem(onGenerateItem());
                m_dirtyItemContent = true;
            }
        }
        else {
            removeAllItems();
        }

        m_dirtyItemInstances = true;
    }
}

bool UIListView2::attemptRefreshContents()
{
    if (m_dirtyItemContent) {
        // TODO: Scroll
        for (int i = 0; i < m_logicalChildren->size(); i++) {
            onRefreshItemContent(i, static_pointer_cast<UIListViewItem2>(m_logicalChildren->at(i)));
        }

        m_dirtyItemContent = false;
        return true;
    }
    else {
        return false;
    }
}

} // namespace ln

