
#include "Internal.hpp"
#include <LuminoFramework/UI/UIEvents.hpp>
#include <LuminoFramework/UI/UICommand.hpp>
#include <LuminoFramework/UI/UIStyle.hpp>
#include <LuminoFramework/UI/UIText.hpp>
#include <LuminoFramework/UI/Controls/UILinkLabel.hpp>

namespace ln {

//==============================================================================
// UILinkLabel

LN_OBJECT_IMPLEMENT(UILinkLabel, UIControl) {}

UILinkLabel::UILinkLabel()
    : m_isPressed(false)
{
}

bool UILinkLabel::init()
{
    if (!UIControl::init()) return false;
    return true;
}

void UILinkLabel::setText(const StringView& value)
{
    if (!m_textContent) {
        m_textContent = makeObject_deprecated<UIText>();
        addElement(m_textContent);
    }
    m_textContent->setText(value);
}

void UILinkLabel::setUrl(const StringView& value)
{
    m_url = value;
}

void UILinkLabel::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::MouseDownEvent) {
        m_isPressed = true;
        retainCapture();
        e->handled = true;
        return;
    }
    else if (e->type() == UIEvents::MouseUpEvent) {
        if (m_isPressed) {
            m_isPressed = false;
            releaseCapture();
            e->handled = true;
            raiseRequestNavigateEvent();
        }
        return;
    }

    UIControl::onRoutedEvent(e);
}

void UILinkLabel::raiseRequestNavigateEvent()
{
    auto args = UIRequestNavigateEventArgs::create(this, UIEvents::RequestNavigate, m_url);
    raiseEvent(args);
}

} // namespace ln

