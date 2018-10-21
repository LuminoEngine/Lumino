
#include "Internal.hpp"
#include "EmptyPlatformWindowManager.hpp"
#include "GLFWPlatformWindowManager.hpp"
#include "PlatformManager.hpp"

namespace ln {
namespace detail {

PlatformManager::PlatformManager()
	: m_windowManager()
{
}

void PlatformManager::initialize(const Settings& settings)
{
#ifdef LN_GLFW
	auto windowManager = ln::makeRef<GLFWPlatformWindowManager>();
	windowManager->initialize();
	m_windowManager = windowManager;
#endif

	if (!m_windowManager)
	{
		auto windowManager = ln::makeRef<EmptyPlatformWindowManager>();
		windowManager->initialize();
		m_windowManager = windowManager;
	}

	m_mainWindow = m_windowManager->createWindow(settings.mainWindowSettings);
}

void PlatformManager::dispose()
{
	m_windowManager->dispose();
}

} // namespace detail
} // namespace ln

