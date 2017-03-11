
#include "Internal.h"
#include <Lumino/UI/UIItemsControl.h>
#include <Lumino/UI/UILayoutPanel.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIItemsControl
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIItemsControl, UIElement);

//------------------------------------------------------------------------------
UIItemsControl::UIItemsControl()
{

}

//------------------------------------------------------------------------------
UIItemsControl::~UIItemsControl()
{

}

//------------------------------------------------------------------------------
void UIItemsControl::Initialize(detail::UIManager* manager)
{
	UIControl::Initialize(manager);
	m_items = RefPtr<UIElementCollection>::MakeRef(this);

	auto panel = NewObject<UILayoutPanel>(manager);
	SetLayoutPanel(panel);


	// TODO:
	//SetVContentAlignment(VAlignment::Stretch);
	//SetHContentAlignment(HAlignment::Stretch);
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
}

//------------------------------------------------------------------------------
//int UIItemsControl::GetVisualChildrenCount() const
//{
//	return 1;	// ItemsHostPanel 1つ分
//}
//
////------------------------------------------------------------------------------
//UIElement* UIItemsControl::GetVisualChild(int index) const
//{
//	return m_itemsHostPanel;
//}

////------------------------------------------------------------------------------
//Size UIItemsControl::MeasureOverride(const Size& constraint)
//{
//	Size desiredSize = UIElement::MeasureOverride(constraint);
//
//	m_itemsHostPanel->MeasureLayout(constraint);
//	const Size& childDesiredSize = m_itemsHostPanel->GetDesiredSize();
//
//	desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
//	desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
//
//	return desiredSize;
//}
//
////------------------------------------------------------------------------------
//Size UIItemsControl::ArrangeOverride(const Size& finalSize)
//{
//	Size childFinal(0, 0, finalSize);
//
//	Size childDesiredSize = m_itemsHostPanel->GetDesiredSize();
//	childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
//	childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
//	m_itemsHostPanel->ArrangeLayout(RectF(0, 0, childDesiredSize));
//
//	return finalSize;
//}

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

LN_NAMESPACE_END
