#include "Internal.hpp"
#include <LuminoEngine/UI/RoutingRenderView.hpp>

namespace ln {

//==============================================================================
// RoutingRenderView

RoutingRenderView::RoutingRenderView() {
}

Ref<EventConnection> RoutingRenderView::connectOnUIEvent(Ref<UIGeneralEventHandler> handler) {
    return m_onUIEvent.connect(handler);
}

void RoutingRenderView::updateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle) {
    onUpdateUIStyle(styleContext, parentFinalStyle);
}

void RoutingRenderView::updateUILayout(UILayoutContext* layoutContext) {
    onUpdateUILayout(layoutContext);
}

void RoutingRenderView::onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) {
}

void RoutingRenderView::onUpdateUILayout(UILayoutContext* layoutContext) {
}

UIElement* RoutingRenderView::onLookupMouseHoverElement(const Point& framewindowClientPoint) {
    return nullptr;
}

void RoutingRenderView::onRoutedEvent(UIEventArgs* e) {
    m_onUIEvent.raise(e);
}

} // namespace ln

