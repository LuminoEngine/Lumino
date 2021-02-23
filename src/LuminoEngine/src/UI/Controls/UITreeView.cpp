
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/Layout/UILayoutPanel.hpp>
#include <LuminoEngine/UI/Controls/UIButton.hpp>
#include <LuminoEngine/UI/Controls/UIScrollView.hpp>
#include <LuminoEngine/UI/Controls/UITreeView.hpp>
#include "../UIStyleInstance.hpp"

namespace ln {

//==============================================================================
// UITreeItem

UITreeItem::UITreeItem()
    : m_ownerTreeView(nullptr)
{
}

void UITreeItem::init()
{
    UICollectionItem::init();

    auto vsm = getVisualStateManager();
    vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::Normal);
    vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::MouseOver);
    vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::Pressed);

    // TODO: CreationContext とか用意したほうがいいかも。init を public にしないとダメだし。
	m_expanderButton = makeObject<UIToggleButton>();
    //m_expanderButton = makeRef<UIToggleButton>();
    //m_expanderButton->m_objectManagementFlags.unset(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
    //m_expanderButton->init();

	m_expanderButton->addClass(u"UITreeItem-Expander");
    m_expanderButton->connectOnChecked(bind(this, &UITreeItem::expander_Checked));
    m_expanderButton->connectOnUnchecked(bind(this, &UITreeItem::expander_Unchecked));

    //m_expanderButton->setBackgroundColor(Color::Red);

	addVisualChild(m_expanderButton);

    //auto layout = makeObject<UIStackLayout_Obsolete>();
    auto layout = makeObject<UIStackLayout>();
    layout->setOrientation(UILayoutOrientation::Vertical);
    m_itemsLayout = layout;
    addVisualChild(m_itemsLayout);
}

void UITreeItem::setContent(UIElement* value)
{
    assert(!m_headerContent);    // TODO
    m_headerContent = value;
    addVisualChild(m_headerContent);
}

void UITreeItem::addChild(UITreeItem* item)
{
    if (LN_REQUIRE(item)) return;
    m_items.add(item);
    item->m_ownerTreeView = m_ownerTreeView;
    m_itemsLayout->addVisualChild(item);   // TODO: 多重追加対策
}

void UITreeItem::onExpanded()
{
    m_ownerTreeView->makeChildItems(this);
    
    for (auto& child : m_items) {
        child->m_internalVisibility = UIVisibility::Visible;
    }
}

void UITreeItem::onCollapsed()
{
    for (auto& child : m_items) {
        child->m_internalVisibility = UIVisibility::Collapsed;
    }
}

void UITreeItem::onClick(UIMouseEventArgs* e)
{
    UICollectionItem::onClick(e);
    m_ownerTreeView->onItemClick(this, e);
}

void UITreeItem::onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel)
{
	if (oldViewModel) {
		LN_NOTIMPLEMENTED();
	}

	m_model = dynamic_cast<UICollectionItemViewModel*>(newViewModel);
	if (!m_model) {
		LN_NOTIMPLEMENTED();
	}

    // TODO: model の変更通知対応
    if (m_model->getChildrenCount() > 0) {
        m_expanderButton->m_internalVisibility = UIVisibility::Visible;
    }
    else {
        m_expanderButton->m_internalVisibility = UIVisibility::Hidden;
    }

    UIElement::setContent(m_model->getValue(u""));
}

Size UITreeItem::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    //bool expanderVisible = false;
    //if (m_model) {
    //    //m_model
    //}


    struct ElementList : public IUIElementList {
        List<Ref<UITreeItem>>* list;
        virtual int getElementCount() const { return list->size(); }
        virtual UIElement* getElement(int i) const { return list->at(i); }
    } list;
    list.list = &m_items;

    Size size;

    // expander
    m_expanderButton->measureLayout(layoutContext, constraint);
    size.width += m_expanderButton->desiredSize().width;
    size.height = std::max(size.height, m_expanderButton->desiredSize().height);

    // header
    if (m_headerContent) {
        m_headerContent->measureLayout(layoutContext, constraint);
        size.width += m_headerContent->desiredSize().width;
        size.height = std::max(size.height, m_headerContent->desiredSize().height);
    }

    // children
    m_itemsLayout->measureLayout(layoutContext, constraint);
    size.width = std::max(size.width, m_itemsLayout->desiredSize().width);
    size.height += m_itemsLayout->desiredSize().height;

    Size desiredSize = UICollectionItem::measureOverride(layoutContext, constraint);

    return Size::max(size, desiredSize);
}

Size UITreeItem::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    const auto finalSize = finalArea.getSize();

    struct ElementList : public IUIElementList {
        List<Ref<UITreeItem>>* list;
        virtual int getElementCount() const { return list->size(); }
        virtual UIElement* getElement(int i) const { return list->at(i); }
    } list;
    list.list = &m_items;


    // expander
    float headerHeight = m_finalStyle->minHeight;
    Rect expanderSlot(0, 0, headerHeight, headerHeight);
    m_expanderButton->arrangeLayout(layoutContext, expanderSlot); // TODO: actualsize 返すようにしていいかも

    Rect area = Rect(expanderSlot.width, 0, finalSize.width - expanderSlot.width, expanderSlot.height);

    // header
	float headerContentHeight = 0;
    if (m_headerContent) {
        //area.x += m_expanderButton->m_actualSize.width;
        //area.width -= m_expanderButton->m_actualSize.width;

        Rect contentSlotRect;
        detail::LayoutHelper::adjustAlignment(
            area, m_headerContent->desiredSize(),
            m_ownerTreeView->m_finalStyle->horizontalContentAlignment, m_ownerTreeView->m_finalStyle->verticalContentAlignment, &contentSlotRect);


        m_headerContent->arrangeLayout(layoutContext, contentSlotRect);
		headerContentHeight = m_headerContent->actualSize().height;
    }

    // children
    area.y = std::max(m_expanderButton->actualSize().height, headerContentHeight);
    m_itemsLayout->arrangeLayout(layoutContext, area);

    return finalSize;
}

void UITreeItem::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::MouseDownEvent) {
        if (static_cast<UIMouseEventArgs*>(e)->getClickCount() == 2) {
        }
    }
    UICollectionItem::onRoutedEvent(e);
}

void UITreeItem::traverse(IUITreeItemVisitor* visitor)
{
    if (isRenderVisible()) {
        visitor->visit(this);
        for (auto& child : m_items) {
            child->traverse(visitor);
        }
    }
}

void UITreeItem::expander_Checked(UIEventArgs* e)
{
    onExpanded();
}

void UITreeItem::expander_Unchecked(UIEventArgs* e)
{
    onCollapsed();
}

//==============================================================================
// UITreeView
//  UITreeView : View-Model パターンで実装するときに使う
//  UITreeControl : 直接 Item を操作するのに使う
//  としていたが、UITreeControl は廃止する。やっぱり似たような名前の要素を増やしたくない。
//  代わりに UITreeView で両方対応する。
//  setModel() されたかどうかで、Virtialize モードかどうかを区別する。
//  Virtialize モードON の時は addItem() 禁止とかにする。（末尾に追加とかでもよさそうだけど）

UITreeView::UITreeView()
{
}

void UITreeView::init()
{
    UIItemsControl::init();

    auto layout = makeObject<UIStackLayout>();
    layout->setOrientation(UILayoutOrientation::Vertical);
    setItemsLayoutPanel(layout);

    setHorizontalContentAlignment(UIHAlignment::Left);
    setVerticalContentAlignment(UIVAlignment::Center);
}

//void UITreeView::setModel(UICollectionViewModel* model)
//{
//    m_model = model;
//    //int count = m_model->getRowCount(nullptr);
//    //for (int i = 0; i < count; i++) {
//    //    auto itemModel = m_model->getIndex(i, 0, nullptr);
//    //    auto itemData = m_model->getData(itemModel, u"");
//
//    //    auto text = makeObject<UIText>();
//    //    text->setText(itemData);
//
//    //    auto item = makeObject<UITreeItem>();
//    //    item->setContent(text);
//    //    item->setData(makeVariant(itemModel));
//
//    //    //if (!parent) {
//    //        addElement(item);
//    //    //}
//    //    //else {
//    //   //     parent->addChild(item);
//    //    //}
//    //}
//}

void UITreeView::onItemClick(UITreeItem* item, UIMouseEventArgs* e)
{
}

Ref<UITreeItem> UITreeView::onRenderItem(UICollectionItemViewModel* viewModel)
{
    auto item = makeObject<UITreeItem>();
    item->setViewModel(viewModel);
    return item;
}

void UITreeView::onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel)
{
	if (oldViewModel) {
		LN_NOTIMPLEMENTED();
	}

	m_model = dynamic_cast<UICollectionViewModel*>(newViewModel);
	if (!m_model) {
		LN_NOTIMPLEMENTED();
	}

	makeChildItems(nullptr);
}

void UITreeView::onSourcePropertyChanged(UINotifyPropertyChangedEventArgs* e)
{
    if (e->action() == UICollectionChangedAction::Reset) {
        removeAllChildren();
        makeChildItems(nullptr);
    }
}

Size UITreeView::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
#if 0
    if (m_layoutMode == UITreeViewLayoutMode::IndentedList) {
        class LocalVisitor : public IUITreeItemVisitor
        {
        public:
            List<UITreeItem*>* list;
            virtual void visit(UITreeItem* item)
            {
                list->add(item);
            }
        };
        LocalVisitor v;
        v.list = &m_listLayoutCache;
        m_listLayoutCache.clear();

        for (auto& child : *m_logicalChildren) {
            static_pointer_cast<UITreeItem>(child)->traverse(&v);
        }

        if (m_listLayoutCache.size() >= 1) {
            for (auto& item : m_listLayoutCache) {
                item->measureLayout(layoutContext, constraint);
            }

            Size size = m_listLayoutCache.front()->desiredSize();

            // 全 item 固定サイズで算出
            size.height = size.height * m_listLayoutCache.size();

            return size;
        }
    }
#endif

    return UIItemsControl::measureOverride(layoutContext, constraint);
}

Size UITreeView::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
#if 0
    if (m_layoutMode == UITreeViewLayoutMode::IndentedList) {
        if (m_listLayoutCache.size() >= 1) {
            Rect rect;
            for (auto& item : m_listLayoutCache) {
                rect.width = item->desiredSize().width;
                rect.height = item->desiredSize().height;
                item->arrangeLayout(layoutContext, rect);
                rect.y += rect.height;
            }

            return finalSize;
        }
    }
#endif

    return UIItemsControl::arrangeOverride(layoutContext, finalArea);
}

void UITreeView::addItemInternal(UITreeItem* item)
{
    assert(item);
    item->m_ownerTreeView = this;
    addItem(item);
}

void UITreeView::makeChildItems(UITreeItem* item)
{
    assert(isVirtualize());

    UICollectionItemViewModel* itemModel = nullptr; // null is root
    if (item) {

        itemModel = item->m_model;
        int count = itemModel->getChildrenCount();
        if (item->m_items.size() != count) {
            item->removeAllChildren();

            for (int i = 0; i < count; i++) {
                auto childModel = itemModel->getItem(i);
                //auto itemData = m_model->getData(childModel, u"");

                //auto text = makeObject<UIText>();
                //text->setText(itemData);

                //auto child = makeObject<UITreeItem>();
                //child->setContent(text);
                //child->setViewModel(childModel);
                //child->setData(makeVariant(childModel));
                auto child = onRenderItem(childModel);

                item->addChild(child);
            }
        }


    }
    else {
        int count = m_model->getItemCount();
        for (int i = 0; i < count; i++) {
            auto childModel = m_model->getItem(i);
            auto child = onRenderItem(childModel);

            addItemInternal(child);
        }
    }
}

void UITreeView::onItemAdded(UICollectionItem* item)
{
    //if (m_layoutMode == UITreeViewLayoutMode::IndentedList) {
    //    addVisualChild(item);
    //}
}

//==============================================================================
// UITreeItem2

UITreeItem2::UITreeItem2()
    : m_parentItem(nullptr)
    , m_headerContent(nullptr)
{
    m_objectManagementFlags.unset(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
}

bool UITreeItem2::init()
{
    if (!UIControl::init()) return false;

    auto vsm = getVisualStateManager();
    vsm->registerState(UIVisualStates::SelectionStates, UIVisualStates::Unselected);
    vsm->registerState(UIVisualStates::SelectionStates, UIVisualStates::Selected);
    vsm->gotoState(UIVisualStates::Unselected);

    // TODO: CreationContext とか用意したほうがいいかも。init を public にしないとダメだし。
    m_expanderButton = makeObject<UIToggleButton>();
    m_expanderButton->addClass(u"UITreeItem-Expander");
    m_expanderButton->connectOnChecked(bind(this, &UITreeItem2::expander_Checked));
    m_expanderButton->connectOnUnchecked(bind(this, &UITreeItem2::expander_Unchecked));

    //setBackgroundColor(Color::Red);
    //m_expanderButton->setBackgroundColor(Color::Red);

    addVisualChild(m_expanderButton);



    return true;
}

bool UITreeItem2::isExpanded() const
{
    return m_expanderButton->isChecked();
}

void UITreeItem2::onSelected(UIEventArgs* e)
{
}

void UITreeItem2::onUnselected(UIEventArgs* e)
{
}

void UITreeItem2::setContent(UIElement* value)
{
    assert(!m_headerContent);    // TODO: 多重追加対策
    m_headerContent = value;
    addChild(value);
}

void UITreeItem2::onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel)
{
    if (oldViewModel) {
        LN_NOTIMPLEMENTED();
    }

    m_model = dynamic_cast<UICollectionItemViewModel*>(newViewModel);
    if (!m_model) {
        LN_NOTIMPLEMENTED();
    }

    // TODO: model の変更通知対応
    if (m_model->getChildrenCount() > 0) {
        m_expanderButton->m_internalVisibility = UIVisibility::Visible;
    }
    else {
        m_expanderButton->m_internalVisibility = UIVisibility::Hidden;
    }

    UIElement::setContent(m_model->getValue(u""));
}

void UITreeItem2::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::MouseDownEvent) {
        if (auto* t = getTreeView()) {
            t->notifyItemClicked(this);

            if (static_cast<UIMouseEventArgs*>(e)->getClickCount() == 2) {
                // Double click
                t->onItemSubmitted(UIEventArgs::create(this, UIEvents::Submitted));
            }
        }


        e->handled = true;
        return;

    }

    UIControl::onRoutedEvent(e);
}

Size UITreeItem2::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    //for (auto& item : *m_logicalChildren) {
    //    static_pointer_cast<UITreeItem2>(item)->m_layoutingOwnerTreeView = m_layoutingOwnerTreeView;
    //    static_pointer_cast<UITreeItem2>(item)->m_layoutDepth = m_layoutDepth + 1;
    //}
    //if (layoutContext->collectingTreeItem) {
    //    layoutContext->treeView->addItemAsVisualChildren(this);
    //}


    Size size;

    // expander
    m_expanderButton->measureLayout(layoutContext, constraint);
    size.width += m_expanderButton->desiredSize().width;
    size.height = std::max(size.height, m_expanderButton->desiredSize().height);

    // depth offset
    size.width += m_finalStyle->minHeight * (m_layoutDepth);

    // header
    if (m_headerContent) {
        m_headerContent->measureLayout(layoutContext, constraint);
        size.width += m_headerContent->desiredSize().width;
        size.height = std::max(size.height, m_headerContent->desiredSize().height);
    }

    Size desiredSize = UIControl::measureOverride(layoutContext, constraint);

    return Size::max(size, desiredSize);
}

Size UITreeItem2::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    const auto finalSize = finalArea.getSize();
    UITreeView2* treeView = getTreeView();

    float headerHeight = m_finalStyle->minHeight;
    Size expanderSlotAreaSize(headerHeight, headerHeight);
    float offset = expanderSlotAreaSize.width * (m_layoutDepth);

    // expander
    Rect expanderSlot(offset, 0, expanderSlotAreaSize);
    m_expanderButton->arrangeLayout(layoutContext, expanderSlot); // TODO: actualsize 返すようにしていいかも

    Rect contentArea = Rect(expanderSlot.getRight(), 0, finalSize.width - expanderSlot.getRight(), expanderSlot.height);

    Rect contentSlotRect;
    detail::LayoutHelper::adjustAlignment(
        contentArea, m_headerContent->desiredSize(),
        treeView->m_finalStyle->horizontalContentAlignment,
        treeView->m_finalStyle->verticalContentAlignment, &contentSlotRect);

    // header
    //float headerContentHeight = 0;
    //if (m_headerContent) {
    //    m_headerContent->arrangeLayout(layoutContext, contentSlotRect);
    //    headerContentHeight = m_headerContent->actualSize().height;
    //}

    //Rect finalArea = contentSlotRect;
    {   // TODO: UIControl::arrangeOverride そのままになっている。arrangeOverride は Rect もらうようにしていいかも

        if (m_aligned3x3GridLayoutArea) {
            // padding, border を考慮した領域を計算
            Rect clientArea = detail::LayoutHelper::arrangeClientArea(this, contentSlotRect);
            // Inline 要素を arrange & 論理子要素の領域 (content area) を計算
            Rect contentArea;
            m_aligned3x3GridLayoutArea->arrange(layoutContext, m_inlineElements, clientArea, &contentArea);
            // 論理子要素を arrange
            detail::LayoutHelper::UIFrameLayout_staticArrangeChildrenArea(
                layoutContext, this,
                m_finalStyle->horizontalContentAlignment,
                m_finalStyle->verticalContentAlignment,
                m_logicalChildren, contentArea);
            //UIFrameLayout2::staticArrangeChildrenArea(this, m_logicalChildren, contentArea);

        }
        else {
            UIFrameLayout2::staticArrangeLogicalChildren(layoutContext, this, contentSlotRect);
        }
    }

    return finalSize;
}

void UITreeItem2::expander_Checked(UIEventArgs* e)
{
    attemptCreateChildItemsInstance();

    for (auto& child : m_childItems) {
        child->m_internalVisibility = UIVisibility::Visible;
    }
}

void UITreeItem2::expander_Unchecked(UIEventArgs* e)
{
    for (auto& child : m_childItems) {
        child->m_internalVisibility = UIVisibility::Collapsed;
    }
}

void UITreeItem2::attemptCreateChildItemsInstance()
{
    int count = m_model->getChildrenCount();
    if (m_childItems.size() != count) {
        //removeAllChildren();

        // TODO: 完全再構築じゃなくて、Model に新規で追加されたものだけ追加したい。
        // そうしないとメモリ効率の問題もあるし、Expanded プロパティにバインドしていないときの展開状態の保存とかできない。
        for (auto& item : m_childItems) {
            removeVisualChild(item);
        }
        m_childItems.clear();


        UITreeView2* treeView = getTreeView();
        if (LN_ENSURE(treeView)) return;

        for (int i = 0; i < count; i++) {
            auto childModel = m_model->getItem(i);

            auto item = treeView->generateTreeItem(childModel);

            m_childItems.add(item);
            item->m_parentItem = this;
            treeView->addItemAsLogicalChildren(item);
        }

        dirtyVisualItemCount();
    }
}

UITreeView2* UITreeItem2::getTreeView()
{
    // TODO: 直接の親要素を TreeView ということにしたので、再帰は不要
    // TODO: dynamic_cast じゃなくて flags 確認で。
    if (auto* t = dynamic_cast<UITreeView2*>(m_logicalParent))
        return t;
    else if (auto* t = dynamic_cast<UITreeItem2*>(m_logicalParent))
        return t->getTreeView();
    else
        return nullptr;
}

void UITreeItem2::dirtyVisualItemCount()
{
    getTreeView()->m_dirtyItemVisualTree = true;
}

void UITreeItem2::traverseTreeItems(IVisitor* visitor)
{
    //if (isRenderVisible()) {
        visitor->visit(this);
        if (isExpanded()) {
            for (auto& child : m_childItems) {
                child->traverseTreeItems(visitor);
            }
        }
    //}
}

void UITreeItem2::setSelectedInternal(bool selected)
{
    if (m_isSelected != selected) {
        m_isSelected = selected;

        if (m_isSelected) {
            onSelected(UIEventArgs::create(this, UIEvents::Selected));
            getVisualStateManager()->gotoState(UIVisualStates::Selected);
        }
        else {
            onUnselected(UIEventArgs::create(this, UIEvents::Selected));
            getVisualStateManager()->gotoState(UIVisualStates::Unselected);
        }
    }
}

//==============================================================================
// UITreeView2

/*
    [2020/7/3] UITreeItem2 と setContent()

    いまのところ、setContent() は WPF でいうところの Header を set するもの。
    ただ addChild で追加できる子要素と別管理になっているのでちょっとまぎらわしい。
    Windows とかと同じように、LogicalChildren を配置する ClientArea を、Header 扱いする、でいいと思う。
    ExpandButton は VisualParent として　ClientArea の外におく。


    [2020/6/23] ViewModel はオプションの方向に再設計してみる

    そもそもなんで ViewModel を使いたくない？
    - Item ごとに ViewModel クラスが必要になるため。ListView ならルートにひとつあれば足りるが、Tree はそうもいかない。
    - Item は ViewModel でなければならないか、ViewModel でラップしなければならないためかなり煩わしい。
        - いずれにしても ViewModel の派生を実装する必要がある。
    
    ViewModel が無いと不利な点は？
    - 変更通知ができなくなる。たとえばシーンエディタのプロパティビューで名前を変更したら、Hieralchy へ反映したいとき。
        - この場合は WorldObjectViewModel を作って、TreeItem と PropertyView に bind することになる。

    今の setViewModel を、UIViewModel もらうんじゃなくて Object をもらうようにしてしまえば？
    - Binding 使うときに煩わしいか。継承なんてリッチな機能の無い言語もある。
    - HSP でカスタム TreeView を作るとしたら？
        - setViewModel は intptr とかにしてほしい。整数でデータを表現できるようにしてほしい。
        - 一緒にデータ数も渡したいかも。コールバックでアイテム数もらうのはつらい。
        - GenerateItemContent コールバックで TreeItem の handle をもらう。これに データを示す整数値と、子要素数を set する。
        - 必要に応じてこの handle に TextBlock や Button を add.
    
    [2020/6/30] TreeView は ViewModel 在り無しどちらを推奨する？

    ViewModel 在り: 動的な、データ "編集"
    ViewModel 無し: 静的な、データ "閲覧"

    閲覧は周辺の他の View 含めて完全に readonly な場合のみを想定している。
    例えば、Treeは静的でもプロパティビューから名前を変更したら TreeItem を変えたい、という場合は ViewModel を使う。


*/

UITreeView2::UITreeView2()
    : m_scrollViewHelper()
{
}

bool UITreeView2::init()
{
    if (!UIControl::init()) return false;

    auto layout = makeObject<UIStackLayout>();
    layout->setOrientation(UILayoutOrientation::Vertical);
    addVisualChild(layout);
    m_itemsHostLayout = layout;

    //m_scrollBarV = makeObject<UIScrollBar>();
    ////m_scrollBarV->setBackgroundColor(Color::Red);
    //m_scrollBarV->setWidth(10);
    //addVisualChild(m_scrollBarV);

    m_scrollViewHelper = makeRef<UIScrollViewHelper>(this);
    m_scrollViewHelper->setScrollTarget(m_itemsHostLayout);
    m_scrollViewHelper->setVScrollbarVisible(true);


    return true;
}

Ref<EventConnection> UITreeView2::connectOnChecked(Ref<UIGeneralEventHandler> handler)
{
    return m_onItemSubmitted.connect(handler);
}

void UITreeView2::setGenerateTreeItemHandler(Ref<UIGenerateTreeItemHandler> handler)
{
    m_onGenerateTreeItem = handler;
}

void UITreeView2::onSelectionChanged(UISelectionChangedEventArgs* e)
{
}

void UITreeView2::onItemSubmitted(UIEventArgs* e)
{
    m_onItemSubmitted.raise(e);
}

void UITreeView2::onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel)
{
    if (oldViewModel) {
        LN_NOTIMPLEMENTED();
    }

    m_model = dynamic_cast<UICollectionViewModel*>(newViewModel);
    if (!m_model) {
        LN_NOTIMPLEMENTED();
    }

    rebuildTreeFromViewModel();
}

void UITreeView2::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
    // Item の数が変化していたらツリーを再捜査して LayoutPanel を再構築する。
    // 何も対策しないと Expand をうけて add された Item がリストの一番下についかされてしまうので、
    // 親子関係を考慮して順序正しく構築するために必要。もうちょっと高速化する方法ありそうだが…。
    if (m_dirtyItemVisualTree) {
        class LocalVisitor : public UITreeItem2::IVisitor
        {
        public:
            UITreeView2* self;
            void visit(UITreeItem2* item) override
            {
                if (item->m_parentItem)
                    item->m_layoutDepth = item->m_parentItem->m_layoutDepth + 1;
                else
                    item->m_layoutDepth = 0;
                self->m_itemsHostLayout->addVisualChild(item);
            }
        };
        LocalVisitor v;
        v.self = this;
        m_itemsHostLayout->removeAllVisualChild();

        for (UITreeItem2* item : m_rootItems) {
            item->traverseTreeItems(&v);
        }

        m_dirtyItemVisualTree = false;
    }

    UIControl::onUpdateStyle(styleContext, finalStyle);
}

Size UITreeView2::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    //m_scrollBarV->measureLayout(layoutContext, constraint);
    Size scrollBarDesiredSize = m_scrollViewHelper->measure(layoutContext, constraint);

    //auto old_collectingTreeItem = layoutContext->collectingTreeItem;
    //auto old_treeView = layoutContext->treeView;
    //if (m_dirtyItemVisualTree) {
    //    layoutContext->collectingTreeItem = true;
    //    layoutContext->treeView = this;
    //    m_itemsHostLayout->removeAllVisualChild();
    //    m_dirtyItemVisualTree = false;
    //}

    //for (auto& item : *m_logicalChildren) {
    //    static_pointer_cast<UITreeItem2>(item)->m_layoutingOwnerTreeView = this;
    //    static_pointer_cast<UITreeItem2>(item)->m_layoutDepth = 0;
    //}


    m_itemsHostLayout->measureLayout(layoutContext, constraint);
    Size layoutSize = m_itemsHostLayout->desiredSize();
    Size localSize = UIElement::measureOverride(layoutContext, constraint);

    // Restore context state.
    //layoutContext->collectingTreeItem = old_collectingTreeItem;
    //layoutContext->treeView = old_treeView;

    Size desiredSize = Size::max(layoutSize, localSize);
    desiredSize.width += scrollBarDesiredSize.width;
    desiredSize.height += scrollBarDesiredSize.height;
    return desiredSize;
}

Size UITreeView2::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    const auto finalSize = finalArea.getSize();
    //Size scrollHSize = m_scrollBarV->desiredSize();
    //Rect scrollHArea(finalSize.width - scrollHSize.width, 0, scrollHSize.width, finalSize.height);
    //m_scrollBarV->arrangeLayout(layoutContext, scrollHArea);
    Rect clientArea = m_scrollViewHelper->calculateClientArea(finalSize);

    Rect contentSlotRect = detail::LayoutHelper::makePaddingRect(this, clientArea.getSize());
    contentSlotRect.x += clientArea.x;
    contentSlotRect.y += clientArea.y;

    m_itemsHostLayout->arrangeLayout(layoutContext, contentSlotRect);

    m_scrollViewHelper->arrange(layoutContext, finalSize);

    return finalSize;
}

void UITreeView2::onRoutedEvent(UIEventArgs* e)
{
    UIControl::onRoutedEvent(e);
    m_scrollViewHelper->handleRoutedEvent(e);
}

void UITreeView2::rebuildTreeFromViewModel()
{
    LN_CHECK(m_model);
    LN_CHECK(m_rootItems.isEmpty());  // TODO: 再構築は未実装
    m_rootItems.clear();

    int count = m_model->getItemCount();
    for (int i = 0; i < count; i++) {
        auto childModel = m_model->getItem(i);

        auto item = generateTreeItem(childModel);

        addItemAsLogicalChildren(item);
        m_rootItems.add(item);
    }
}

void UITreeView2::addItemAsLogicalChildren(UITreeItem2* item)
{
    LN_CHECK(item);
    m_logicalChildren->add(item);
    item->m_logicalParent = this;
    m_itemsHostLayout->addVisualChild(item);
}

//void UITreeView2::addItemAsVisualChildren(UITreeItem2* item)
//{
//    LN_CHECK(item);
//    m_itemsHostLayout->addVisualChild(item);
//}

void UITreeView2::clearSelection()
{
    for (auto& item : m_selectedItems) {
        item->setSelectedInternal(false);
    }
    m_selectedItems.clear();
}

void UITreeView2::selectItemExclusive(UITreeItem2* item)
{
    clearSelection();

    m_selectedItems.add(item);
    item->setSelectedInternal(true);

    onSelectionChanged(UISelectionChangedEventArgs::create(this, UIEvents::SelectionChanged));
}

void UITreeView2::notifyItemClicked(UITreeItem2* item)
{
    selectItemExclusive(item);
}

Ref<UITreeItem2> UITreeView2::generateTreeItem(UICollectionItemViewModel* viewModel)
{
    auto item = makeObject<UITreeItem2>();
    item->setViewModel(viewModel);

    if (m_onGenerateTreeItem) {
        m_onGenerateTreeItem->call(item);
    }

    return item;
}

} // namespace ln

