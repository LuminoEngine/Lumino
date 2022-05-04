#pragma once
#include <LuminoGraphics/Rendering/RenderView.hpp>
#include "UIEvents.hpp"

namespace ln {
class UILayoutContext;
class UIViewport;

// ビュー内への UIEvent 通知機能を持つ RenderView.
// 元々は RenderView の機能として持っていたが、Rendering module と UI module の相互依存関係を無くすため、こちらに移動してきた。
class RoutingRenderView : public RenderView {
public:
    RoutingRenderView();

    UIViewport* viewport() const { return m_parentViewport; }

    // LN_METHOD(Event)
    Ref<EventConnection> connectOnUIEvent(Ref<UIGeneralEventHandler> handler);

    void updateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle);
    void updateUILayout(UILayoutContext* layoutContext);

protected:
    virtual void onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle);
    virtual void onUpdateUILayout(UILayoutContext* layoutContext);
    virtual UIElement* onLookupMouseHoverElement(const Point& framewindowClientPoint);
    virtual void onRoutedEvent(UIEventArgs* e);

private:
    UIViewport* m_parentViewport = nullptr;
    Event<UIGeneralEventHandler> m_onUIEvent;

    friend class UIViewport;
};

} // namespace ln

