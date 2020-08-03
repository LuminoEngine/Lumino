#pragma once
#include "UIContainerElement.hpp"
#include "UIAdorner.hpp"

namespace ln {
class UIPopupAdorner;

enum class PlacementMode
{
    Bottom,
	Overray,	// 中央に重ねる
};


// TODO: レイアウトは行わず、イベントルーティングだけ行うフラグとか用意してみる
class UIPopup
    : public UIContainerElement
{
public:
    void setPlacementTarget(UIElement* target);
	UIElement* placementTarget() const;

    void setPlacementMode(PlacementMode mode);
	PlacementMode placementMode() const { return m_placementMode; }

    bool isOpend() const { return m_opend; }

    void open();
    void close();

protected:
    virtual const String& elementName() const override { static String name = u"UIPopup"; return name; }
    virtual void onRoutedEvent(UIEventArgs* e) override;
	//virtual bool onHitTest(const Point& frameClientPosition) override;
	//virtual void deactivateInternal()
	//{
	//	UIContainerElement::deactivateInternal();
	//}


LN_CONSTRUCT_ACCESS:
    UIPopup();
    void init();

private:
    Ref<UIElement> m_placementTarget;
    PlacementMode m_placementMode;
    Ref<UIAdorner> m_adorner;
    bool m_opend;

    friend class UIPopupAdorner;
};

// UIAdorner は target にぴったり重なるように layout されるが、
// UIPopupAdorner は PlacementMode をもとに、その finalRect からの相対位置を使って UIPopup を layout する。
class UIPopupAdorner
    : public UIAdorner
{
public:
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
    virtual void onUpdateLayout(UILayoutContext* layoutContext) override;
    virtual void render(UIRenderingContext* context, const Matrix& parentTransform) override;

LN_CONSTRUCT_ACCESS:
    UIPopupAdorner();
    void init(UIElement* adornedElement, UIPopup* popup);

private:
    Ref<UIElement> m_adornedElement;
    Ref<UIPopup> m_popup;
};

} // namespace ln

