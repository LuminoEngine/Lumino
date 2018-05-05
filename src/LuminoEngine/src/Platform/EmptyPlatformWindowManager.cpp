
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

void EmptyPlatformWindow::initialize(const WindowCreationSettings& settings)
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

void EmptyPlatformWindowManager::initialize()
{
	PlatformWindowManager::initialize();
}

void EmptyPlatformWindowManager::dispose()
{
	PlatformWindowManager::dispose();
}

Ref<PlatformWindow> EmptyPlatformWindowManager::createWindow(const WindowCreationSettings& settings)
{
	auto obj = Ref<EmptyPlatformWindow>::makeRef();
	obj->initialize(settings);
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

