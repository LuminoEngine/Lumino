
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UITextBlock.hpp>

#include "../Font/TextLayoutEngine.hpp"
#include "../Font/FontManager.hpp"

// TODO: Test
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>


namespace ln {

//==============================================================================
// UITextBlock
LN_OBJECT_IMPLEMENT(UITextBlock, UIElement) {}

Ref<UITextBlock> UITextBlock::create()
{
    return makeObject<UITextBlock>();
}

Ref<UITextBlock> UITextBlock::create(const StringRef& text)
{
    return makeObject<UITextBlock>(text);
}

UITextBlock::UITextBlock()
{
}

void UITextBlock::init()
{
    UIElement::init();

    // WPF default
    setHorizontalAlignment(HAlignment::Left);
    setVerticalAlignment(VAlignment::Top);

    setBlendMode(BlendMode::Alpha);
}

void UITextBlock::init(const StringRef& text)
{
    init();
    setText(text);
}

Size UITextBlock::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	// TODO: LayoutContext みたいなのを作って、それから dpi scale うけとるようにしてみよう
	float scale = 1.0f;
	if (UIFrameWindow* w = static_cast<UIFrameWindow*>(getFrameWindow())) {
		scale = w->platformWindow()->dpiFactor();
	}

    // TODO: LayoutContext
    detail::FontGlobalMetrics gm;
    auto fc = detail::FontHelper::resolveFontCore(finalStyle()->font, scale);
    fc->getGlobalMetrics(&gm);

    Size size = finalStyle()->font->measureRenderSize(m_text, scale);

	return Size(size.width, gm.lineSpace);
}

void UITextBlock::onRender(UIRenderingContext* context)
{
    //{
    //    auto tex = makeObject<Texture2D>(u"D:/Proj/LN/HC1/Assets/Windowskin/window.png");
    //    auto mat = Material::create(tex);
    //    context->drawBoxBackground(Rect(10, 20, 100, 200), Thickness(16), CornerRadius(), BrushImageDrawMode::BorderFrame, mat, Rect(64, 0, 64, 64));
    //}


    context->drawText(m_text);
}

} // namespace ln

