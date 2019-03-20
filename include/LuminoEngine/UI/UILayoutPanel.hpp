#pragma once
#include "UIElement.hpp"
#include "UILayoutElement.hpp"

namespace ln {

class UILayoutPanel
	: public UIElement
{
public:
    UILayoutPanel();
	void init();

private:
};

/*
要素を四辺に沿って整列
*/
class UIFrameLayout	// TODO: BorderLayout の方がいいかも https://doc.qt.io/qt-5/qtwidgets-layouts-borderlayout-example.html
    : public UILayoutPanel
{
public:
    static Ref<UIFrameLayout> create();

	// TODO: internal
	static Size staticMeasureOverride(UIElement* ownerElement, const Size& constraint);
	static Size staticArrangeOverride(UIElement* ownerElement, const Size& finalSize);

LN_PROTECTED_INTERNAL_ACCESS:
    UIFrameLayout();
    virtual ~UIFrameLayout();
    void init();

    // UIElement interface
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

private:
};

} // namespace ln

