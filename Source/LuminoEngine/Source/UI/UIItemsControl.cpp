
#include "Internal.h"
#include <Lumino/UI/UIItemsControl.h>
#include <Lumino/UI/UILayoutPanel.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIItemsControl
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIItemsControl, UIControl)

UIItemsControl::UIItemsControl()
{
}

UIItemsControl::~UIItemsControl()
{
}

void UIItemsControl::initialize()
{
	UIControl::initialize();

	setHContentAlignment(HAlignment::Stretch);

	auto panel = newObject<UIStackPanel>();
	panel->setHAlignment(HAlignment::Stretch);
	panel->setVAlignment(VAlignment::Stretch);
	setLayoutPanel(panel);
}

void UIItemsControl::addItem(UIElement* item)
{
	addChild(item);
}


//==============================================================================
// UIItemsControl
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIHeaderedItemsControl, UIItemsControl)

UIHeaderedItemsControl::UIHeaderedItemsControl()
	: m_headerContainer(nullptr)
	, m_headerContent(nullptr)
{
}

UIHeaderedItemsControl::~UIHeaderedItemsControl()
{
}

void UIHeaderedItemsControl::initialize()
{
	UIItemsControl::initialize();

	m_headerContainer = newObject<UIControl>();
	m_headerContainer->setBackground(Brush::Green);	// TODO:
	m_headerContainer->setHeight(16);				// TODO:
	m_headerContainer->setSize(Size(16, 16));		// TODO:
	addVisualChild(m_headerContainer);
}

void UIHeaderedItemsControl::setHeader(UIElement* header)
{
	m_headerContainer->removeVisualChild(m_headerContent);

	m_headerContent = header;
	m_headerContent->setBackground(Brush::Red);	// TODO:
	m_headerContent->setHeight(16);	// TODO:

	if (m_headerContent != nullptr)
	{
		m_headerContainer->addVisualChild(m_headerContent);
	}
}

Size UIHeaderedItemsControl::measureOverride(const Size& constraint)
{
	Size desiredSize(0, 0);

	// ヘッダの領域を計測する
	m_headerContainer->measureLayout(constraint);
	Size headerSize = m_headerContainer->getDesiredSize();

	// 子アイテムの領域を計測する
	UILayoutPanel* itemsPanel = getLayoutPanel();
	itemsPanel->measureLayout(constraint);
	Size panelSize = itemsPanel->getDesiredSize();

	// 下方向に結合する
	desiredSize.height += headerSize.height;
	desiredSize.height += panelSize.height;
	desiredSize.width = std::max(headerSize.width, panelSize.width);

	Size thisSize = ln::detail::LayoutHelper::measureElement(this, constraint);

	return Size::max(desiredSize, thisSize);
}

Size UIHeaderedItemsControl::arrangeOverride(const Size& finalSize)
{
	// Header
	Size headerSize = m_headerContainer->getDesiredSize();
	Rect headerRect(0, 0, finalSize.width, headerSize.height);
	m_headerContainer->arrangeLayout(headerRect);

	// Items
	Rect itemsRect(0, headerRect.height, finalSize.width, finalSize.height - headerRect.height);
	getLayoutPanel()->arrangeLayout(itemsRect);

	return finalSize;
}


#if 0
//==============================================================================
// UIItemsControl
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIItemsControl, UIElement);

//------------------------------------------------------------------------------
UIItemsControl::UIItemsControl()
{

}

//------------------------------------------------------------------------------
UIItemsControl::~UIItemsControl()
{

}

//------------------------------------------------------------------------------
void UIItemsControl::initialize(detail::UIManager* manager)
{
	UIControl::initialize(manager);
	m_items = Ref<UIElementCollection>::makeRef(this);

	auto panel = NewObject<UILayoutPanel>(manager);
	SetLayoutPanel(panel);


	// TODO:
	//setVContentAlignment(VAlignment::Stretch);
	//setHContentAlignment(HAlignment::Stretch);
}

//------------------------------------------------------------------------------
UIElementCollection* UIItemsControl::GetItems() const
{
	return m_items;
}

//------------------------------------------------------------------------------
void UIItemsControl::AddChild(UIElement* element)
{
	m_items->Add(element);
}

//------------------------------------------------------------------------------
void UIItemsControl::RemoveChild(UIElement* element)
{
	m_items->Remove(element);
}

//------------------------------------------------------------------------------
void UIItemsControl::SetLayoutPanel(UILayoutPanel* panel)
{
	UILayoutPanel* oldPanel = m_itemsHostPanel;
	UILayoutPanel* newPanel = panel;

	// 既に持っていれば取り除いておく
	if (m_itemsHostPanel != nullptr && m_itemsHostPanel != panel)
	{
		RemoveVisualChild(m_itemsHostPanel);
		m_itemsHostPanel = nullptr;
	}

	// 新しく保持する
	if (panel != nullptr)
	{
		AddVisualChild(panel);
		m_itemsHostPanel = panel;
	}

	// 変更通知
	if (oldPanel != newPanel)
	{
		OnLayoutPanelChanged(newPanel);
	}
}

//------------------------------------------------------------------------------
//int UIItemsControl::getVisualChildrenCount() const
//{
//	return 1;	// ItemsHostPanel 1つ分
//}
//
////------------------------------------------------------------------------------
//UIElement* UIItemsControl::getVisualChild(int index) const
//{
//	return m_itemsHostPanel;
//}

////------------------------------------------------------------------------------
//Size UIItemsControl::measureOverride(const Size& constraint)
//{
//	Size desiredSize = UIElement::measureOverride(constraint);
//
//	m_itemsHostPanel->measureLayout(constraint);
//	const Size& childDesiredSize = m_itemsHostPanel->getDesiredSize();
//
//	desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
//	desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
//
//	return desiredSize;
//}
//
////------------------------------------------------------------------------------
//Size UIItemsControl::arrangeOverride(const Size& finalSize)
//{
//	Size childFinal(0, 0, finalSize);
//
//	Size childDesiredSize = m_itemsHostPanel->getDesiredSize();
//	childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
//	childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
//	m_itemsHostPanel->arrangeLayout(RectF(0, 0, childDesiredSize));
//
//	return finalSize;
//}


//------------------------------------------------------------------------------
void UIItemsControl::OnLayoutPanelChanged(UILayoutPanel* newPanel)
{
}

//------------------------------------------------------------------------------
void UIItemsControl::OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e)
{
	switch (e.action)
	{
		case tr::NotifyCollectionChangedAction::Add:
			LN_CHECK_STATE(e.newItems.GetCount() == 1);	// TODO
			m_itemsHostPanel->GetChildren()->Insert(e.newStartingIndex, e.newItems.GetAt(0));
			break;
		case tr::NotifyCollectionChangedAction::Move:
			LN_NOTIMPLEMENTED();
			break;
		case tr::NotifyCollectionChangedAction::Remove:
			m_itemsHostPanel->GetChildren()->RemoveAt(e.oldStartingIndex);
			break;
		case tr::NotifyCollectionChangedAction::Replace:
			LN_NOTIMPLEMENTED();
			break;
		case tr::NotifyCollectionChangedAction::Reset:
			m_itemsHostPanel->GetChildren()->Clear();
			break;
		default:
			break;
	}
}
#endif

LN_NAMESPACE_END
