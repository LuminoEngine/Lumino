
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
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
    if (!UIToggleButton::init()) return false;

	//auto vsm = getVisualStateManager();
	//vsm->registerState(UIVisualStates::CommonStates, UIVisualStates::Pressed);

	m_checkMark = makeObject<UIElement>();
	m_checkMark->setVAlignment(UIVAlignment::Center);
	m_checkMark->addClass(_TT("UICheckBox-CheckMark"));
	m_checkMark->setPartParent(this);
	addVisualChild(m_checkMark);


    m_checkMarkFont = detail::EngineDomain::fontManager()->glyphIconFontManager()->getFontAwesomeFont(_TT("Solid"), 20);
	m_checkMarkCodePoint = 0xF00C;

	m_squareCodePoint = 0xF0C8;

    return true;
}

Size UICheckBox::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	m_checkMark->measureLayout(layoutContext, constraint);
	m_boxSize = m_checkMark->desiredSize();
	//m_boxSize = layoutContext->measureTextSize(m_checkMarkFont, m_squareCodePoint);

	Size baseSize = UIToggleButton::measureOverride(layoutContext, constraint);


	
	Size desiredSize(baseSize.width + m_boxSize.width, std::max(baseSize.height, m_boxSize.height));

	return desiredSize;
}

Size UICheckBox::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	m_checkMark->arrangeLayout(layoutContext, Rect(finalArea.x, finalArea.y, m_boxSize.width, finalArea.height));

	Rect baseArea(m_boxSize.width, 0, finalArea.width - m_boxSize.width, finalArea.height);

	return UIToggleButton::arrangeOverride(layoutContext, baseArea);
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

		//BoxElementShapeBaseStyle baseStyle;
		//baseStyle.baseRect = Rect(0, 0, 20, 20);

		//BoxElementShapeBackgroundStyle backbroundStyle;
		//backbroundStyle.color = Color::Gray;

		//context->drawBoxElement(baseStyle, const BoxElementShapeBackgroundStyle * backbroundStyle = nullptr, const BoxElementShapeBorderStyle * borderStyle = nullptr, const BoxElementShapeShadowStyle * shadowStyle = nullptr);

		

		//context->drawChar(m_squareCodePoint, Color::Gray, m_checkMarkFont);
		//context->drawChar(m_checkMarkCodePoint, Color::Blue, m_checkMarkFont);
	}
}

} // namespace ln

