
#include "Internal.hpp"
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UICommand.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIText.hpp>
#include <LuminoEngine/UI/Controls/UIButton.hpp>

namespace ln {

//==============================================================================
// UIButtonBase

LN_OBJECT_IMPLEMENT(UIButtonBase, UIControl) {}

UIButtonBase::UIButtonBase()
    : m_isPressed(false)
{
}

bool UIButtonBase::init()
{
    if (!UIControl::init()) return false;
    auto vsm = getVisualStateManager();
    vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::Pressed);
    return true;
}

void UIButtonBase::setText(const StringRef& text)
{
    if (!m_textContent) {
        m_textContent = makeObject<UIText>();
        addElement(m_textContent);
    }
    m_textContent->setText(text);
}

void UIButtonBase::setCommand(UICommand* command)
{
    m_command = command;
}

void UIButtonBase::onClick(UIEventArgs* e)
{
    if (m_command) {
        auto args = UICommandEventArgs::create(this, UIEvents::ExecuteCommandEvent, m_command);
        raiseEvent(args);
    }
}

void UIButtonBase::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::MouseDownEvent) {

        m_isPressed = true;
        //focus();
		retainCapture();
        getVisualStateManager()->gotoState(_TT("Pressed"));
        e->handled = true;

        return;
    }
    else if (e->type() == UIEvents::MouseUpEvent) {


        if (m_isPressed)
        {
            m_isPressed = false;
			releaseCapture();
            getVisualStateManager()->gotoState(_TT("MouseOver"));
            onClick(e);
            e->handled = true;
        }

        return;
    }
    //else if (e->type() == UIEvents::MouseEnterEvent) {
    //    if (!m_isPressed) {
    //        getVisualStateManager()->gotoState(_TT("MouseOver");
    //    }
    //}
    //else if (e->type() == UIEvents::MouseLeaveEvent) {
    //    if (!m_isPressed) {
    //        getVisualStateManager()->gotoState(_TT("Normal");
    //    }
    //}

    UIControl::onRoutedEvent(e);
}

//==============================================================================
// UIButton

LN_OBJECT_IMPLEMENT(UIButton, UIButtonBase) {}

Ref<UIButton> UIButton::create()
{
    return makeObject<UIButton>();
}

Ref<UIButton> UIButton::create(const StringRef& text)
{
    return makeObject<UIButton>(text);
}

UIButton::UIButton()
{
}

bool UIButton::init()
{
    if (!UIButtonBase::init()) return false;
    return true;
}

void UIButton::init(const StringRef& text)
{
    init();
    setText(text);
}

//Ref<EventConnection> UIButton::connectOnClicked(UIGeneralEventHandler handler)
//{
//    return m_onClicked.connect(handler);
//}
//
Ref<EventConnection> UIButton::connectOnClicked(Ref<UIEventHandler> handler)
{
	return m_onClicked.connect(handler);
}

void UIButton::onClick(UIEventArgs* e)
{
    UIButtonBase::onClick(e);
    m_onClicked.raise();
}

//==============================================================================
// UIToggleButton

UIToggleButton::UIToggleButton()
	: m_checkState(UICheckState::Unchecked)
{
}

bool UIToggleButton::init()
{
    if (!UIButtonBase::init()) return false;

	auto* vsm = getVisualStateManager();
	vsm->registerState(UIVisualStates::CheckStates, UIVisualStates::CheckedState);
	vsm->registerState(UIVisualStates::CheckStates, UIVisualStates::UncheckedState);

	vsm->gotoState(UIVisualStates::UncheckedState);

    return true;
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

Ref<EventConnection> UIToggleButton::connectOnChecked(Ref<UIGeneralEventHandler> handler)
{
	return m_onChecked.connect(handler);
}

Ref<EventConnection> UIToggleButton::connectOnUnchecked(Ref<UIGeneralEventHandler> handler)
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

//==============================================================================
// UIButton::BuilderDetails

void UIButton::BuilderDetails::apply(UIButton* p) const
{
    UIElement::BuilderDetails::apply(p);
    p->setText(text);
}

} // namespace ln

