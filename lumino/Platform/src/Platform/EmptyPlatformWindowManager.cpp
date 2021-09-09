
#include "Internal.hpp"
#include "EmptyPlatformWindowManager.hpp"

namespace ln {
namespace detail {
	
//=============================================================================
// EmptyPlatformWindow

EmptyPlatformWindow::EmptyPlatformWindow()
{
}

Result EmptyPlatformWindow::init(const WindowCreationSettings& settings)
{
    return true;
}

void EmptyPlatformWindow::setAllowDragDrop(bool value)
{
    LN_NOTIMPLEMENTED();
}

bool EmptyPlatformWindow::isAllowDragDrop() const
{
    return false;
}

//=============================================================================
// EmptyPlatformWindowManager

EmptyPlatformWindowManager::EmptyPlatformWindowManager(PlatformManager* manager)
    : PlatformWindowManager(manager)
{
}

EmptyPlatformWindowManager::~EmptyPlatformWindowManager()
{
}

Result EmptyPlatformWindowManager::init()
{
    return true;
}

void EmptyPlatformWindowManager::dispose()
{
}

Ref<PlatformWindow> EmptyPlatformWindowManager::createMainWindow(const WindowCreationSettings& settings)
{
    auto ptr = ln::makeRef<EmptyPlatformWindow>();
    if (!ptr->init(settings)) {
        return nullptr;
    }
    return ptr;
}

Ref<PlatformWindow> EmptyPlatformWindowManager::createSubWindow(const WindowCreationSettings& settings)
{
    auto ptr = ln::makeRef<EmptyPlatformWindow>();
    if (!ptr->init(settings)) {
        return nullptr;
    }
    return ptr;
}

void EmptyPlatformWindowManager::destroyWindow(PlatformWindow* window)
{
}

void EmptyPlatformWindowManager::processSystemEventQueue(EventProcessingMode mode)
{
}

OpenGLContext* EmptyPlatformWindowManager::getOpenGLContext() const
{
    return nullptr;
}

} // namespace detail
} // namespace ln

