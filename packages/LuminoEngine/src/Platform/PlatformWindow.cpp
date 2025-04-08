#include "Internal.hpp"
#include <LuminoEngine/Rendering/SurfaceContext.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/detail/PlatformWindowManager.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>

namespace ln {

//==============================================================================
// PlatformWindow

PlatformWindow::PlatformWindow()
    : m_windowManager(nullptr)
    , m_eventListeners()
    , m_dpiFactor(1.0f) {
}

Result_deprecated<> PlatformWindow::init(detail::PlatformWindowManager* windowManager) {
    m_windowManager = windowManager;
    return ok();
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

} // namespace ln
