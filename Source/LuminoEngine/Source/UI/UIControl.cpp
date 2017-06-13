
#include "Internal.h"
#include <Lumino/UI/UIControl.h>
#include <Lumino/UI/UILayoutPanel.h>
#include "LayoutImpl.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIControl
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIControl, UIElement);
LN_TR_PROPERTY_IMPLEMENT(UIControl, HAlignment, HContentAlignment, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIControl, VAlignment, VContentAlignment, tr::PropertyMetadata());

//------------------------------------------------------------------------------
UIControl::UIControl()
	//: m_visualTreeRoot(nullptr)
{
}

//------------------------------------------------------------------------------
UIControl::~UIControl()
{
	//LN_SAFE_RELEASE(m_visualTreeRoot);
}

//------------------------------------------------------------------------------
void UIControl::initialize()
{
	UIElement::initialize();
	SetFocusable(true);

	auto* vsm = GetVisualStateManager();
	vsm->RegisterVisualState(UIVisualStates::CommonGroup, UIVisualStates::NormalState);
	vsm->RegisterVisualState(UIVisualStates::CommonGroup, UIVisualStates::MouseOverState);
	vsm->RegisterVisualState(UIVisualStates::CommonGroup, UIVisualStates::PressedState);
	vsm->RegisterVisualState(UIVisualStates::CommonGroup, UIVisualStates::DisabledState);
	vsm->RegisterVisualState(UIVisualStates::FocusGroup, UIVisualStates::UnfocusedState);
	vsm->RegisterVisualState(UIVisualStates::FocusGroup, UIVisualStates::FocusedState);
	GoToVisualState(UIVisualStates::NormalState);

	HContentAlignment = HAlignment::Stretch;
	VContentAlignment = VAlignment::Stretch;


	m_items = RefPtr<UIElementCollection>::makeRef(this);
	auto panel = newObject<UIAbsoluteLayout>();
	SetLayoutPanel(panel);
}

//------------------------------------------------------------------------------
UIElementCollection* UIControl::GetItems() const
{
	return m_items;
}

//------------------------------------------------------------------------------
void UIControl::AddChild(UIElement* element)
{
	m_items->add(element);
	element->SetLogicalParent(this);
}

//------------------------------------------------------------------------------
void UIControl::RemoveChild(UIElement* element)
{
	element->SetLogicalParent(nullptr);
	m_items->remove(element);
}

//------------------------------------------------------------------------------
void UIControl::ClearChildren()
{
	for (auto* c : *m_items)
	{
		c->SetLogicalParent(nullptr);
	}
	m_items->clear();
}

//------------------------------------------------------------------------------
void UIControl::SetLayoutPanel(UILayoutPanel* newPanel)
{
	if (newPanel != m_itemsHostPanel)
	{
		// 既に持っていれば取り除いておく
		if (m_itemsHostPanel != nullptr)
		{
			for (auto* c : *m_items)
			{
				m_itemsHostPanel->GetChildren()->remove(c);
			}

			RemoveVisualChild(m_itemsHostPanel);
			m_itemsHostPanel = nullptr;
		}

		// 新しく保持する
		if (newPanel != nullptr)
		{
			AddVisualChild(newPanel);
			m_itemsHostPanel = newPanel;

			for (auto* c : *m_items)
			{
				m_itemsHostPanel->GetChildren()->add(c);
			}
		}

		// 変更通知
		OnLayoutPanelChanged(m_itemsHostPanel);

		//m_invalidateItemsHostPanel = true;
	}
}

//------------------------------------------------------------------------------
UILayoutPanel* UIControl::GetLayoutPanel() const
{
	return m_itemsHostPanel;
}


////------------------------------------------------------------------------------
//int UIControl::getVisualChildrenCount() const
//{
//	return (m_visualTreeRoot != nullptr) ? 1 : 0;
//}
//
////------------------------------------------------------------------------------
//ILayoutElement* UIControl::getVisualChild(int index) const
//{
//	LN_THROW(0 <= index && index < getVisualChildrenCount(), OutOfRangeException);
//	return m_visualTreeRoot;
//}
//
//------------------------------------------------------------------------------
Size UIControl::measureOverride(const Size& constraint)
{
	//if (m_invalidateItemsHostPanel)
	//{

	//	m_invalidateItemsHostPanel = false;
	//}


#if 1
	Size desiredSize = UIElement::measureOverride(constraint);

	m_itemsHostPanel->measureLayout(constraint);
	const Size& childDesiredSize = m_itemsHostPanel->getDesiredSize();

	desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
	desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);

	return desiredSize;
#else
	return detail::LayoutImpl<UIControl>::UILayoutPanel_MeasureOverride(
		this, constraint,
		[](UIControl* panel, const Size& constraint) { return panel->UIElement::MeasureOverride(constraint); });
	//Size desiredSize = UIElement::measureOverride(constraint);
	//if (m_visualTreeRoot != nullptr)
	//{
	//    m_visualTreeRoot->measureLayout(constraint);
	//    const Size& childDesiredSize = m_visualTreeRoot->getDesiredSize();

	//    desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
	//    desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
	//}
	//return desiredSize;
#endif

}

//------------------------------------------------------------------------------
Size UIControl::arrangeOverride(const Size& finalSize)
{
#if 1
	Size childDesiredSize = m_itemsHostPanel->getDesiredSize();
	childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
	childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
	m_itemsHostPanel->arrangeLayout(Rect(0.0f, 0.0f, childDesiredSize));
	return finalSize;
#else
	return detail::LayoutImpl<UIControl>::UILayoutPanel_ArrangeOverride(this, Vector2::Zero, finalSize);
	//RectF childFinal(0, 0, finalSize);
	//if (m_visualTreeRoot != nullptr)
	//{
	//    Size childDesiredSize = m_visualTreeRoot->getDesiredSize();
	//    childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
	//    childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
	//    m_visualTreeRoot->arrangeLayout(RectF(0, 0, childDesiredSize));
	//}
	//return finalSize;
#endif
}


//------------------------------------------------------------------------------
const HAlignment* UIControl::GetPriorityContentHAlignment()
{
	if (HContentAlignment.get() == VAlignment::Stretch) return nullptr;
	return &HContentAlignment.get();
}
//------------------------------------------------------------------------------
const VAlignment* UIControl::GetPriorityContentVAlignment()
{
	if (VContentAlignment.get() == VAlignment::Stretch) return nullptr;
	return &VContentAlignment.get();
}

//------------------------------------------------------------------------------
void UIControl::OnRoutedEvent(UIEventArgs* e)
{
	// TODO: ここでやるべきではない。MFC なら PreTranslate 相当なので。On～で行う。
	if (e->getType() == UIEvents::MouseEnterEvent)
	{
		GoToVisualState(UIVisualStates::MouseOverState);
	}
	else if (e->getType() == UIEvents::MouseLeaveEvent)
	{
		GoToVisualState(UIVisualStates::NormalState);
	}

	UIElement::OnRoutedEvent(e);
}

//------------------------------------------------------------------------------
void UIControl::OnGotFocus(UIEventArgs* e)
{
	GoToVisualState(UIVisualStates::FocusedState);
	UIElement::OnGotFocus(e);
}

//------------------------------------------------------------------------------
void UIControl::OnLostFocus(UIEventArgs* e)
{
	GoToVisualState(UIVisualStates::UnfocusedState);
	UIElement::OnLostFocus(e);
}


//------------------------------------------------------------------------------
//void UIControl::SetVisualTreeRoot(UIElement* element)
//{
//	if (m_visualTreeRoot != nullptr)
//	{
//		m_visualTreeRoot->setParent(nullptr);
//	}
//
//	LN_REFOBJ_SET(m_visualTreeRoot, element);
//
//	if (m_visualTreeRoot != nullptr)
//	{
//		m_visualTreeRoot->setParent(this);
//	}
//}

//------------------------------------------------------------------------------
void UIControl::OnLayoutPanelChanged(UILayoutPanel* newPanel)
{
}

//------------------------------------------------------------------------------
void UIControl::OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e)
{
	switch (e.action)
	{
	case tr::NotifyCollectionChangedAction::Add:
		if (LN_CHECK_STATE(e.newItems.getCount() == 1)) return;	// TODO
		m_itemsHostPanel->GetChildren()->insert(e.newStartingIndex, e.newItems.getAt(0));
		break;
	case tr::NotifyCollectionChangedAction::Move:
		LN_NOTIMPLEMENTED();
		break;
	case tr::NotifyCollectionChangedAction::Remove:
		m_itemsHostPanel->GetChildren()->removeAt(e.oldStartingIndex);
		break;
	case tr::NotifyCollectionChangedAction::replace:
		LN_NOTIMPLEMENTED();
		break;
	case tr::NotifyCollectionChangedAction::reset:
		m_itemsHostPanel->GetChildren()->clear();
		break;
	default:
		break;
	}
}

LN_NAMESPACE_END
