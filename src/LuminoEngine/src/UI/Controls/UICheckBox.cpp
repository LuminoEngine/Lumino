
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/Controls/UICheckBox.hpp>
#include "../../Font/FontManager.hpp"

namespace ln {

//==============================================================================
// UICheckBox

LN_OBJECT_IMPLEMENT(UICheckBox, UIToggleButton) {}

UICheckBox::UICheckBox()
    : m_checkMarkFont(nullptr)
    , m_checkMarkCodePoint(0)
{
}

bool UICheckBox::init()
{
    if (!UIControl::init()) return false;

    m_checkMarkFont = detail::EngineDomain::fontManager()->glyphIconFontManager()->getFontAwesomeFont(u"Reguler", 20);
    m_checkMarkCodePoint = 0xF00C;//detail::EngineDomain::fontManager()->glyphIconFontManager()->getFontAwesomeCodePoint(value);

    return true;
}

Size UICheckBox::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	return UIToggleButton::measureOverride(layoutContext, constraint);
}

Size UICheckBox::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	return UIToggleButton::arrangeOverride(layoutContext, finalArea);
}

void UICheckBox::onRender(UIRenderingContext* context)
{
	if (m_checkMarkFont && m_checkMarkCodePoint > 0) {
		//Size areaSize;
		//areaSize.width = slotRect.width - (m_margin.left + m_margin.right);
		//areaSize.height = slotRect.height - (m_margin.top + m_margin.bottom);

		//Size desiredSize = context->measureTextSize(m_font, m_codePoint);
		////desiredSize.width = Math::isNaN(m_width) ? 0.0f : m_width;
		////desiredSize.height = Math::isNaN(m_height) ? 0.0f : m_height;

		//Rect localRect;
		//detail::LayoutHelper::adjustHorizontalAlignment(areaSize, desiredSize, m_width, m_hAlignment, &localRect);
		//detail::LayoutHelper::adjustVerticalAlignment(areaSize, desiredSize, m_height, m_vAlignment, &localRect);

		//Matrix transform = Matrix::makeTranslation(localRect.x, localRect.y, 0);

		//context->setTransfrom(transform);
		context->drawChar(m_checkMarkCodePoint, Color::Blue, m_checkMarkFont);
	}
}

} // namespace ln

