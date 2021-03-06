﻿
#include "Internal.hpp"
#include "../Font/FontCore.hpp"
#include "../Font/FontManager.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIIcon.hpp>
#include "UIStyleInstance.hpp"

namespace ln {

//==============================================================================
// UIIcon

LN_OBJECT_IMPLEMENT(UIIcon, UIElement) {}

Ref<UIIcon> UIIcon::loadFontIcon(const StringRef& iconName)
{
	return makeObject<UIIcon>(iconName);
}

Ref<UIIcon> UIIcon::loadFontIcon(const StringRef& iconName, int size)
{
	return makeObject<UIIcon>(iconName, size);
}

Ref<UIIcon> UIIcon::loadFontIcon(const StringRef& iconName, int size, const Color& color)
{
	return makeObject<UIIcon>(iconName, size, color);
}

UIIcon::UIIcon()
	: m_font(nullptr)
	, m_codePoint(0)
{
}

bool UIIcon::init()
{
	if (!UIElement::init()) return false;
	setAlignments(UIHAlignment::Center, UIVAlignment::Center);
	return true;
}

bool UIIcon::init(const StringRef& iconName)
{
	if (!init()) return false;
	setIconName(iconName);
	return true;
}

bool UIIcon::init(const StringRef& iconName, int size)
{
	if (!init()) return false;
	setIconName(iconName);
	setFontSize(size);
	return true;
}

bool UIIcon::init(const StringRef& iconName, int size, const Color& color)
{
	if (!init()) return false;
	setIconName(iconName);
	setFontSize(size);
	setTextColor(color);
	return true;
}

void UIIcon::setIconName(const StringRef& value)
{
    // TODO: size
	m_codePoint = detail::EngineDomain::fontManager()->glyphIconFontManager()->getFontAwesomeCodePoint(value);
}

Size UIIcon::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	// フォントアイコンの場合は setFontSize でサイズを指定できるようにする
	float fontSize = finalStyle()->font->size();

	if (!m_font) {
		m_font = detail::EngineDomain::fontManager()->glyphIconFontManager()->getFontAwesomeFont(u"Reguler", fontSize);
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

