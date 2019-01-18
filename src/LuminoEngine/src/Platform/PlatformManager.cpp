
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
	if (m_mainWindow) {
		m_windowManager->destroyWindow(m_mainWindow);
		m_mainWindow = nullptr;
	}
	if (m_windowManager) {
		m_windowManager->dispose();
		m_windowManager = nullptr;
	}
}

} // namespace detail
} // namespace ln

