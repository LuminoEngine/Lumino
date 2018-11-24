#pragma once
#include "UIElement.hpp"
#include "UILayoutElement.hpp"

namespace ln {

class UILayoutPanel
	: public UIElement
    , public detail::ILayoutElement
{
public:
    UILayoutPanel();
	void initialize();

private:
};

/*
要素を四辺に沿って整列
*/
class UIFrameLayout
    : public UILayoutPanel
{
public:
    static Ref<UIFrameLayout> create();

LN_PROTECTED_INTERNAL_ACCESS:
    UIFrameLayout();
    virtual ~UIFrameLayout();
    void initialize();

    // UIElement interface
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

private:
};

} // namespace ln

