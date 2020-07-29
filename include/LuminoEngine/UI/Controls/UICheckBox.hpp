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
	const String& elementName() const override { static String name = u"UICheckBox"; return name; }
	void onRender(UIRenderingContext* context) override;

private:
	Font* m_checkMarkFont;
	uint32_t m_checkMarkCodePoint;
};

} // namespace ln

