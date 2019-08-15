#pragma once
#include "UIContainerElement.hpp"

namespace ln {
class UIAdorner;

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

LN_CONSTRUCT_ACCESS:
    UIPopup();
    void init();

private:
    Ref<UIElement> m_placementTarget;
    PlacementMode m_placementMode;
    Ref<UIAdorner> m_adorner;
    bool m_opend;
};

} // namespace ln

