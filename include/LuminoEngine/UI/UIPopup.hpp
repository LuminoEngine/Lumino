#pragma once
#include "UIContainerElement.hpp"
#include "UIAdorner.hpp"

namespace ln {
class UIPopupAdorner;

enum class PlacementMode
{
    Bottom,
};


// TODO: レイアウトや描画は行わず、イベントルーティングだけ行うフラグとか用意してみる
class UIPopup
    : public UIContainerElement
{
public:
    void setPlacementTarget(UIElement* target);
    void setPlacementMode(PlacementMode mode);

    bool isOpend() const { return m_opend; }

    void open();
    void close();

protected:
    virtual const String& elementName() const  override { static String name = u"UIPopup"; return name; }

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

class UIPopupAdorner
    : public UIAdorner
{
public:
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;
    virtual void onUpdateLayout(const Rect& finalGlobalRect) override;
    virtual void render(UIRenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    UIPopupAdorner();
    void init(UIElement* adornedElement, UIPopup* popup);

private:
    Ref<UIElement> m_adornedElement;
    Ref<UIElement> m_popup;
};

} // namespace ln

