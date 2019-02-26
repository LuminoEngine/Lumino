
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UITextBlock.hpp>

namespace ln {

//==============================================================================
// UITextBlock

Ref<UITextBlock> UITextBlock::create()
{
    return newObject<UITextBlock>();
}

UITextBlock::UITextBlock()
{
}

void UITextBlock::init()
{
    UIElement::init();
    setBlendMode(BlendMode::Alpha);
}

void UITextBlock::onRender(UIRenderingContext* context)
{
    context->drawText(m_text, Font::create(), actualStyle()->textColor);
}

} // namespace ln

