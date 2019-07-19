
#include "Internal.hpp"
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UITextBlock.hpp>
#include <LuminoEngine/UI/UIButton.hpp>

namespace ln {

//==============================================================================
// UIButtonBase

UIButtonBase::UIButtonBase()
    : m_isPressed(false)
{
}

void UIButtonBase::init()
{
    UIControl::init();
    auto vsm = getVisualStateManager();
	vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::Normal);
    vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::MouseOver);
    vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::Pressed);
}

void UIButtonBase::setText(const StringRef& text)
{
    if (!m_textContent) {
        m_textContent = makeObject<UITextBlock>();
        addElement(m_textContent);
    }
    m_textContent->setText(text);
}

void UIButtonBase::onClick(UIEventArgs* e)
{
}

void UIButtonBase::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::MouseDownEvent) {

        m_isPressed = true;
        //focus();
		retainCapture();
        getVisualStateManager()->gotoState(u"Pressed");
        e->handled = true;

        return;
    }
    else if (e->type() == UIEvents::MouseUpEvent) {


        if (m_isPressed)
        {
            m_isPressed = false;
			releaseCapture();
            getVisualStateManager()->gotoState(u"MouseOver");
            onClick(e);
            e->handled = true;
        }

        return;
    }
    else if (e->type() == UIEvents::MouseEnterEvent) {
        if (!m_isPressed) {
            getVisualStateManager()->gotoState(u"MouseOver");
        }
    }
    else if (e->type() == UIEvents::MouseLeaveEvent) {
        if (!m_isPressed) {
            getVisualStateManager()->gotoState(u"Normal");
        }
    }

    UIControl::onRoutedEvent(e);
}

//==============================================================================
// UIButton

UIButton::UIButton()
{
}

void UIButton::init()
{
	UIButtonBase::init();
}


//==============================================================================
// UIToggleButton

UIToggleButton::UIToggleButton()
	: m_checkState(UICheckState::Unchecked)
{
}

void UIToggleButton::init()
{
	UIButtonBase::init();

	auto* vsm = getVisualStateManager();
	vsm->registerState(UIVisualStates::CheckStates, UIVisualStates::CheckedState);
	vsm->registerState(UIVisualStates::CheckStates, UIVisualStates::UncheckedState);

	vsm->gotoState(UIVisualStates::UncheckedState);
}

void UIToggleButton::setChecked(bool checked)
{
	UICheckState newState = (checked) ? UICheckState::Checked : UICheckState::Unchecked;
	if (newState != m_checkState)
	{
		m_checkState = newState;
		checkChanged();
	}
}

bool UIToggleButton::isChecked() const
{
	return m_checkState != UICheckState::Unchecked;
}

EventConnection UIToggleButton::connectOnChecked(UIEventHandler handler)
{
	return m_onChecked.connect(handler);
}

EventConnection UIToggleButton::connectOnUnchecked(UIEventHandler handler)
{
	return m_onUnchecked.connect(handler);
}

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

void UIToggleButton::checkChanged()
{
	switch (m_checkState)
	{
	case ln::UICheckState::Unchecked:
		m_onUnchecked.raise(UIEventArgs::create(this, UIEvents::UncheckedEvent, this));
		getVisualStateManager()->gotoState(UIVisualStates::UncheckedState);
		break;
	case ln::UICheckState::Indeterminate:
		LN_NOTIMPLEMENTED();
		break;
	case ln::UICheckState::Checked:
		m_onChecked.raise(UIEventArgs::create(this, UIEvents::CheckedEvent, this));
		getVisualStateManager()->gotoState(UIVisualStates::CheckedState);
		break;
	default:
		break;
	}
}

} // namespace ln

