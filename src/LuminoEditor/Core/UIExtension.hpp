#pragma once

namespace ln {

class NavigationMenuItem
	: public UIIcon
{
protected:
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

LN_CONSTRUCT_ACCESS:
    NavigationMenuItem();
	void init();

private:
};

} // namespace ln
