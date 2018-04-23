#pragma once

#include "OpenGLDeviceContext.hpp"

namespace ln {
namespace detail {
class GLFWPlatformWindow;

class GLFWSwapChain
	: public GLSwapChain
{
public:
	GLFWSwapChain(GLFWPlatformWindow* window);
	virtual ~GLFWSwapChain() = default;

	GLFWPlatformWindow* window() const { return m_window; }

private:
	GLFWPlatformWindow* m_window;
};

class GLFWContext
	: public GLContext
{
public:
	GLFWContext() = default;
	virtual ~GLFWContext() = default;

	virtual Ref<GLSwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	virtual void makeCurrent(GLSwapChain* swapChain) override;
	virtual void swap(GLSwapChain* swapChain) override;
};

} // namespace detail
} // namespace ln

