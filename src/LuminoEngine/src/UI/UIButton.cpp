
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIButton.hpp>

namespace ln {

//==============================================================================
// UIButton

UIButton::UIButton()
{
}

void UIButton::init()
{
    UIControl::init();
    //auto vsm = getVisualStateManager();
    //vsm->registerState(u"Common", u"Pressed");
    //vsm->gotoState(u"Pressed");
}

} // namespace ln

