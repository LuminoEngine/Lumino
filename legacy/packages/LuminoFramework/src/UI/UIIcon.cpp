
#include "Internal.hpp"
#include <LuminoEngine/Font/detail/FontCore.hpp>
#include <LuminoEngine/Font/detail/FontManager.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoFramework/UI/UIStyle.hpp>
#include <LuminoFramework/UI/UIRenderingContext.hpp>
#include <LuminoFramework/UI/UIIcon.hpp>
#include "UIStyleInstance.hpp"

namespace ln {

//==============================================================================
// UIIcon

LN_OBJECT_IMPLEMENT(UIIcon, UIElement) {}

Ref<UIIcon> UIIcon::loadFontIcon(const StringView& iconName)
{
	return makeObject_deprecated<UIIcon>(iconName);
}

Ref<UIIcon> UIIcon::loadFontIcon(const StringView& iconName, int size)
{
	return makeObject_deprecated<UIIcon>(iconName, size);
}

Ref<UIIcon> UIIcon::loadFontIcon(const StringView& iconName, int size, const Color& color)
{
	return makeObject_deprecated<UIIcon>(iconName, size, color);
}

UIIcon::UIIcon()
	: m_font(nullptr)
	, m_codePoint(0)
{
}

bool UIIcon::init()
{
	if (!UIElement::init()) return false;
	setAlignments(UIAlignment::Center);
	return true;
}

bool UIIcon::init(const StringView& iconName)
{
	if (!init()) return false;
	setIconName(iconName);
	return true;
}

bool UIIcon::init(const StringView& iconName, int size)
{
	if (!init()) return false;
	setIconName(iconName);
	setFontSize(size);
	return true;
}

bool UIIcon::init(const StringView& iconName, int size, const Color& color)
{
	if (!init()) return false;
	setIconName(iconName);
	setFontSize(size);
	setTextColor(color);
	return true;
}

void UIIcon::setIconName(const StringView& value)
{
    // TODO: size
	m_codePoint = detail::FontManager::instance()->glyphIconFontManager()->getFontAwesomeCodePoint(value);
}

Size UIIcon::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	// フォントアイコンの場合は setFontSize でサイズを指定できるようにする
	float fontSize = finalStyle()->font->size();

	if (!m_font) {
		m_font = detail::FontManager::instance()->glyphIconFontManager()->getFontAwesomeFont(_TT("Reguler"), fontSize);
	}
	if (m_font->size() != fontSize) {
		m_font->setSize(fontSize);
	}

	detail::FontGlobalMetrics gm;
	auto fc = detail::FontHelper::resolveFontCore(m_font, layoutContext->dpiScale());
	fc->getGlobalMetrics(&gm);

	Size areaSize = Size(gm.lineSpace, gm.lineSpace);

	detail::FontGlyphMetrics metrics;
	fc->getGlyphMetrics(m_codePoint, &metrics);
	m_renderOffset.x = (areaSize.width - metrics.size.width) / 2;
	m_renderOffset.y = (areaSize.height - metrics.size.height) / 2;

	return areaSize;
}

void UIIcon::onRender(UIRenderingContext* context)
{
	Color color = finalStyle()->textColor;
	if (!isEnabled()) {
		color.a = 0.5f;
	}

	context->drawChar(m_codePoint, color, m_font, Matrix::makeTranslation(m_renderOffset.x, m_renderOffset.y, 0));
}

} // namespace ln

