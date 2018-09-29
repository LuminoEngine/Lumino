#pragma once

#ifdef LN_GLFW

//#include <GLFW/glfw3.h>
#include "PlatformWindowManager.hpp"

struct GLFWwindow;

namespace ln {
namespace detail {

class GLFWPlatformWindow
	: public PlatformWindow
{
public:
	GLFWPlatformWindow();
	virtual ~GLFWPlatformWindow();

	void initialize(const WindowCreationSettings& settings);
	virtual void dispose() override;
	virtual void getSize(SizeI* size) override;
	virtual void getFramebufferSize(int* width, int* height) override;
	virtual PointI pointFromScreen(const PointI& screenPoint) override;
	virtual PointI pointToScreen(const PointI& clientPoint) override;

	GLFWwindow* glfwWindow() const { return m_glfwWindow; }

	void* getWin32Window() const;

private:
	static void window_close_callback(GLFWwindow* window);
	static void window_focus_callback(GLFWwindow* window, int focused);	// window activate/diactivate
	static void window_framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void window_mouseButton_callback(GLFWwindow* window, int button, int action, int mods);
	static void window_mousePos_callback(GLFWwindow* window, double xpos, double ypos);
	static ModifierKeys glfwKeyModToLNKeyMod(int mods);

	GLFWwindow* m_glfwWindow;
};

class GLFWPlatformWindowManager
	: public PlatformWindowManager
{
public:
	GLFWPlatformWindowManager();
	virtual ~GLFWPlatformWindowManager();

	void initialize();
	virtual void dispose() override;
	virtual Ref<PlatformWindow> createWindow(const WindowCreationSettings& settings) override;
	virtual void destroyWindow(PlatformWindow* window) override;
	virtual void processSystemEventQueue() override;
};

} // namespace detail
} // namespace ln

#endif
