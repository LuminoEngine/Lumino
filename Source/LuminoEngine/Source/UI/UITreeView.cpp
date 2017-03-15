
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
LN_UI_TYPEINFO_IMPLEMENT(UITreeViewItem, UIControl)

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
void UITreeViewItem::Initialize(ln::detail::UIManager* manager)
{
	UIControl::Initialize(manager);
	SetHContentAlignment(HAlignment::Left);
	SetHAlignment(HAlignment::Stretch);
	GoToVisualState(NormalState);
}

//------------------------------------------------------------------------------
void UITreeViewItem::SetHeader(UIElement* header)
{
	RemoveVisualChild(m_header);

	m_header = header;
	m_header->SetBackground(Brush::Black);
	m_header->SetHeight(16);	// TODO:

	if (m_header != nullptr)
		AddVisualChild(m_header);
}

//------------------------------------------------------------------------------
Size UITreeViewItem::MeasureOverride(const Size& constraint)
{
	Size desiredSize(16, 0);	// TODO: Branch の余白は後で考える http://doc.qt.io/qt-4.8/stylesheet-examples.html#customizing-qtreeview

	// Measure Header
	m_header->MeasureLayout(constraint);
	Size headerSize = m_header->GetDesiredSize();

	// Measure Items
	UILayoutPanel* itemsPanel = GetLayoutPanel();
	itemsPanel->MeasureLayout(constraint);
	Size panelSize = itemsPanel->GetDesiredSize();

	// 下方向に結合する
	desiredSize.height += headerSize.height;
	desiredSize.height += panelSize.height;
	desiredSize.width = std::max(headerSize.width, panelSize.width);

	Size thisSize = ln::detail::LayoutHelper::MeasureElement(this, constraint);

	return Size::Max(desiredSize, thisSize);

	// ※GetLayoutPanel() で得られる UILayoutPanel の Measure をここで行うので 
}

//------------------------------------------------------------------------------
Size UITreeViewItem::ArrangeOverride(const Size& finalSize)
{
	Size expanderSize(16, 16);	// TODO: Branch の余白は後で考える

	// Header
	Size headerSize = m_header->GetDesiredSize();
	RectF headerRect(expanderSize.width, 0, finalSize.width - expanderSize.width, std::max(expanderSize.height, headerSize.height));
	m_header->ArrangeLayout(headerRect);
	
	// Items
	RectF itemsRect(expanderSize.width, headerRect.height, finalSize.width - expanderSize.width, finalSize.height - headerRect.height);
	GetLayoutPanel()->ArrangeLayout(itemsRect);

	return finalSize;
}


//==============================================================================
// UITreeView
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UITreeView, UIControl)

const String UITreeView::NormalState = _T("Normal");

//------------------------------------------------------------------------------
UITreeViewPtr UITreeView::Create()
{
	auto ptr = UITreeViewPtr::MakeRef();
	ptr->Initialize(ln::detail::UIManager::GetInstance());
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
void UITreeView::Initialize(ln::detail::UIManager* manager)
{
	UIControl::Initialize(manager);
	SetHContentAlignment(HAlignment::Stretch);

	auto panel = RefPtr<UIStackPanel>::MakeRef();
	panel->Initialize(manager);
	panel->SetHAlignment(HAlignment::Stretch);
	panel->SetVAlignment(VAlignment::Stretch);
	SetLayoutPanel(panel);
	GoToVisualState(NormalState);
}

////------------------------------------------------------------------------------
//UITreeViewItemPtr UITreeView::AddTextItem(const String& text)
//{
//	auto textBlock = RefPtr<UITextBlock>::MakeRef();
//	textBlock->Initialize(GetManager());
//	textBlock->SetText(text);
//	return AddItem(textBlock);
//}
//
////------------------------------------------------------------------------------
//UITreeViewItemPtr UITreeView::AddItem(UIElement* item)
//{
//	LN_CHECK_ARG(item != nullptr);
//
//	// 受け取った item を UITreeViewItem でラップして、UITreeViewItem をリストに入れる
//	auto listItem = RefPtr<UITreeViewItem>::MakeRef();
//	listItem->Initialize(GetManager());
//	listItem->SetContent(item);
//	GetItems()->Add(listItem);
//	return listItem;
//}

} // namespace tr
LN_NAMESPACE_END
