
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
    vsm->registerState(u"Common", u"MouseOver");
    vsm->registerState(u"Common", u"Pressed");
}

void UIButtonBase::setText(const StringRef& text)
{
    if (!m_textContent) {
        m_textContent = makeObject<UITextBlock>();
        addElement(m_textContent);
    }
    m_textContent->setText(text);
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
            //onClick(e);
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
{
}

void UIToggleButton::init()
{
	UIButtonBase::init();
}

} // namespace ln

