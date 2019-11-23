﻿#pragma once
#ifdef LN_GLFW

#include "OpenGLDeviceContext.hpp"

namespace ln {
namespace detail {
class GLFWPlatformWindow;

class GLFWSwapChain
	: public GLSwapChain
{
public:
	GLFWSwapChain(OpenGLDevice* device, GLFWPlatformWindow* window);
	virtual ~GLFWSwapChain() = default;

	GLFWPlatformWindow* window() const { return m_window; }

	virtual void getBackendBufferSize(SizeI* outSize) override;

private:
	GLFWPlatformWindow* m_window;
};

class GLFWContext
	: public GLContext
{
public:
	GLFWContext() = default;
	virtual ~GLFWContext() = default;
	void init(OpenGLDevice* device, PlatformWindow* window);

	virtual Ref<GLSwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	virtual void makeCurrent(GLSwapChain* swapChain) override;
	virtual void swap(GLSwapChain* swapChain) override;

private:
	OpenGLDevice* m_device;
	GLFWPlatformWindow* m_mainWindow;
};

} // namespace detail
} // namespace ln

#endif
