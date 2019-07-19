
#include "Internal.hpp"
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>

namespace ln {

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
// UIListBox

UIListBox::UIListBox()
{
}

void UIListBox::init()
{
	UIItemContainerElement::init();
}

//==============================================================================
// UITreeViewItem

UITreeViewItem::UITreeViewItem()
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

	addVisualChild(m_expanderButton);
}

Size UITreeViewItem::measureOverride(const Size& constraint)
{
	m_expanderButton->measureLayout(constraint);
	return UIElement::measureOverride(constraint);
}

Size UITreeViewItem::arrangeOverride(const Size& finalSize)
{
	m_expanderButton->arrangeLayout(Rect(0, 0, finalSize));
	return UIElement::arrangeOverride(finalSize);
}

//==============================================================================
// UITreeView

UITreeView::UITreeView()
{
}

void UITreeView::init()
{
	UIItemContainerElement::init();
}

void UITreeView::setModel(UIItemsViewModel* model)
{
	m_model = model;
}

} // namespace ln

