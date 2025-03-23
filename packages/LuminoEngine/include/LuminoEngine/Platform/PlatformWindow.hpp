#pragma once
#include <LuminoCore/Geometries/GeometryStructs.hpp>
#include "PlatformEvent.hpp"
#include "../GPU/SwapChain.hpp"

namespace ln {

class PlatformWindow : public Object {
public:
    virtual ~PlatformWindow() = default;
    //virtual void dispose() = 0;

    float dpiFactor() const { return m_dpiFactor; }
    GraphicsContext* graphicsContext() const { return m_graphicsContext; }

    virtual void setWindowTitle(const String& title) = 0;
    virtual void getSize(SizeI* size) = 0;
    virtual void setSize(const SizeI& size) = 0;
    virtual void getFramebufferSize(
        int* width,
        int* height) = 0; // スワップチェインのサイズとするべきサイズ (retina display や dpi スケーリングを考慮)
    virtual void setAllowDragDrop(bool value) = 0;
    virtual bool isAllowDragDrop() const = 0;

    virtual PointI pointFromScreen(const PointI& screenPoint) = 0;
    virtual PointI pointToScreen(const PointI& clientPoint) = 0;

    virtual void grabCursor() = 0;
    virtual void releaseCursor() = 0;

    void attachEventListener(IPlatforEventListener* listener);
    void detachEventListener(IPlatforEventListener* listener);
    bool sendEventToAllListener(const PlatformEventArgs& e); // return : isHandled

    // TODO: intenral
    detail::PlatformWindowManager* windowManager() const { return m_windowManager; }

protected:
    PlatformWindow();
    Result<> init(detail::PlatformWindowManager* windowManager);
    void setDPIFactor(float value) { m_dpiFactor = value; }

private:
    detail::PlatformWindowManager* m_windowManager;
    List<IPlatforEventListener*> m_eventListeners;
    float m_dpiFactor;
    Ref<GraphicsContext> m_graphicsContext;

    friend class detail::PlatformManager;
};

} // namespace ln
