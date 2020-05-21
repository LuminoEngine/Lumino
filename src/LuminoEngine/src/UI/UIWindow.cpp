
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIWindow.hpp>

namespace ln {

//==============================================================================
// UIWindow
// https://material-ui.com/demos/cards/

Ref<UIWindow> UIWindow::create()
{
    return makeObject<UIWindow>();
}

UIWindow::UIWindow()
    : m_opend(false)
{
    auto vsm = getVisualStateManager();
    vsm->registerState(UIVisualStates::DisplayStates, UIVisualStates::Opend);
    vsm->registerState(UIVisualStates::DisplayStates, UIVisualStates::Closed);
    open();
}

void UIWindow::init()
{
    UIContainerElement::init();
}

void UIWindow::open()
{
    if (!m_opend) {
        if (auto vsm = getVisualStateManager()) {
            vsm->gotoState(UIVisualStates::Opend);
        }
        m_opend = true;
    }
}

void UIWindow::close()
{
    if (m_opend) {
        if (auto vsm = getVisualStateManager()) {
            vsm->gotoState(UIVisualStates::Closed);
        }
        m_opend = false;
    }
}

} // namespace ln

