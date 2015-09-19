
#include "../Internal.h"
#include <Lumino/GUI/Button.h>

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// Button
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(Button, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(Button);
const String	Button::IsMouseOverProperty(_T("IsMouseOver"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button* Button::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button::Button(GUIManagerImpl* manager)
	: ContentControl(manager)
	, m_isMouseOver(false)
{
	// プロパティの登録
	//LN_DEFINE_PROPERTY(Button, bool, IsMouseOverProperty, NULL, &Button::IsMouseOver, false);

	//m_chrome.Attach(LN_NEW ButtonChrome(manager));
	//SetContent(Variant(m_chrome));

	//MouseMove += LN_CreateDelegate(this, &Button::UIElement_MouseMove);

	// Register handler
	LN_REGISTER_ROUTED_EVENT_HANDLER(Button, MouseEventArgs, UIElement::MouseEnterEvent, RoutedHandler_MouseEnter);
	LN_REGISTER_ROUTED_EVENT_HANDLER(Button, MouseEventArgs, UIElement::MouseLeaveEvent, RoutedHandler_MouseLeave);
	LN_REGISTER_ROUTED_EVENT_HANDLER(Button, MouseEventArgs, UIElement::MouseMoveEvent, RoutedHandler_MouseMove);
	LN_REGISTER_ROUTED_EVENT_HANDLER(Button, MouseEventArgs, UIElement::MouseDownEvent, RoutedHandler_MouseDown);
	LN_REGISTER_ROUTED_EVENT_HANDLER(Button, MouseEventArgs, UIElement::MouseUpEvent, RoutedHandler_MouseUp);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button::~Button()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnClick()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnRender(Painter* painter)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::RoutedHandler_MouseEnter(MouseEventArgs* e)
{
	VisualStateManager::GoToState(this, VisualStatus::MouseOver);
	e->Handled = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::RoutedHandler_MouseLeave(MouseEventArgs* e)
{
	VisualStateManager::GoToState(this, VisualStatus::Normal);
	e->Handled = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::RoutedHandler_MouseMove(MouseEventArgs* e)
{
	//VisualStateManager::GoToState(this, VisualStatus::MouseOver);

	m_isMouseOver = true;
	// TODO
	//OnPropertyChanged(IsMouseOverProperty, m_isMouseOver);
	e->Handled = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::RoutedHandler_MouseDown(MouseEventArgs* e)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::RoutedHandler_MouseUp(MouseEventArgs* e)
{
	
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Button::UIElement_MouseMove(CoreObject* sender, MouseEventArgs* e)
//{
//	m_isMouseOver = true;
//	OnPropertyChanged(IsMouseOverProperty, m_isMouseOver);
//
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Button::Render()
//{
//	printf("Button::Render()\n");
//	ContentControl::Render();
//}

LN_NAMESPACE_GUI_END
} // namespace Lumino
