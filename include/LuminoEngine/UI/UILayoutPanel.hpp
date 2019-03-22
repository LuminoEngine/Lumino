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

protected:
	virtual int getVisualChildrenCount() const override;
	virtual UIElement* getVisualChild(int index) const override;

public:	// TODO: internal
	void addLayoutOwnerLogicalChild(UIElement* element);
	void clearLayoutOwnerLogicalChildren();

private:
	List<Ref<UIElement>> m_layoutOwnerLogicalChildren;
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

class UIStackLayout
	: public UILayoutPanel
{
public:
	static Ref<UIStackLayout> create();

	void setOrientation(Orientation orientation) { m_orientation = orientation; }
	Orientation getOrientation() const { return m_orientation; }

LN_PROTECTED_INTERNAL_ACCESS:
	UIStackLayout();
	virtual ~UIStackLayout();
	void init();

	// UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

private:
	Orientation m_orientation;
};

// TODO: GridLayout https://blog.qt.io/jp/2011/01/06/qml-layout/

} // namespace ln

