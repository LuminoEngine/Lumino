#pragma once
#include "UIButton.hpp"

namespace ln {

class UICheckBox
	: public UIToggleButton
{
	LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
	UICheckBox();
	bool init();

protected:
	const String& elementName() const override { static String name = _TT("UICheckBox"); return name; }
	Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
	void onRender(UIRenderingContext* context) override;

private:
	Ref<UIElement> m_checkMark;
	Font* m_checkMarkFont;
	uint32_t m_checkMarkCodePoint;
	uint32_t m_squareCodePoint;
	Size m_boxSize;
};

} // namespace ln

