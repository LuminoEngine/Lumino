
#ifdef LN_GLFW

#include "Internal.hpp"
#include "../Platform/GLFWPlatformWindowManager.hpp"
#include "GLFWContext.hpp"

namespace ln {
namespace detail {

//=============================================================================
// GLFWSwapChain

GLFWSwapChain::GLFWSwapChain(GLFWPlatformWindow* window)
	: m_window(window)
{
}

//=============================================================================
// GLFWContext

void GLFWContext::initialize(PlatformWindow* window)
{
	auto* glfwWindow = dynamic_cast<GLFWPlatformWindow*>(window);
	LN_CHECK(glfwWindow);
	m_mainWindow = glfwWindow;

	printf("glfwMakeContextCurrent:%p\n", m_mainWindow->glfwWindow());
	glfwMakeContextCurrent(m_mainWindow->glfwWindow());
}

Ref<GLSwapChain> GLFWContext::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	auto* glfwWindow = dynamic_cast<GLFWPlatformWindow*>(window);
	LN_CHECK(glfwWindow);

	return makeRef<GLFWSwapChain>(glfwWindow);
}

void GLFWContext::makeCurrent(GLSwapChain* swapChain)
{
	printf("glfwMakeContextCurrent:%p\n", static_cast<GLFWSwapChain*>(swapChain)->window()->glfwWindow());
	glfwMakeContextCurrent(static_cast<GLFWSwapChain*>(swapChain)->window()->glfwWindow());
}

void GLFWContext::swap(GLSwapChain* swapChain)
{
	glfwSwapBuffers(static_cast<GLFWSwapChain*>(swapChain)->window()->glfwWindow());
}

} // namespace detail
} // namespace ln

#endif
