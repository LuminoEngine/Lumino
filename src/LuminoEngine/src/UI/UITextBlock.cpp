
#include "Internal.hpp"
#include <LuminoEngine/Engine/VMProperty.hpp>

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
LN_OBJECT_IMPLEMENT(UITextBlock, UIElement) {
	 typeInfo->registerViewProperty(makeRef<ViewPropertyInfo>(TypeInfo::getTypeInfo<String>(), "text", LN_MAKE_VIEW_PROPERTY_ACCESSOR(UITextBlock, String, text, setText)));
}


ViewProperty* UITextBlock::getViewProperty(StringRef name)
{
	ViewPropertyInfo* info = TypeInfo::getTypeInfo(this)->findViewProperty(name);
	auto itr = std::find_if(m_viewProperties.begin(), m_viewProperties.end(), [&](auto& x) { return x->m_info == info; });
	if (itr != m_viewProperties.end())
		return (*itr);
	else {
		auto prop = makeRef<ViewProperty>();
		prop->m_owner = this;
		prop->m_info = info;
		m_viewProperties.push_back(prop);
		return prop;
	}
}


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

    // Alignment は Center をデフォルトにしてみる。
    // Web だと Top-Left だが、これは下方向のサイズ上限がないっていうのと、活版印刷を意識しているため…と思ってる。
    //
    // WPF も Top-Left がデフォルト。これは Desktop アプリの文化かな。
    // 実際のところ、これまでいろいろなアプリ作ってきたけど Center-Left が一番よく使う。次に Top-Center.
    //
    // モバイル・タブレット系の Framework では Center-Center が多い。
    // Lumino は今後モバイル・タブレット系に寄せていきたいところ。
    // 
    // また、デフォルトが Top-Left だと表示したときぱっと目に入りづらいことがあるので、デバッグや導入的な視点からちょっとマイナスかも。
    setHAlignment(HAlignment::Center);
    setVAlignment(VAlignment::Center);


    // WPF default
    //setHAlignment(HAlignment::Left);
    //setVAlignment(VAlignment::Top);

    setBlendMode(BlendMode::Alpha);

    //attemptAddToPrimaryElement();
}

void UITextBlock::init(const StringRef& text)
{
    init();
    setText(text);
}

void UITextBlock::setText(const StringRef& value)
{
	m_text = value;
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

