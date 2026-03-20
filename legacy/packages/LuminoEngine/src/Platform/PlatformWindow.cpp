#include "Internal.hpp"
#include <LuminoEngine/Rendering/SurfaceContext.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/FPSController.hpp>
#include <LuminoEngine/Platform/detail/PlatformWindowManager.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>

namespace ln {

//==============================================================================
// PlatformWindow

PlatformWindow::PlatformWindow()
    : m_windowManager(nullptr)
    , m_eventListeners()
    , m_dpiFactor(1.0f)
    , m_surfaceContext()
    , m_fpsController(makeURef<FPSController>()) {
}

PlatformWindow::~PlatformWindow() {
}

Result_deprecated<> PlatformWindow::init(detail::PlatformWindowManager* windowManager) {
    m_windowManager = windowManager;
    return ok();
}

FPSController* PlatformWindow::fpsController() const {
    return m_fpsController;
}

SurfaceContext* PlatformWindow::surfaceContext() const {
    return m_surfaceContext;
}

void PlatformWindow::attachEventListener(IPlatforEventListener* listener) {
    m_eventListeners.add(listener);
}

void PlatformWindow::detachEventListener(IPlatforEventListener* listener) {
    m_eventListeners.remove(listener);
}

bool PlatformWindow::sendEventToAllListener(const PlatformEventArgs& e) {
    for (IPlatforEventListener* listener : m_eventListeners) {
        if (listener->onPlatformEvent(e)) {
            return true;
        }
    }

    // Default event process
    {
        if (e.type == PlatformEventType::close) {
            auto* manager = m_windowManager->manager();
            if (manager->mainWindow() == this) {
                manager->requestQuit();
            }
        }
    }

    return false;
}

MaybeResult PlatformWindow::present() {
    m_surfaceContext->context()->present();
    m_fpsController->process();
    return LN_MAKE_SUCCESS();
}

} // namespace ln
