
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIButton.h>
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIButtonBase
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIButtonBase, UIContentsControl);

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
void UIButtonBase::initialize()
{
	UIContentsControl::initialize();

	HContentAlignment = HAlignment::Center;
	VContentAlignment = VAlignment::Center;
}

//------------------------------------------------------------------------------
void UIButtonBase::setText(const StringRef& text)
{
	if (m_textContent == nullptr)
	{
		m_textContent = newObject<UITextBlock>();
		addChild(m_textContent);
	}
	m_textContent->setText(text);
}

//------------------------------------------------------------------------------
EventConnection UIButtonBase::connectOnClick(UIEventHandler handler)
{
	return m_onClick.connect(handler);
}

//------------------------------------------------------------------------------
void UIButtonBase::onClick(UIEventArgs* e)
{
	m_onClick.raise(e);
	//raiseEvent(ClickEvent, this, UIEventArgs::create(this));
}

//------------------------------------------------------------------------------
void UIButtonBase::onMouseDown(UIMouseEventArgs* e)
{
	if (m_clickMode == ClickMode::Release)
	{
		m_isPressed = true;
		focus();
		captureMouse();
		goToVisualState(UIVisualStates::PressedState);
		e->handled = true;
	}
	else if (m_clickMode == ClickMode::Press)
	{
		onClick(e);
		e->handled = true;
	}

	UIContentsControl::onMouseDown(e);
}

//------------------------------------------------------------------------------
void UIButtonBase::onMouseUp(UIMouseEventArgs* e)
{
	if (m_clickMode == ClickMode::Release)
	{
		if (m_isPressed)
		{
			m_isPressed = false;
			releaseMouseCapture();
			goToVisualState(UIVisualStates::MouseOverState);
			onClick(e);
			e->handled = true;
		}
	}

	UIContentsControl::onMouseUp(e);
}

//==============================================================================
// UIButton
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIButton, UIButtonBase);

//------------------------------------------------------------------------------
Ref<UIButton> UIButton::create()
{
	return newObject<UIButton>();
}

//------------------------------------------------------------------------------
Ref<UIButton> UIButton::create(const StringRef& text, float width, float height)
{
	return newObject<UIButton>(text, width, height);
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
	UIButton::initialize();
	setText(text);
	setWidth(width);
	setHeight(height);
}

//==============================================================================
// UIToggleButton
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIToggleButton, UIButtonBase);

const String UIToggleButton::CheckedState = _LT("Checked");
const String UIToggleButton::UncheckedState = _LT("Unchecked");

//------------------------------------------------------------------------------
Ref<UIToggleButton> UIToggleButton::create()
{
	return newObject<UIToggleButton>();
}

//------------------------------------------------------------------------------
Ref<UIToggleButton> UIToggleButton::create(const StringRef& text, float width, float height)
{
	return newObject<UIToggleButton>(text, width, height);
}

//------------------------------------------------------------------------------
UIToggleButton::UIToggleButton()
	: m_checkState(UICheckState::Unchecked)
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

	auto* vsm = getVisualStateManager();
	vsm->registerVisualState(UIVisualStates::CheckStates, CheckedState);
	vsm->registerVisualState(UIVisualStates::CheckStates, UncheckedState);

	goToVisualState(UncheckedState);
}

//------------------------------------------------------------------------------
void UIToggleButton::initialize(const StringRef& text, float width, float height)
{
	UIToggleButton::initialize();
	setText(text);
	setWidth(width);
	setHeight(height);
}

//------------------------------------------------------------------------------
void UIToggleButton::setChecked(bool checked)
{
	UICheckState newState = (checked) ? UICheckState::Checked : UICheckState::Unchecked;
	if (newState != m_checkState)
	{
		m_checkState = newState;
		checkChanged();
	}
}

//------------------------------------------------------------------------------
bool UIToggleButton::isChecked() const
{
	return m_checkState != UICheckState::Unchecked;
}

//------------------------------------------------------------------------------
EventConnection UIToggleButton::connectOnChecked(UIEventHandler handler)
{
	return m_onChecked.connect(handler);
}

//------------------------------------------------------------------------------
EventConnection UIToggleButton::connectOnUnchecked(UIEventHandler handler)
{
	return m_onUnchecked.connect(handler);
}

//------------------------------------------------------------------------------
void UIToggleButton::onClick(UIEventArgs* e)
{
	if (m_checkState != UICheckState::Checked)
	{
		m_checkState = UICheckState::Checked;
	}
	else
	{
		m_checkState = UICheckState::Unchecked;
	}

	checkChanged();

	UIButtonBase::onClick(e);
}

//------------------------------------------------------------------------------
void UIToggleButton::checkChanged()
{
	switch (m_checkState)
	{
	case ln::UICheckState::Unchecked:
		m_onUnchecked.raise(UIEventArgs::create(UIEvents::UncheckedEvent, this));
		goToVisualState(UncheckedState);
		break;
	case ln::UICheckState::Indeterminate:
		break;
	case ln::UICheckState::Checked:
		m_onChecked.raise(UIEventArgs::create(UIEvents::CheckedEvent, this));
		goToVisualState(CheckedState);
		break;
	default:
		break;
	}
}

LN_NAMESPACE_END
