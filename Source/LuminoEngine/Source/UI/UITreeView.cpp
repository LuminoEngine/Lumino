
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UITreeView.h>
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// UITreeViewItem
//==============================================================================
	LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITreeViewItem, UIControl)

//------------------------------------------------------------------------------
UITreeViewItem::UITreeViewItem()
	: m_header(nullptr)
{
}

//------------------------------------------------------------------------------
UITreeViewItem::~UITreeViewItem()
{
}

//------------------------------------------------------------------------------
void UITreeViewItem::initialize()
{
	UIControl::initialize();
	setHContentAlignment(HAlignment::Left);
	setHAlignment(HAlignment::Stretch);
	goToVisualState(UIVisualStates::NormalState);
}

//------------------------------------------------------------------------------
void UITreeViewItem::setHeader(UIElement* header)
{
	removeVisualChild(m_header);

	m_header = header;
	m_header->setBackground(Brush::Black);
	m_header->setHeight(16);	// TODO:

	if (m_header != nullptr)
		addVisualChild(m_header);
}

//------------------------------------------------------------------------------
Size UITreeViewItem::measureOverride(const Size& constraint)
{
	Size desiredSize(16, 0);	// TODO: Branch の余白は後で考える http://doc.qt.io/qt-4.8/stylesheet-examples.html#customizing-qtreeview

	// measure Header
	m_header->measureLayout(constraint);
	Size headerSize = m_header->getDesiredSize();

	// measure Items
	UILayoutPanel* itemsPanel = getLayoutPanel();
	itemsPanel->measureLayout(constraint);
	Size panelSize = itemsPanel->getDesiredSize();

	// 下方向に結合する
	desiredSize.height += headerSize.height;
	desiredSize.height += panelSize.height;
	desiredSize.width = std::max(headerSize.width, panelSize.width);

	Size thisSize = ln::detail::LayoutHelper::measureElement(this, constraint);

	return Size::max(desiredSize, thisSize);

	// ※getLayoutPanel() で得られる UILayoutPanel の measure をここで行うので 
}

//------------------------------------------------------------------------------
Size UITreeViewItem::arrangeOverride(const Size& finalSize)
{
	Size expanderSize(16, 16);	// TODO: Branch の余白は後で考える

	// Header
	Size headerSize = m_header->getDesiredSize();
	Rect headerRect(expanderSize.width, 0, finalSize.width - expanderSize.width, std::max(expanderSize.height, headerSize.height));
	m_header->arrangeLayout(headerRect);
	
	// Items
	Rect itemsRect(expanderSize.width, headerRect.height, finalSize.width - expanderSize.width, finalSize.height - headerRect.height);
	getLayoutPanel()->arrangeLayout(itemsRect);

	return finalSize;
}


//==============================================================================
// UITreeView
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITreeView, UIControl)

//------------------------------------------------------------------------------
UITreeViewPtr UITreeView::create()
{
	auto ptr = UITreeViewPtr::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UITreeView::UITreeView()
{
}

//------------------------------------------------------------------------------
UITreeView::~UITreeView()
{
}

//------------------------------------------------------------------------------
void UITreeView::initialize()
{
	UIControl::initialize();
	setHContentAlignment(HAlignment::Stretch);

	auto panel = RefPtr<UIStackPanel>::makeRef();
	panel->initialize();
	panel->setHAlignment(HAlignment::Stretch);
	panel->setVAlignment(VAlignment::Stretch);
	setLayoutPanel(panel);
	goToVisualState(UIVisualStates::NormalState);
}

////------------------------------------------------------------------------------
//UITreeViewItemPtr UITreeView::addTextItem(const String& text)
//{
//	auto textBlock = RefPtr<UITextBlock>::MakeRef();
//	textBlock->initialize(getManager());
//	textBlock->SetText(text);
//	return AddItem(textBlock);
//}
//
////------------------------------------------------------------------------------
//UITreeViewItemPtr UITreeView::AddItem(UIElement* item)
//{
//
//	// 受け取った item を UITreeViewItem でラップして、UITreeViewItem をリストに入れる
//	auto listItem = RefPtr<UITreeViewItem>::MakeRef();
//	listItem->initialize(getManager());
//	listItem->setContent(item);
//	getItems()->Add(listItem);
//	return listItem;
//}

} // namespace tr
LN_NAMESPACE_END
