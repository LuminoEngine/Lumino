
#include "Internal.hpp"
#include "EmptyPlatformWindowManager.hpp"

namespace ln {
namespace detail {
	
//=============================================================================
// EmptyPlatformWindow

EmptyPlatformWindow::EmptyPlatformWindow()
{
}

EmptyPlatformWindow::~EmptyPlatformWindow()
{
}

void EmptyPlatformWindow::init(const WindowCreationSettings& settings)
{
}

void EmptyPlatformWindow::dispose()
{
}

//=============================================================================
// EmptyPlatformWindowManager

EmptyPlatformWindowManager::EmptyPlatformWindowManager()
{
}

EmptyPlatformWindowManager::~EmptyPlatformWindowManager()
{
}

void EmptyPlatformWindowManager::init()
{
	PlatformWindowManager::init();
}

void EmptyPlatformWindowManager::dispose()
{
	PlatformWindowManager::dispose();
}

Ref<PlatformWindow> EmptyPlatformWindowManager::createWindow(const WindowCreationSettings& settings)
{
	auto obj = ln::makeRef<EmptyPlatformWindow>();
	obj->init(settings);
	return obj;
}

void EmptyPlatformWindowManager::destroyWindow(PlatformWindow* window)
{
	if (LN_REQUIRE(window)) return;
	window->dispose();
}

void EmptyPlatformWindowManager::processSystemEventQueue()
{
}

} // namespace detail
} // namespace ln

