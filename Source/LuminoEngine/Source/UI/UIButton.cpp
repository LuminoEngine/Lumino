
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIButton.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIButton
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIButton, UIContentControl);

// Event definition
LN_ROUTED_EVENT_IMPLEMENT(UIButton, UIMouseEventArgs, ClickEvent, "Click", click);

//------------------------------------------------------------------------------
UIButtonPtr UIButton::Create()
{
	auto ptr = UIButtonPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIButton::UIButton()
{

}

//------------------------------------------------------------------------------
UIButton::~UIButton()
{

}

//------------------------------------------------------------------------------
void UIButton::Initialize(detail::UIManager* manager)
{
	UIContentControl::Initialize(manager);
}

//------------------------------------------------------------------------------
void UIButton::SetText(const StringRef& text)
{
	auto textBlock = UITextBlockPtr::MakeRef();
	textBlock->Initialize(GetManager());
	textBlock->SetText(text);
	SetContent(textBlock);
}

//------------------------------------------------------------------------------
void UIButton::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	UIContentControl::OnRoutedEvent(ev, e);
}

//------------------------------------------------------------------------------
void UIButton::OnClick()
{
	RaiseEvent(ClickEvent, this, UIEventArgs::Create(this));
}

//------------------------------------------------------------------------------
void UIButton::OnMouseDown(UIMouseEventArgs* e)
{
	m_isPressed = true;
	Focus();
	CaptureMouse();
	GoToVisualState(PressedState);
	e->handled = true;

	UIContentControl::OnMouseDown(e);
}

//------------------------------------------------------------------------------
void UIButton::OnMouseUp(UIMouseEventArgs* e)
{
	if (m_isPressed)
	{
		m_isPressed = false;
		ReleaseMouseCapture();
		GoToVisualState(MouseOverState);
		OnClick();
		e->handled = true;
	}

	UIContentControl::OnMouseUp(e);
}

LN_NAMESPACE_END
