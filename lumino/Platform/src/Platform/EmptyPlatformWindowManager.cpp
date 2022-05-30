
#include "Internal.hpp"
#include "EmptyPlatformWindowManager.hpp"

namespace ln {

//==============================================================================
// ExternalProxyPlatformWindow

ExternalProxyPlatformWindow::ExternalProxyPlatformWindow() {
}

Result ExternalProxyPlatformWindow::init(const WindowCreationSettings& settings) {
    return ok();
}

void ExternalProxyPlatformWindow::setAllowDragDrop(bool value) {
    LN_NOTIMPLEMENTED();
}

bool ExternalProxyPlatformWindow::isAllowDragDrop() const {
    return false;
}

void ExternalProxyPlatformWindow::injectSizeChanged(int width, int height) {
    m_size.width = width;
    m_size.height = height;
    sendEventToAllListener(PlatformEventArgs::makeWindowSizeChangedEvent(this, width, height));
}

namespace detail {

//==============================================================================
// EmptyPlatformWindowManager

EmptyPlatformWindowManager::EmptyPlatformWindowManager(PlatformManager* manager)
    : PlatformWindowManager(manager) {
}

EmptyPlatformWindowManager::~EmptyPlatformWindowManager() {
}

Result EmptyPlatformWindowManager::init() {
    return ok();
}

void EmptyPlatformWindowManager::dispose() {
}

Ref<PlatformWindow> EmptyPlatformWindowManager::createWindow(const WindowCreationSettings& settings, PlatformWindow* mainWindow) {
    auto ptr = ln::makeRef<ExternalProxyPlatformWindow>();
    if (!ptr->init(settings)) {
        return nullptr;
    }
    return ptr;
}

void EmptyPlatformWindowManager::destroyWindow(PlatformWindow* window) {
}

void EmptyPlatformWindowManager::processSystemEventQueue(EventProcessingMode mode) {
}

OpenGLContext* EmptyPlatformWindowManager::getOpenGLContext() const {
    return nullptr;
}

} // namespace detail
} // namespace ln
