
#include "Internal.hpp"
#include <LuminoEngine/UI/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>

namespace ln {

//==============================================================================
// UICollectionItem

UICollectionItem::UICollectionItem()
{
}

void UICollectionItem::init()
{
	UIControl::init();
}

void UICollectionItem::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent) {
		printf("");
	}

	UIControl::onRoutedEvent(e);
}

//==============================================================================
// UIItemsControl
//	ただ複数の Item を追加できる、っていうだけだと、UIControl と変わらない。
//	WPF でいう Selector の機能も兼ねる。

UIItemsControl::UIItemsControl()
{
}

void UIItemsControl::init()
{
	UIControl::init();
}

void UIItemsControl::addSelectionTarget(UICollectionItem* item)
{
	if (LN_REQUIRE(item)) return;
	m_selectionTargets.add(item);
}

//void UIItemsControl::onRoutedEvent(UIEventArgs* e)
//{
//	if (e->type() == UIEvents::MouseDownEvent) {
//		
//	}
//
//	UIControl::onRoutedEvent(e);
//}








//==============================================================================
// UIItemElement

UIItemElement::UIItemElement()
{
}

void UIItemElement::init()
{
	UIElement::init();
}

//==============================================================================
// UIItemContainerElement

UIItemContainerElement::UIItemContainerElement()
{
}

void UIItemContainerElement::init()
{
	UIScrollViewer::init();
}

//==============================================================================
// UITreeViewItem

UITreeViewItem::UITreeViewItem()
    : m_ownerTreeView(nullptr)
{
}

void UITreeViewItem::init()
{
	UIElement::init();

	m_expanderButton = makeObject<UIToggleButton>();

	// TODO: style
	m_expanderButton->setWidth(16);
	m_expanderButton->setHeight(16);
	setWidth(100);
	setHeight(20);
	setBackgroundColor(Color::AliceBlue);

	m_expanderButton->addClass(u"UIButton-TreeExpander");
    m_expanderButton->connectOnChecked(bind(this, &UITreeViewItem::expander_Checked));
    m_expanderButton->connectOnUnchecked(bind(this, &UITreeViewItem::expander_Unchecked));

	addVisualChild(m_expanderButton);

    auto layout = makeObject<UIStackLayout2>();
    layout->setOrientation(Orientation::Vertical);
    m_itemsLayout = layout;
}

void UITreeViewItem::setContent(UIElement* value)
{
    assert(!m_headerContent);    // TODO
    m_headerContent = value;
    addVisualChild(m_headerContent);
}

void UITreeViewItem::addChild(UITreeViewItem* item)
{
    if (LN_REQUIRE(item)) return;
    m_items.add(item);
    addVisualChild(item);   // TODO: 多重追加対策
}

void UITreeViewItem::onExpanded()
{
}

void UITreeViewItem::onCollapsed()
{
}

Size UITreeViewItem::measureOverride(const Size& constraint)
{
    struct ElementList : public IUIElementList {
        List<Ref<UITreeViewItem>>* list;
        virtual int getElementCount() const { return list->size(); }
        virtual UIElement* getElement(int i) const { return list->at(i); }
    } list;
    list.list = &m_items;

    Size size;

    // expander
    m_expanderButton->measureLayout(constraint);
    size.width += m_expanderButton->desiredSize().width;
    size.height = std::max(size.height, m_expanderButton->desiredSize().height);

    // header
    if (m_headerContent) {
        m_headerContent->measureLayout(constraint);
        size.width += m_headerContent->desiredSize().width;
        size.height = std::max(size.height, m_headerContent->desiredSize().height);
    }

    // children
    m_itemsLayout->measureLayout(&list, constraint);
    size.width = std::max(size.width, m_itemsLayout->desiredSize().width);
    size.height += m_itemsLayout->desiredSize().height;

    return size;
}

Size UITreeViewItem::arrangeOverride(const Size& finalSize)
{
    struct ElementList : public IUIElementList {
        List<Ref<UITreeViewItem>>* list;
        virtual int getElementCount() const { return list->size(); }
        virtual UIElement* getElement(int i) const { return list->at(i); }
    } list;
    list.list = &m_items;

    Rect area = Rect(0, 0, finalSize);

    // expander
    m_expanderButton->arrangeLayout(area); // TODO: actualsize 返すようにしていいかも

    // header
    if (m_headerContent) {
        area.x += m_expanderButton->m_actualSize.width;
        area.width -= m_expanderButton->m_actualSize.width;
        m_headerContent->arrangeLayout(area);
    }

    // children
    area.y = std::max(m_expanderButton->m_actualSize.height, m_headerContent->m_actualSize.height);
    m_itemsLayout->arrangeLayout(&list, area);

    return finalSize;
}

void UITreeViewItem::expander_Checked(UIEventArgs* e)
{
    onExpanded();
}

void UITreeViewItem::expander_Unchecked(UIEventArgs* e)
{
    onCollapsed();
}

//==============================================================================
// UITreeView

UITreeView::UITreeView()
{
}

void UITreeView::init()
{
	UIItemContainerElement::init();

    auto layout = makeObject<UIStackLayout2>();
    layout->setOrientation(Orientation::Vertical);
    setLayoutPanel(layout);
}

void UITreeView::setModel(UIItemsViewModel* model)
{
	m_model = model;
}

void UITreeView::addItem(UITreeViewItem* item)
{
    if (LN_REQUIRE(item)) return;
    item->m_ownerTreeView = this;
    LN_NOTIMPLEMENTED();
}

} // namespace ln

