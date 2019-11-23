
#include "Internal.hpp"
#include <LuminoEngine/UI/UIFocusNavigator.hpp>

namespace ln {

//==============================================================================
// UIFocusNavigator

UIFocusNavigator::UIFocusNavigator()
{
}

void UIFocusNavigator::init()
{
    UIContainerElement::init();
}

void UIFocusNavigator::gotoFocus(UIElement* element)
{
}

void UIFocusNavigator::pushFocus(UIElement* element)
{
    if (LN_REQUIRE(element)) return;
    if (LN_REQUIRE(element->m_focusable)) return;
    m_history.add(element);
    element->focus();
}

void UIFocusNavigator::popFocus()
{
    if (LN_REQUIRE(!m_history.isEmpty())) return;
    m_history.removeLast();

    if (!m_history.isEmpty()) {
        m_history.back()->focus();
    }
}

void UIFocusNavigator::clearFocusHistory()
{
}

} // namespace ln

