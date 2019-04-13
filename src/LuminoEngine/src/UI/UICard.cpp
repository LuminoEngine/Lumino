
#include "Internal.hpp"
#include <LuminoEngine/UI/UICard.hpp>

namespace ln {

//==============================================================================
// UICard
// https://material-ui.com/demos/cards/

Ref<UICard> UICard::create()
{
    return newObject<UICard>();
}

UICard::UICard()
{
}

void UICard::init()
{
    UIContainerElement::init();
}

} // namespace ln

