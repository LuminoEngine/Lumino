
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIButton.h>
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIButtonBase
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIButtonBase, UIControl);

//------------------------------------------------------------------------------
UIButtonBase::UIButtonBase()
	: m_clickMode(ClickMode::Release)
	, m_isPressed(false)
{
}

//------------------------------------------------------------------------------
UIButtonBase::~UIButtonBase()
{
}

//------------------------------------------------------------------------------
void UIButtonBase::Initialize()
{
	UIControl::Initialize();

	HContentAlignment = HAlignment::Center;
	VContentAlignment = VAlignment::Center;

	// TODO: UIControl::Initialize() の中でも作ってるから、そっちが無駄になる。
	// UIControl では何も作らなくてもいいかも。null の場合、UILayoutPanel と同じレイアウトにするとか。
	SetLayoutPanel(NewObject<UIStackPanel>());
}

//------------------------------------------------------------------------------
void UIButtonBase::SetText(const StringRef& text)
{
	auto textBlock = UITextBlockPtr::MakeRef();
	textBlock->Initialize();
	textBlock->SetText(text);
	AddChild(textBlock);
}

//------------------------------------------------------------------------------
EventConnection UIButtonBase::ConnectOnGotFocus(UIEventHandler handler)
{
	return m_onClick.Connect(handler);
}

//------------------------------------------------------------------------------
void UIButtonBase::OnClick(UIEventArgs* e)
{
	m_onClick.Raise(e);
	//RaiseEvent(ClickEvent, this, UIEventArgs::Create(this));
}

//------------------------------------------------------------------------------
void UIButtonBase::OnMouseDown(UIMouseEventArgs* e)
{
	if (m_clickMode == ClickMode::Release)
	{
		m_isPressed = true;
		Focus();
		CaptureMouse();
		GoToVisualState(PressedState);
		e->handled = true;
	}
	else if (m_clickMode == ClickMode::Press)
	{
		OnClick(e);
		e->handled = true;
	}

	UIControl::OnMouseDown(e);
}

//------------------------------------------------------------------------------
void UIButtonBase::OnMouseUp(UIMouseEventArgs* e)
{
	if (m_clickMode == ClickMode::Release)
	{
		if (m_isPressed)
		{
			m_isPressed = false;
			ReleaseMouseCapture();
			GoToVisualState(MouseOverState);
			OnClick(e);
			e->handled = true;
		}
	}

	UIControl::OnMouseUp(e);
}

//==============================================================================
// UIButton
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIButton, UIButtonBase);

//------------------------------------------------------------------------------
RefPtr<UIButton> UIButton::Create()
{
	return NewObject<UIButton>();
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
void UIButton::Initialize()
{
	UIButtonBase::Initialize();
}

//==============================================================================
// UIToggleButton
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIToggleButton, UIButtonBase);

//------------------------------------------------------------------------------
RefPtr<UIToggleButton> UIToggleButton::Create()
{
	return NewObject<UIToggleButton>();
}

//------------------------------------------------------------------------------
UIToggleButton::UIToggleButton()
{
}

//------------------------------------------------------------------------------
UIToggleButton::~UIToggleButton()
{
}

//------------------------------------------------------------------------------
void UIToggleButton::Initialize()
{
	UIButtonBase::Initialize();
}

LN_NAMESPACE_END
