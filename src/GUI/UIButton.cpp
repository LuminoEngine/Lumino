
#include "../Internal.h"
#include <Lumino/GUI/UIButton.h>

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// UIButton
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(UIButton, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(UIButton);
const String	UIButton::IsMouseOverProperty(_T("IsMouseOver"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIButton* UIButton::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIButton::UIButton(GUIManagerImpl* manager)
	: ContentControl(manager)
	, m_isMouseOver(false)
{
	// プロパティの登録
	//LN_DEFINE_PROPERTY(UIButton, bool, IsMouseOverProperty, NULL, &UIButton::IsMouseOver, false);

	//m_chrome.Attach(LN_NEW ButtonChrome(manager));
	//SetContent(Variant(m_chrome));

	//MouseMove += LN_CreateDelegate(this, &UIButton::UIElement_MouseMove);

	// Register handler
	IsEnabledChanged += LN_CreateDelegate(this, &UIButton::Handler_IsEnabledChanged);
	LN_REGISTER_ROUTED_EVENT_HANDLER(UIButton, MouseEventArgs, UIElement::MouseEnterEvent, RoutedHandler_MouseEnter);
	LN_REGISTER_ROUTED_EVENT_HANDLER(UIButton, MouseEventArgs, UIElement::MouseLeaveEvent, RoutedHandler_MouseLeave);
	LN_REGISTER_ROUTED_EVENT_HANDLER(UIButton, MouseEventArgs, UIElement::MouseMoveEvent, RoutedHandler_MouseMove);
	LN_REGISTER_ROUTED_EVENT_HANDLER(UIButton, MouseEventArgs, UIElement::MouseDownEvent, RoutedHandler_MouseDown);
	LN_REGISTER_ROUTED_EVENT_HANDLER(UIButton, MouseEventArgs, UIElement::MouseUpEvent, RoutedHandler_MouseUp);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIButton::~UIButton()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIButton::OnClick()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIButton::OnRender(Painter* painter)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIButton::Handler_IsEnabledChanged(PropertyChangedEventArgs* e)
{
	if (e->NewValue.GetBool()) {
		VisualStateManager::GoToState(this, VisualStatus::Normal);
	}
	else {
		VisualStateManager::GoToState(this, VisualStatus::Disabled);	// TODO: 前の値に戻さなくて良い？
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIButton::RoutedHandler_MouseEnter(MouseEventArgs* e)
{
	VisualStateManager::GoToState(this, VisualStatus::MouseOver);
	e->Handled = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIButton::RoutedHandler_MouseLeave(MouseEventArgs* e)
{
	VisualStateManager::GoToState(this, VisualStatus::Normal);
	e->Handled = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIButton::RoutedHandler_MouseMove(MouseEventArgs* e)
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
void UIButton::RoutedHandler_MouseDown(MouseEventArgs* e)
{
	Focus();
	VisualStateManager::GoToState(this, VisualStatus::Pressed);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIButton::RoutedHandler_MouseUp(MouseEventArgs* e)
{
	VisualStateManager::GoToState(this, VisualStatus::Normal);
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void UIButton::UIElement_MouseMove(CoreObject* sender, MouseEventArgs* e)
//{
//	m_isMouseOver = true;
//	OnPropertyChanged(IsMouseOverProperty, m_isMouseOver);
//
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void UIButton::Render()
//{
//	printf("UIButton::Render()\n");
//	ContentControl::Render();
//}

LN_NAMESPACE_GUI_END
} // namespace Lumino
