
#include "Internal.hpp"
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
	auto windowManager = Ref<GLFWPlatformWindowManager>::makeRef();
	windowManager->initialize();

	m_windowManager = windowManager;
}

void PlatformManager::dispose()
{
	m_windowManager->dispose();
}

} // namespace detail
} // namespace ln

