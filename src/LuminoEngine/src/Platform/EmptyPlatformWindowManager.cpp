
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

Ref<PlatformWindow> EmptyPlatformWindowManager::createWindow(const WindowCreationSettings& settings, PlatformWindow* mainWindow)
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

} // namespace detail
} // namespace ln

