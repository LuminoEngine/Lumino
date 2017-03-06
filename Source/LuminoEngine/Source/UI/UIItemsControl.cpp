
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
	UIElement::Initialize(manager);
	m_items = RefPtr<UIElementCollection>::MakeRef(this);
}

//------------------------------------------------------------------------------
UIElementCollection* UIItemsControl::GetItems() const
{
	return m_items;
}

//------------------------------------------------------------------------------
void UIItemsControl::SetItemsHostPanel(UILayoutPanel* panel)
{
	LN_CHECK_ARG(panel != nullptr);
	//SetVisualTreeRoot(panel);
	LN_NOTIMPLEMENTED();
	m_itemsHostPanel = panel;
}

//------------------------------------------------------------------------------
//int UIItemsControl::GetVisualChildrenCount() const
//{
//	return 1;	// ItemsHostPanel 1‚Â•ª
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
			LN_NOTIMPLEMENTED();
			break;
		case tr::NotifyCollectionChangedAction::Replace:
			LN_NOTIMPLEMENTED();
			break;
		case tr::NotifyCollectionChangedAction::Reset:
			LN_NOTIMPLEMENTED();
			break;
		default:
			break;
	}
}

LN_NAMESPACE_END
