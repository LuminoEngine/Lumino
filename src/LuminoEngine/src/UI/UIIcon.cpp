
#include "Internal.hpp"
#include "../Font/FontManager.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIIcon.hpp>

namespace ln {

//==============================================================================
// UIIcon

UIIcon::UIIcon()
	: m_font(nullptr)
	, m_codePoint(0)
{
}

void UIIcon::init()
{
    UIElement::init();
}

void UIIcon::setIconName(const StringRef& value)
{
	m_font = detail::EngineDomain::fontManager()->glyphIconFontManager()->getFontAwesomeFont(u"Reguler", 20);
	m_codePoint = detail::EngineDomain::fontManager()->glyphIconFontManager()->getFontAwesomeCodePoint(value);
}

Size UIIcon::measureOverride(const Size& constraint)
{
	return Size(20, 20);
}

void UIIcon::onRender(UIRenderingContext* context)
{
	context->drawChar(m_codePoint, finalStyle()->textColor, m_font);
}

} // namespace ln

