
#ifdef LN_GLFW


#include "Internal.hpp"
#include <GLFW/glfw3.h>
#if defined(LN_OS_WIN32)
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <GLFW/glfw3native.h>
#	include "Windows/Win32PlatformWindowManager.hpp"
#elif defined(LN_OS_MAC)
#	define GLFW_EXPOSE_NATIVE_COCOA
#	include <GLFW/glfw3native.h>
#endif
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include "PlatformManager.hpp"
#include "GLFWPlatformWindowManager.hpp"

// https://github.com/glfw/glfw/issues/310
static void glfwSetWindowCenter(GLFWwindow* window) {
    // Get window position and size
    int window_x, window_y;
    glfwGetWindowPos(window, &window_x, &window_y);

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    // Halve the window size and use it to adjust the window position to the center of the window
    window_width /= 2;
    window_height /= 2;

    window_x += window_width;
    window_y += window_height;

    // Get the list of monitors
    int monitors_length;
    GLFWmonitor **monitors = glfwGetMonitors(&monitors_length);

    if (monitors == NULL) {
        // Got no monitors back
        return;
    }

    // Figure out which monitor the window is in
    GLFWmonitor *owner = NULL;
    int owner_x, owner_y, owner_width, owner_height;

    for (int i = 0; i < monitors_length; i++) {
        // Get the monitor position
        int monitor_x, monitor_y;
        glfwGetMonitorPos(monitors[i], &monitor_x, &monitor_y);

        // Get the monitor size from its video mode
        int monitor_width, monitor_height;
        GLFWvidmode *monitor_vidmode = (GLFWvidmode*)glfwGetVideoMode(monitors[i]);

        if (monitor_vidmode == NULL) {
            // Video mode is required for width and height, so skip this monitor
            continue;

        }
        else {
            monitor_width = monitor_vidmode->width;
            monitor_height = monitor_vidmode->height;
        }

        // Set the owner to this monitor if the center of the window is within its bounding box
        if ((window_x > monitor_x && window_x < (monitor_x + monitor_width)) && (window_y > monitor_y && window_y < (monitor_y + monitor_height))) {
            owner = monitors[i];

            owner_x = monitor_x;
            owner_y = monitor_y;

            owner_width = monitor_width;
            owner_height = monitor_height;
        }
    }

    if (owner != NULL) {
        // Set the window position to the center of the owner monitor
        glfwSetWindowPos(window, owner_x + (owner_width * 0.5) - window_width, owner_y + (owner_height * 0.5) - window_height);
    }
}

namespace ln {
namespace detail {

//=============================================================================
// GLFWContext

GLFWContext::GLFWContext(GLFWPlatformWindow* mainWindow)
	: m_mainWindow(mainWindow)
{
	assert(m_mainWindow);
}

//SizeI GLFWContext::getBackendBufferSize() const = 0
//{
//}

void GLFWContext::makeCurrentMain()
{
	makeCurrent(m_mainWindow);
}

void GLFWContext::makeCurrent(PlatformWindow* window)
{
	if (window) {
		const auto w = static_cast<GLFWPlatformWindow*>(window);
		glfwMakeContextCurrent(w->glfwWindow());
	}
	else {
		glfwMakeContextCurrent(nullptr);
	}
}

//=============================================================================
// GLFWPlatformWindow

static Keys GLFWKeyToLNKey[GLFW_KEY_LAST];
static void initKeyTable()
{
	static bool mInitedKeyTable = false;
	if ( mInitedKeyTable ) return;

	GLFWKeyToLNKey[GLFW_KEY_A] = Keys::A;
	GLFWKeyToLNKey[GLFW_KEY_B] = Keys::B;
	GLFWKeyToLNKey[GLFW_KEY_C] = Keys::C;
	GLFWKeyToLNKey[GLFW_KEY_D] = Keys::D;
	GLFWKeyToLNKey[GLFW_KEY_E] = Keys::E;
	GLFWKeyToLNKey[GLFW_KEY_F] = Keys::F;
	GLFWKeyToLNKey[GLFW_KEY_G] = Keys::G;
	GLFWKeyToLNKey[GLFW_KEY_H] = Keys::H;
	GLFWKeyToLNKey[GLFW_KEY_I] = Keys::I;
	GLFWKeyToLNKey[GLFW_KEY_J] = Keys::J;
	GLFWKeyToLNKey[GLFW_KEY_K] = Keys::K;
	GLFWKeyToLNKey[GLFW_KEY_L] = Keys::L;
	GLFWKeyToLNKey[GLFW_KEY_M] = Keys::M;
	GLFWKeyToLNKey[GLFW_KEY_N] = Keys::N;
	GLFWKeyToLNKey[GLFW_KEY_O] = Keys::O;
	GLFWKeyToLNKey[GLFW_KEY_P] = Keys::P;
	GLFWKeyToLNKey[GLFW_KEY_Q] = Keys::Q;
	GLFWKeyToLNKey[GLFW_KEY_R] = Keys::R;
	GLFWKeyToLNKey[GLFW_KEY_S] = Keys::S;
	GLFWKeyToLNKey[GLFW_KEY_T] = Keys::T;
	GLFWKeyToLNKey[GLFW_KEY_U] = Keys::U;
	GLFWKeyToLNKey[GLFW_KEY_V] = Keys::V;
	GLFWKeyToLNKey[GLFW_KEY_W] = Keys::W;
	GLFWKeyToLNKey[GLFW_KEY_X] = Keys::X;
	GLFWKeyToLNKey[GLFW_KEY_Y] = Keys::Y;
	GLFWKeyToLNKey[GLFW_KEY_Z] = Keys::Z;

	GLFWKeyToLNKey[GLFW_KEY_0] = Keys::D0;
	GLFWKeyToLNKey[GLFW_KEY_1] = Keys::D1;
	GLFWKeyToLNKey[GLFW_KEY_2] = Keys::D2;
	GLFWKeyToLNKey[GLFW_KEY_3] = Keys::D3;
	GLFWKeyToLNKey[GLFW_KEY_4] = Keys::D4;
	GLFWKeyToLNKey[GLFW_KEY_5] = Keys::D5;
	GLFWKeyToLNKey[GLFW_KEY_6] = Keys::D6;
	GLFWKeyToLNKey[GLFW_KEY_7] = Keys::D7;
	GLFWKeyToLNKey[GLFW_KEY_8] = Keys::D8;
	GLFWKeyToLNKey[GLFW_KEY_9] = Keys::D9;

	GLFWKeyToLNKey[GLFW_KEY_F1] = Keys::F1;
	GLFWKeyToLNKey[GLFW_KEY_F2] = Keys::F2;
	GLFWKeyToLNKey[GLFW_KEY_F3] = Keys::F3;
	GLFWKeyToLNKey[GLFW_KEY_F4] = Keys::F4;
	GLFWKeyToLNKey[GLFW_KEY_F5] = Keys::F5;
	GLFWKeyToLNKey[GLFW_KEY_F6] = Keys::F6;
	GLFWKeyToLNKey[GLFW_KEY_F7] = Keys::F7;
	GLFWKeyToLNKey[GLFW_KEY_F8] = Keys::F8;
	GLFWKeyToLNKey[GLFW_KEY_F9] = Keys::F9;
	GLFWKeyToLNKey[GLFW_KEY_F10] = Keys::F10;
	GLFWKeyToLNKey[GLFW_KEY_F11] = Keys::F11;
	GLFWKeyToLNKey[GLFW_KEY_F12] = Keys::F12;

	GLFWKeyToLNKey[GLFW_KEY_SPACE] = Keys::Space;
	GLFWKeyToLNKey[GLFW_KEY_ESCAPE] = Keys::Escape;
	GLFWKeyToLNKey[GLFW_KEY_UP] = Keys::Up;
	GLFWKeyToLNKey[GLFW_KEY_DOWN] = Keys::Down;
	GLFWKeyToLNKey[GLFW_KEY_LEFT] = Keys::Left;
	GLFWKeyToLNKey[GLFW_KEY_RIGHT] = Keys::Right;
	GLFWKeyToLNKey[GLFW_KEY_LEFT_SHIFT] = Keys::LShift;
	GLFWKeyToLNKey[GLFW_KEY_RIGHT_SHIFT] = Keys::RShift;
	GLFWKeyToLNKey[GLFW_KEY_LEFT_CONTROL] = Keys::LCtrl;
	GLFWKeyToLNKey[GLFW_KEY_RIGHT_CONTROL] = Keys::RCtrl;
	GLFWKeyToLNKey[GLFW_KEY_LEFT_ALT] = Keys::LAlt;
	GLFWKeyToLNKey[GLFW_KEY_RIGHT_ALT] = Keys::RAlt;
	GLFWKeyToLNKey[GLFW_KEY_TAB] = Keys::Tab;
	GLFWKeyToLNKey[GLFW_KEY_ENTER] = Keys::Enter;
	GLFWKeyToLNKey[GLFW_KEY_BACKSPACE] = Keys::BackSpace;
	GLFWKeyToLNKey[GLFW_KEY_INSERT] = Keys::Insert;
	GLFWKeyToLNKey[GLFW_KEY_DELETE] = Keys::Delete;
	GLFWKeyToLNKey[GLFW_KEY_PAGE_UP] = Keys::PageUp;
	GLFWKeyToLNKey[GLFW_KEY_PAGE_DOWN] = Keys::PageDown;
	GLFWKeyToLNKey[GLFW_KEY_HOME] = Keys::Home;
	GLFWKeyToLNKey[GLFW_KEY_END] = Keys::End;

	GLFWKeyToLNKey[GLFW_KEY_SEMICOLON] = Keys::Colon;
	GLFWKeyToLNKey[GLFW_KEY_EQUAL] = Keys::Semicolon;
	GLFWKeyToLNKey[GLFW_KEY_COMMA] = Keys::Comma;
	GLFWKeyToLNKey[GLFW_KEY_PERIOD] = Keys::Period;
	GLFWKeyToLNKey[GLFW_KEY_SLASH] = Keys::Slash;
	GLFWKeyToLNKey[GLFW_KEY_MINUS] = Keys::Minus;
	GLFWKeyToLNKey[GLFW_KEY_BACKSLASH] = Keys::Backslash;
	GLFWKeyToLNKey[GLFW_KEY_WORLD_2] = Keys::Yen;
	GLFWKeyToLNKey[GLFW_KEY_APOSTROPHE] = Keys::Caret;
	GLFWKeyToLNKey[GLFW_KEY_LEFT_BRACKET] = Keys::LBracket;
	GLFWKeyToLNKey[GLFW_KEY_RIGHT_BRACKET] = Keys::RBracket;

	mInitedKeyTable = true;
}

GLFWPlatformWindow::GLFWPlatformWindow()
	: m_glfwWindow(NULL)
{
}

GLFWPlatformWindow::~GLFWPlatformWindow()
{
}

Result GLFWPlatformWindow::init(GLFWPlatformWindowManager* windowManager, const WindowCreationSettings& settings, GLFWContext* sharedContext)
{
	initKeyTable();

	//// とりあえず生成時固定。GLFW は実行中に変更したければ、
	//// glfwCreateWindow() で新しいウィンドウを開きなおす必要がある。
	//if (m_fullScreen)
	//{
	//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	//	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	//	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
	//	StringA s;
	//	s.AssignCStr(windowTitle.c_str());
	//	m_glfwWindow = glfwCreateWindow(vidmode->width, vidmode->height, s.c_str(), NULL/*glfwGetPrimaryMonitor()*/, NULL);
	//}
	//else
	{
		// glGenSamplers を使うのに最低 3.3 必要。
		// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenSamplers.xhtml
		
#if defined(LN_OS_MAC)		// macOS だとバージョン指定が効かない.
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
		glfwWindowHint(GLFW_RESIZABLE, (settings.resizable) ? GL_TRUE : GL_FALSE);
		glfwWindowHint(GLFW_DECORATED, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// for NSGL(macOS)
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// for NSGL(macOS)

		GLFWwindow* sharedWindow = (sharedContext) ? sharedContext->mainWindow()->glfwWindow() : nullptr;
        if (!windowManager->manager()->glfwWithOpenGLAPI()) {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			sharedWindow = nullptr;
		}

		m_glfwWindow = glfwCreateWindow(
			settings.clientSize.width, settings.clientSize.height,
			settings.title.toStdString().c_str(), nullptr, sharedWindow);
		if (LN_ENSURE(m_glfwWindow)) return false;


#if defined(LN_OS_WIN32)
		setDPIFactor(AbstractWin32PlatformWindow::getDpiFactor((HWND)getWin32Window()));
#endif

		SetWindowLongPtr((HWND)getWin32Window(), GWL_STYLE, GetWindowLongPtrA((HWND)getWin32Window(), GWL_STYLE) & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX));
		glfwSetWindowSizeLimits(m_glfwWindow, 100, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);
		glfwSetWindowSize(m_glfwWindow, settings.clientSize.width, settings.clientSize.height);
	}

	glfwSetWindowUserPointer(m_glfwWindow, this);
	glfwSetWindowCloseCallback(m_glfwWindow, window_close_callback);
	glfwSetWindowFocusCallback(m_glfwWindow, window_focus_callback);
	glfwSetFramebufferSizeCallback(m_glfwWindow, window_framebuffer_size_callback);
	glfwSetKeyCallback(m_glfwWindow, window_key_callback);
    glfwSetCharCallback(m_glfwWindow, window_char_callback);
	glfwSetMouseButtonCallback(m_glfwWindow, window_mouseButton_callback);
	glfwSetCursorPosCallback(m_glfwWindow, window_mousePos_callback);
    glfwSetScrollCallback(m_glfwWindow, window_scroll_callback);

    glfwSetWindowCenter(m_glfwWindow);

	//GLFWmonitor* primary = glfwGetPrimaryMonitor();
	//int widthMM, heightMM;
	//glfwGetMonitorPhysicalSize(primary, &widthMM, &heightMM);
	//const GLFWvidmode* mode = glfwGetVideoMode(primary);
	//const double dpi = mode->width / (widthMM / 25.4);
	// 96dpi, 600ppi
	// 1インチ (= 25.4 mm) 


	//glfwMakeContextCurrent(m_glfwWindow);
    return true;
}

void GLFWPlatformWindow::dispose()
{
	glfwDestroyWindow(m_glfwWindow);
}

void GLFWPlatformWindow::setWindowTitle(const String& title)
{
    glfwSetWindowTitle(m_glfwWindow, title.toStdString().c_str());
}

void GLFWPlatformWindow::getSize(SizeI* size)
{
	glfwGetWindowSize(m_glfwWindow, &size->width, &size->height);
}

void GLFWPlatformWindow::setSize(const SizeI& size)
{
	glfwSetWindowSize(m_glfwWindow, size.width, size.height);
}

void GLFWPlatformWindow::getFramebufferSize(int* width, int* height)
{
	glfwGetFramebufferSize(m_glfwWindow, width, height);
}

void GLFWPlatformWindow::setAllowDragDrop(bool value)
{
	glfwSetDropCallback(m_glfwWindow, window_drop_callback);
}

bool GLFWPlatformWindow::isAllowDragDrop() const
{
	return false;
}

PointI GLFWPlatformWindow::pointFromScreen(const PointI& screenPoint)
{
#if defined(LN_EMSCRIPTEN)
	return screenPoint;
#elif defined(LN_OS_WIN32)
	POINT pt = { screenPoint.x, screenPoint.y };
	::ScreenToClient(glfwGetWin32Window(m_glfwWindow), &pt);
	return PointI(pt.x, pt.y);
#elif defined(LN_OS_MAC)
	PointI clientPoint;
	Cocoa_pointFromScreen(glfwGetCocoaWindow(m_glfwWindow), screenPoint.x, screenPoint.y, &clientPoint.x, &clientPoint.y);
	return clientPoint;
#else
	LN_NOTIMPLEMENTED();
#endif
}

PointI GLFWPlatformWindow::pointToScreen(const PointI& clientPoint)
{
#if defined(LN_EMSCRIPTEN)
	return clientPoint;
#elif defined(LN_OS_WIN32)
	POINT pt = { clientPoint.x, clientPoint.y };
	::ClientToScreen(glfwGetWin32Window(m_glfwWindow), &pt);
	return PointI(pt.x, pt.y);
#elif defined(LN_OS_MAC)
	PointI screenPoint;
	Cocoa_pointToScreen(glfwGetCocoaWindow(m_glfwWindow), clientPoint.x, clientPoint.y, &screenPoint.x, &screenPoint.y);
	return screenPoint;
#else
	LN_NOTIMPLEMENTED();
#endif
}

void GLFWPlatformWindow::grabCursor()
{
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GLFWPlatformWindow::releaseCursor()
{
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void* GLFWPlatformWindow::getWin32Window() const
{
#ifdef _WIN32
	return glfwGetWin32Window(m_glfwWindow);
#else
	return nullptr;
#endif
}

//void GLFWPlatformWindow::SetVisible(bool visible)
//{
//	if (visible) {
//		glfwShowWindow(m_glfwWindow);
//	}
//	else {
//		glfwHideWindow(m_glfwWindow);
//	}
//}

void GLFWPlatformWindow::window_close_callback(GLFWwindow* glfw_window)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(glfw_window);
	thisWindow->sendEventToAllListener(PlatformEventArgs::makeClosingEvent(thisWindow));
}

void GLFWPlatformWindow::window_focus_callback(GLFWwindow* glfw_window, int focused)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(glfw_window);
	thisWindow->sendEventToAllListener(PlatformEventArgs::makeActivateChangedEvent(thisWindow, (focused != 0)));
}

void GLFWPlatformWindow::window_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(window);
	thisWindow->sendEventToAllListener(PlatformEventArgs::makeWindowSizeChangedEvent(thisWindow, width, height));
}

void GLFWPlatformWindow::window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(window);

	PlatformEventType eventType = (action == GLFW_PRESS || action == GLFW_REPEAT) ? PlatformEventType::KeyDown : PlatformEventType::KeyUp;

	Keys keyCode = Keys::Unknown;
	if (key != GLFW_KEY_UNKNOWN) {
		keyCode = GLFWKeyToLNKey[key];
	}

	thisWindow->sendEventToAllListener(PlatformEventArgs::makeKeyEvent(thisWindow, eventType, keyCode, glfwKeyModToLNKeyMod(mods), key));
}

void GLFWPlatformWindow::window_char_callback(GLFWwindow* window, unsigned int ch)
{
    GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(window);
    thisWindow->sendEventToAllListener(PlatformEventArgs::makeKeyEvent(thisWindow, PlatformEventType::KeyChar, Keys::Unknown, ModifierKeys::None, ch));

}

void GLFWPlatformWindow::window_mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(window);

	PlatformEventType eventType = PlatformEventType::Unknown;
	switch (action)
	{
	case GLFW_PRESS:
		eventType = PlatformEventType::MouseDown;
		break;
	case GLFW_RELEASE:
		eventType = PlatformEventType::MouseUp;
		break;
	default:
		break;
	}

	MouseButtons mouseButton = MouseButtons::None;
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		mouseButton = MouseButtons::Left;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mouseButton = MouseButtons::Right;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mouseButton = MouseButtons::Middle;
		break;
	case GLFW_MOUSE_BUTTON_4:
		mouseButton = MouseButtons::X1;
		break;
	case GLFW_MOUSE_BUTTON_5:
		mouseButton = MouseButtons::X2;
		break;
	default:
		break;
	}

	if (eventType != PlatformEventType::Unknown && mouseButton != MouseButtons::None)
	{
        //double xpos, ypos;
        //glfwGetCursorPos(window, &xpos, &ypos);
		thisWindow->sendEventToAllListener(PlatformEventArgs::makeMouseButtonEvent(thisWindow, eventType, mouseButton/*, (short)xpos, (short)ypos*/, glfwKeyModToLNKeyMod(mods)));
	}
}

void GLFWPlatformWindow::window_mousePos_callback(GLFWwindow* window, double xpos, double ypos)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(window);

	short grabOffsetX = 0;
	short grabOffsetY = 0;
	if (glfwGetInputMode(thisWindow->m_glfwWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		//int w, h;
		//glfwGetWindowSize(thisWindow->m_glfwWindow, &w, &h);
		//const auto clientPos = PointI(xpos, ypos);// thisWindow->pointFromScreen(PointI(xpos, ypos));
		//grabOffsetX = clientPos.x - (w / 2);
		//grabOffsetY = clientPos.y - (h / 2);
		grabOffsetX = xpos;
		grabOffsetY = ypos;
	}

#if defined(LN_EMSCRIPTEN)
	thisWindow->sendEventToAllListener(PlatformEventArgs::makeMouseMoveEvent(thisWindow, PlatformEventType::MouseMove, (short)xpos, (short)ypos, 0, 0));
#elif defined(LN_OS_WIN32)
	POINT pt;
	::GetCursorPos(&pt);

	PointI grabOffset;
	if (glfwGetInputMode(thisWindow->m_glfwWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		const auto clientPos = thisWindow->pointFromScreen(PointI(pt.x, pt.y));
		SizeI size;
		thisWindow->getSize(&size);
		grabOffsetX = clientPos.x - (size.width / 2);
		grabOffsetY = clientPos.y - (size.height / 2);
	}

	thisWindow->sendEventToAllListener(PlatformEventArgs::makeMouseMoveEvent(thisWindow, PlatformEventType::MouseMove, pt.x, pt.y, grabOffsetX, grabOffsetY));
#elif defined(LN_OS_MAC)
	int x = 0;
	int y = 0;
	Cocoa_getScreenMousePosition(&x, &y);
	thisWindow->sendEventToAllListener(PlatformEventArgs::makeMouseMoveEvent(thisWindow, PlatformEventType::MouseMove, x, y, 0, 0));
#else
	LN_NOTIMPLEMENTED();
#endif
}

void GLFWPlatformWindow::window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(window);
    thisWindow->sendEventToAllListener(PlatformEventArgs::makeMouseWheelEvent(thisWindow, -1.0 * yoffset));
}

void GLFWPlatformWindow::window_drop_callback(GLFWwindow* window, int count, const char** paths)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(window);
	LN_NOTIMPLEMENTED();
}

ModifierKeys GLFWPlatformWindow::glfwKeyModToLNKeyMod(int mods)
{
	Flags<ModifierKeys> modifierKeys =
		((mods == GLFW_MOD_ALT || mods == GLFW_MOD_ALT) ? ModifierKeys::Alt : ModifierKeys::None) |
		((mods == GLFW_MOD_SHIFT || mods == GLFW_MOD_SHIFT) ? ModifierKeys::Shift : ModifierKeys::None) |
		((mods == GLFW_MOD_CONTROL || mods == GLFW_MOD_CONTROL) ? ModifierKeys::Control : ModifierKeys::None);
	return modifierKeys;
}

//=============================================================================
// GLFWPlatformWindowManager

GLFWPlatformWindowManager::GLFWPlatformWindowManager(PlatformManager* manager)
	: PlatformWindowManager(manager)
	, m_glContext(nullptr)
{
}

GLFWPlatformWindowManager::~GLFWPlatformWindowManager()
{
}

Result GLFWPlatformWindowManager::init()
{
	LN_LOG_INFO << "GLFW window manager";

	int result = glfwInit();
	if (LN_ENSURE(result != 0)) return false;

	// OpenGL Version 3.2 を選択
	// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20120908
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

    return true;
}

void GLFWPlatformWindowManager::dispose()
{
	glfwTerminate();
}

Ref<PlatformWindow> GLFWPlatformWindowManager::createMainWindow(const WindowCreationSettings& settings)
{
	if (LN_REQUIRE(!m_glContext)) return nullptr;
	if (LN_REQUIRE(!settings.userWindow)) return nullptr;

	auto ptr = ln::makeRef<GLFWPlatformWindow>();
	if (!ptr->init(this, settings, nullptr)) {
		return nullptr;
	}

	m_glContext = ln::makeRef<GLFWContext>(ptr);

	return ptr;
}

Ref<PlatformWindow> GLFWPlatformWindowManager::createSubWindow(const WindowCreationSettings& settings)
{
	if (LN_REQUIRE(m_glContext)) return nullptr;
	if (LN_REQUIRE(!settings.userWindow)) return nullptr;

	auto ptr = ln::makeRef<GLFWPlatformWindow>();
	if (!ptr->init(this, settings, m_glContext)) {
		return nullptr;
	}
	return ptr;
}

void GLFWPlatformWindowManager::destroyWindow(PlatformWindow* window)
{
	if (LN_REQUIRE(window)) return;
	static_cast<GLFWPlatformWindow*>(window)->dispose();
}

void GLFWPlatformWindowManager::processSystemEventQueue(EventProcessingMode mode)
{
    if (mode == EventProcessingMode::Wait) {
        glfwWaitEvents();
    }
    else {
        glfwPollEvents();
    }
}

OpenGLContext* GLFWPlatformWindowManager::getOpenGLContext() const
{
	return m_glContext;
}

//=============================================================================

bool checkGraphicsSupport()
{
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(200, 200, "Version", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwTerminate();
    return true;
}

} // namespace detail
} // namespace ln

#endif
