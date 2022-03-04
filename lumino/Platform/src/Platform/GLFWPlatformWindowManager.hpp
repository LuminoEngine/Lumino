#pragma once
#ifdef LN_GLFW
#include <LuminoPlatform/detail/PlatformWindowManager.hpp>
#include "OpenGLContext.hpp"

struct GLFWwindow;

namespace ln {
namespace detail {
class GLFWPlatformWindow;
class GLFWPlatformWindowManager;

class GLFWContext
	: public OpenGLContext
{
public:
	GLFWContext(GLFWPlatformWindow* mainWindow);

	GLFWPlatformWindow* mainWindow() const { return m_mainWindow; }
	void makeCurrentMain() override;
	void makeCurrent(PlatformWindow* window) override;

	//virtual Ref<GLSwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	//virtual void makeCurrent(GLSwapChain* swapChain) override;
	//virtual void swap(GLSwapChain* swapChain) override;

private:
	GLFWPlatformWindow* m_mainWindow;
};

class GLFWPlatformWindow
	: public PlatformWindow
{
public:
	GLFWPlatformWindow();
	virtual ~GLFWPlatformWindow();

    Result init(GLFWPlatformWindowManager* windowManager, const WindowCreationSettings& settings, GLFWContext* sharedContext);
	void dispose();
    void setWindowTitle(const String& title) override;
	void getSize(SizeI* size) override;
	void setSize(const SizeI& size) override;
	void getFramebufferSize(int* width, int* height) override;
	void setAllowDragDrop(bool value) override;
	bool isAllowDragDrop() const override;
	PointI pointFromScreen(const PointI& screenPoint) override;
	PointI pointToScreen(const PointI& clientPoint) override;
	void grabCursor() override;
	void releaseCursor() override;

	GLFWwindow* glfwWindow() const { return m_glfwWindow; }

	void* getWin32Window() const;

private:
	static void window_close_callback(GLFWwindow* window);
	static void window_focus_callback(GLFWwindow* window, int focused);	// window activate/diactivate
	static void window_framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_char_callback(GLFWwindow* window, unsigned int ch);
	static void window_mouseButton_callback(GLFWwindow* window, int button, int action, int mods);
	static void window_mousePos_callback(GLFWwindow* window, double xpos, double ypos);
    static void window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void window_drop_callback(GLFWwindow* window, int count, const char** paths);

	static ModifierKeys glfwKeyModToLNKeyMod(int mods);

	GLFWwindow* m_glfwWindow;
};

class GLFWPlatformWindowManager
	: public PlatformWindowManager
{
public:
	GLFWPlatformWindowManager(PlatformManager* manager);
	virtual ~GLFWPlatformWindowManager();

    Result init();
	void dispose() override;
	Ref<PlatformWindow> createMainWindow(const WindowCreationSettings& settings) override;
	Ref<PlatformWindow> createSubWindow(const WindowCreationSettings& settings) override;
	void destroyWindow(PlatformWindow* window) override;
	void processSystemEventQueue(EventProcessingMode mode) override;
	OpenGLContext* getOpenGLContext() const override;

private:
	Ref<GLFWContext> m_glContext;
};

} // namespace detail
} // namespace ln

#endif
