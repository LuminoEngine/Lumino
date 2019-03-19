
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UITextBlock.hpp>

// TODO: Test
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>


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

Size UITextBlock::measureOverride(const Size& constraint)
{
	return Size(100, 20);
}

void UITextBlock::onRender(UIRenderingContext* context)
{
    {
        auto tex = newObject<Texture2D>(u"D:/Proj/LN/HC1/Assets/Windowskin/window.png");
        auto mat = Material::create(tex);
        context->drawBoxBackground(Rect(10, 20, 100, 200), Thickness(16), CornerRadius(), BrushImageDrawMode::BorderFrame, mat, Rect(64, 0, 64, 64));
    }


    context->drawText(m_text, finalStyle().textColor, finalStyle().font);
}

} // namespace ln

