
#include "Internal.h"
#include <Lumino/UI/UIControl.h>
#include <Lumino/UI/UILayoutPanel.h>
#include "LayoutImpl.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIControl
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIControl, UIElement);

//------------------------------------------------------------------------------
UIControl::UIControl()
	: HContentAlignment(HAlignment::Stretch)
	, VContentAlignment(VAlignment::Stretch)
{
}

//------------------------------------------------------------------------------
UIControl::~UIControl()
{
}

//------------------------------------------------------------------------------
void UIControl::initialize()
{
	UIElement::initialize();
	setFocusable(true);

	auto* vsm = getVisualStateManager();
	vsm->registerVisualState(UIVisualStates::CommonGroup, UIVisualStates::NormalState);
	vsm->registerVisualState(UIVisualStates::CommonGroup, UIVisualStates::MouseOverState);
	vsm->registerVisualState(UIVisualStates::CommonGroup, UIVisualStates::PressedState);
	vsm->registerVisualState(UIVisualStates::CommonGroup, UIVisualStates::DisabledState);
	vsm->registerVisualState(UIVisualStates::FocusGroup, UIVisualStates::UnfocusedState);
	vsm->registerVisualState(UIVisualStates::FocusGroup, UIVisualStates::FocusedState);
	goToVisualState(UIVisualStates::NormalState);

	HContentAlignment = HAlignment::Stretch;
	VContentAlignment = VAlignment::Stretch;


	m_items = RefPtr<UIElementCollection>::makeRef(this);
	auto panel = newObject<UIAbsoluteLayout>();
	setLayoutPanel(panel);
}

//------------------------------------------------------------------------------
UIElementCollection* UIControl::getItems() const
{
	return m_items;
}

//------------------------------------------------------------------------------
void UIControl::addChild(UIElement* element)
{
	m_items->add(element);
	element->setLogicalParent(this);
}

//------------------------------------------------------------------------------
void UIControl::removeChild(UIElement* element)
{
	element->setLogicalParent(nullptr);
	m_items->remove(element);
}

//------------------------------------------------------------------------------
void UIControl::clearChildren()
{
	for (auto* c : *m_items)
	{
		c->setLogicalParent(nullptr);
	}
	m_items->clear();
}

//------------------------------------------------------------------------------
void UIControl::setLayoutPanel(UILayoutPanel* newPanel)
{
	if (newPanel != m_itemsHostPanel)
	{
		// 既に持っていれば取り除いておく
		if (m_itemsHostPanel != nullptr)
		{
			for (auto* c : *m_items)
			{
				m_itemsHostPanel->getChildren()->remove(c);
			}

			removeVisualChild(m_itemsHostPanel);
			m_itemsHostPanel = nullptr;
		}

		// 新しく保持する
		if (newPanel != nullptr)
		{
			addVisualChild(newPanel);
			m_itemsHostPanel = newPanel;

			for (auto* c : *m_items)
			{
				m_itemsHostPanel->getChildren()->add(c);
			}
		}

		// 変更通知
		onLayoutPanelChanged(m_itemsHostPanel);

		//m_invalidateItemsHostPanel = true;
	}
}

//------------------------------------------------------------------------------
UILayoutPanel* UIControl::getLayoutPanel() const
{
	return m_itemsHostPanel;
}

//------------------------------------------------------------------------------
EventConnection UIControl::connectOnSubmit(UIEventHandler handler)
{
	return m_onSubmit.connect(handler);
}

//------------------------------------------------------------------------------
void UIControl::onSubmit(UIEventArgs* e)
{
	m_onSubmit.raise(e);
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
const HAlignment* UIControl::getPriorityContentHAlignment()
{
	if (HContentAlignment == VAlignment::Stretch) return nullptr;
	return &HContentAlignment;
}
//------------------------------------------------------------------------------
const VAlignment* UIControl::getPriorityContentVAlignment()
{
	if (VContentAlignment == VAlignment::Stretch) return nullptr;
	return &VContentAlignment;
}

//------------------------------------------------------------------------------
void UIControl::onRoutedEvent(UIEventArgs* e)
{
	// TODO: ここでやるべきではない。MFC なら PreTranslate 相当なので。On～で行う。
	if (e->getType() == UIEvents::MouseEnterEvent)
	{
		goToVisualState(UIVisualStates::MouseOverState);
	}
	else if (e->getType() == UIEvents::MouseLeaveEvent)
	{
		goToVisualState(UIVisualStates::NormalState);
	}

	UIElement::onRoutedEvent(e);
}

//------------------------------------------------------------------------------
void UIControl::onGotFocus(UIEventArgs* e)
{
	goToVisualState(UIVisualStates::FocusedState);
	UIElement::onGotFocus(e);
}

//------------------------------------------------------------------------------
void UIControl::onLostFocus(UIEventArgs* e)
{
	goToVisualState(UIVisualStates::UnfocusedState);
	UIElement::onLostFocus(e);
}

//------------------------------------------------------------------------------
void UIControl::onMouseDown(UIMouseEventArgs* e)
{/*
	if (!m_isPressed)
	{
		m_isPressed = true;
		focus();
		captureMouse();
		goToVisualState(UIVisualStates::PressedState);
		e->handled = true;
	}*/

	UIElement::onMouseDown(e);
}

//------------------------------------------------------------------------------
void UIControl::onMouseUp(UIMouseEventArgs* e)
{
	//if (m_isPressed)
	//{
	//	m_isPressed = false;
	//	releaseMouseCapture();
	//	goToVisualState(UIVisualStates::MouseOverState);
	//	onClick(e);
	//	e->handled = true;
	//}

	UIElement::onMouseUp(e);
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
void UIControl::onLayoutPanelChanged(UILayoutPanel* newPanel)
{
}

//------------------------------------------------------------------------------
void UIControl::onChildCollectionChanged(const tr::ChildCollectionChangedArgs& e)
{
	switch (e.action)
	{
	case tr::NotifyCollectionChangedAction::Add:
		if (LN_CHECK_STATE(e.newItems.getCount() == 1)) return;	// TODO
		m_itemsHostPanel->getChildren()->insert(e.newStartingIndex, e.newItems.getAt(0));
		break;
	case tr::NotifyCollectionChangedAction::Move:
		LN_NOTIMPLEMENTED();
		break;
	case tr::NotifyCollectionChangedAction::Remove:
		m_itemsHostPanel->getChildren()->removeAt(e.oldStartingIndex);
		break;
	case tr::NotifyCollectionChangedAction::replace:
		LN_NOTIMPLEMENTED();
		break;
	case tr::NotifyCollectionChangedAction::reset:
		m_itemsHostPanel->getChildren()->clear();
		break;
	default:
		break;
	}
}

//==============================================================================
// UIUserControl
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIUserControl, UIControl);

//------------------------------------------------------------------------------
RefPtr<UIUserControl> UIUserControl::create()
{
	return newObject<UIUserControl>();
}

//------------------------------------------------------------------------------
UIUserControl::UIUserControl()
{
}

//------------------------------------------------------------------------------
UIUserControl::~UIUserControl()
{
}

//------------------------------------------------------------------------------
void UIUserControl::initialize()
{
	UIControl::initialize();
}

LN_NAMESPACE_END
