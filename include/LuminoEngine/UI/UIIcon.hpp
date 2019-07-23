#pragma once
#include "UIElement.hpp"

namespace ln {


class UIIcon
	: public UIElement
{
public:
	void setIconName(const StringRef& value);

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual void onRender(UIRenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	UIIcon();
	void init();

private:
	Ref<Font> m_font;
	uint32_t m_codePoint;
};

} // namespace ln

