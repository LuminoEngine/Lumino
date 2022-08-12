
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/Controls/UIWindow.hpp>

namespace ln {

//==============================================================================
// UIWindow
// https://material-ui.com/demos/cards/

LN_OBJECT_IMPLEMENT(UIWindow, UIControl) {
}

Ref<UIWindow> UIWindow::create() {
    return makeObject_deprecated<UIWindow>();
}

UIWindow::UIWindow()
    : m_opend(false) {
    specialElementFlags().set(detail::UISpecialElementFlags::FloatingSiblingOrder, true);

    auto vsm = getVisualStateManager();
    vsm->registerState(UIVisualStates::DisplayStates, UIVisualStates::Opend);
    vsm->registerState(UIVisualStates::DisplayStates, UIVisualStates::Closed);
    open();
}

Result UIWindow::init() {
    return UIControl::init();
}

void UIWindow::open() {
    if (!m_opend) {
        if (auto vsm = getVisualStateManager()) {
            vsm->gotoState(UIVisualStates::Opend);
        }
        m_opend = true;
    }
}

void UIWindow::close() {
    if (m_opend) {
        if (auto vsm = getVisualStateManager()) {
            vsm->gotoState(UIVisualStates::Closed);
        }
        m_opend = false;
    }
}

} // namespace ln
