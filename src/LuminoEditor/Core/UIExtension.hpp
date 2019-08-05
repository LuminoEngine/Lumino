#pragma once

namespace ln {

class NavigationMenuItem
	: public UIIcon
{
protected:
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
    NavigationMenuItem();
	void init();

private:
};

} // namespace ln
