
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

Ref<GLSwapChain> GLFWContext::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	auto* glfwWindow = dynamic_cast<GLFWPlatformWindow*>(window);
	LN_CHECK(glfwWindow);

	return makeRef<GLFWSwapChain>(glfwWindow);
}

void GLFWContext::makeCurrent(GLSwapChain* swapChain)
{
	glfwMakeContextCurrent(static_cast<GLFWSwapChain*>(swapChain)->window()->glfwWindow());
}

void GLFWContext::swap(GLSwapChain* swapChain)
{
	glfwSwapBuffers(static_cast<GLFWSwapChain*>(swapChain)->window()->glfwWindow());
}

} // namespace detail
} // namespace ln

