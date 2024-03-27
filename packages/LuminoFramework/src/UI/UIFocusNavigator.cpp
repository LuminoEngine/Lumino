#include "Internal.hpp"
#include <LuminoFramework/UI/UIFocusNavigator.hpp>

namespace ln {

//==============================================================================
// UIFocusNavigator

UIFocusNavigator::UIFocusNavigator()
{
}

bool UIFocusNavigator::init()
{
    if (!Object::init()) return false;
    return true;
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

UIElement* UIFocusNavigator::focusedElement() const
{
	if (m_history.isEmpty()) return nullptr;
	return m_history.back();
}

//UIElement* UIFocusNavigator::lookupMouseHoverElement(const Point& frameClientPosition)
//{
//	if (UIElement* focused = focusedElement()) {
//		if (UIElement* e = focused->lookupMouseHoverElement(frameClientPosition)) {
//			return e;
//		}
//	}
//
//	return nullptr;
//}

} // namespace ln

