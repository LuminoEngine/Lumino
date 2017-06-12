
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIButton.h>
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIButtonBase
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIButtonBase, UIControl);

//------------------------------------------------------------------------------
UIButtonBase::UIButtonBase()
	: m_clickMode(ClickMode::release)
	, m_isPressed(false)
{
}

//------------------------------------------------------------------------------
UIButtonBase::~UIButtonBase()
{
}

//------------------------------------------------------------------------------
void UIButtonBase::initialize()
{
	UIControl::initialize();

	HContentAlignment = HAlignment::Center;
	VContentAlignment = VAlignment::Center;

	// TODO: UIControl::initialize() の中でも作ってるから、そっちが無駄になる。
	// UIControl では何も作らなくてもいいかも。null の場合、UILayoutPanel と同じレイアウトにするとか。
	SetLayoutPanel(NewObject<UIStackPanel>());
}

//------------------------------------------------------------------------------
void UIButtonBase::SetText(const StringRef& text)
{
	auto textBlock = UITextBlockPtr::MakeRef();
	textBlock->initialize();
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
	if (m_clickMode == ClickMode::release)
	{
		m_isPressed = true;
		Focus();
		CaptureMouse();
		GoToVisualState(UIVisualStates::PressedState);
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
	if (m_clickMode == ClickMode::release)
	{
		if (m_isPressed)
		{
			m_isPressed = false;
			ReleaseMouseCapture();
			GoToVisualState(UIVisualStates::MouseOverState);
			OnClick(e);
			e->handled = true;
		}
	}

	UIControl::OnMouseUp(e);
}

//==============================================================================
// UIButton
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIButton, UIButtonBase);

//------------------------------------------------------------------------------
RefPtr<UIButton> UIButton::Create()
{
	return NewObject<UIButton>();
}

//------------------------------------------------------------------------------
RefPtr<UIButton> UIButton::Create(const StringRef& text, float width, float height)
{
	return NewObject<UIButton>(text, width, height);
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
void UIButton::initialize()
{
	UIButtonBase::initialize();
}

//------------------------------------------------------------------------------
void UIButton::initialize(const StringRef& text, float width, float height)
{
	UIButtonBase::initialize();
	SetText(text);
	SetWidth(width);
	SetHeight(height);
}

//==============================================================================
// UIToggleButton
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIToggleButton, UIButtonBase);

const String UIToggleButton::CheckedState = _T("Checked");
const String UIToggleButton::UncheckedState = _T("Unchecked");

//------------------------------------------------------------------------------
RefPtr<UIToggleButton> UIToggleButton::Create()
{
	return NewObject<UIToggleButton>();
}

//------------------------------------------------------------------------------
UIToggleButton::UIToggleButton()
	: m_isChecked(false)
{
}

//------------------------------------------------------------------------------
UIToggleButton::~UIToggleButton()
{
}

//------------------------------------------------------------------------------
void UIToggleButton::initialize()
{
	UIButtonBase::initialize();

	auto* vsm = GetVisualStateManager();
	vsm->RegisterVisualState(UIVisualStates::CommonGroup, CheckedState);
	vsm->RegisterVisualState(UIVisualStates::CommonGroup, UncheckedState);
}

//------------------------------------------------------------------------------
void UIToggleButton::OnClick(UIEventArgs* e)
{
	m_isChecked = !m_isChecked;

	if (m_isChecked)
	{
		GoToVisualState(CheckedState);
	}
	else
	{
		GoToVisualState(UncheckedState);
	}

	UIButtonBase::OnClick(e);
}


LN_NAMESPACE_END
