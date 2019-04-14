
#include "Internal.hpp"
#include <LuminoEngine/UI/UIWindow.hpp>

namespace ln {

//==============================================================================
// UIWindow
// https://material-ui.com/demos/cards/

Ref<UIWindow> UIWindow::create()
{
    return newObject<UIWindow>();
}

UIWindow::UIWindow()
{
}

void UIWindow::init()
{
    UIContainerElement::init();
}

} // namespace ln

