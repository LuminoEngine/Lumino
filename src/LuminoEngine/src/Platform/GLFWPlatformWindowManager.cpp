
#ifdef LN_GLFW

#include "Internal.hpp"
#include "GLFWPlatformWindowManager.hpp"

namespace ln {
namespace detail {
	
//=============================================================================
// GLFWPlatformWindow

static Keys GLFWKeyToLNKey[GLFW_KEY_LAST];
static void initKeyTable()
{
	static bool mInitedKeyTable = false;
	if ( mInitedKeyTable ) return;

	GLFWKeyToLNKey[GLFW_KEY_UNKNOWN] = Keys::Unknown;

	GLFWKeyToLNKey[GLFW_KEY_A] = Keys::A;
	GLFWKeyToLNKey[GLFW_KEY_B] = Keys::B;
	GLFWKeyToLNKey[GLFW_KEY_C] = Keys::C;
	GLFWKeyToLNKey[GLFW_KEY_B] = Keys::D;
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

void GLFWPlatformWindow::initialize(const WindowCreationSettings& settings)
{
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
		glfwWindowHint(GLFW_RESIZABLE, (settings.resizable) ? GL_TRUE : GL_FALSE);
		glfwWindowHint(GLFW_DECORATED, GL_TRUE);
		m_glfwWindow = glfwCreateWindow(settings.clientSize.width, settings.clientSize.height, settings.title.toStdString().c_str(), NULL, NULL);
		if (LN_ENSURE(m_glfwWindow)) return;
	}

	glfwSetWindowUserPointer(m_glfwWindow, this);
	glfwSetWindowCloseCallback(m_glfwWindow, window_close_callback);
	glfwSetWindowFocusCallback(m_glfwWindow, window_focus_callback);
	glfwSetKeyCallback(m_glfwWindow, window_key_callback);

	//glfwMakeContextCurrent(m_glfwWindow);
	//glewInit();
	////GLuint mProgram = glCreateProgram();// LN_CHECK_GLERROR();
	//// 初回クリア (しておかないと、背景が透明なままになる)
	//glClear(GL_COLOR_BUFFER_BIT);
	//glfwSwapBuffers(m_glfwWindow);
}

void GLFWPlatformWindow::dispose()
{
	glfwDestroyWindow(m_glfwWindow);
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

void GLFWPlatformWindow::window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(window);

	PlatformEventType eventType = (action == GLFW_PRESS || action == GLFW_REPEAT) ? PlatformEventType::KeyDown : PlatformEventType::KeyUp;

	Flags<ModifierKeys> modifierKeys =
		((key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) ? ModifierKeys::Alt : ModifierKeys::None) |
		((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) ? ModifierKeys::Shift : ModifierKeys::None) |
		((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) ? ModifierKeys::Control : ModifierKeys::None);

	PlatformEventArgs::makeKeyEvent(thisWindow, eventType, GLFWKeyToLNKey[key], modifierKeys, key);
}

//=============================================================================
// GLFWPlatformWindowManager

GLFWPlatformWindowManager::GLFWPlatformWindowManager()
{
}

GLFWPlatformWindowManager::~GLFWPlatformWindowManager()
{
}

void GLFWPlatformWindowManager::initialize()
{
	PlatformWindowManager::initialize();

	int result = glfwInit();
	if (LN_ENSURE(result != 0)) return;

	// OpenGL Version 3.2 を選択
	// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20120908
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
}

void GLFWPlatformWindowManager::dispose()
{
	glfwTerminate();
	PlatformWindowManager::dispose();
}

Ref<PlatformWindow> GLFWPlatformWindowManager::createWindow(const WindowCreationSettings& settings)
{
	auto obj = ln::makeRef<GLFWPlatformWindow>();
	obj->initialize(settings);
	return obj;
}

void GLFWPlatformWindowManager::destroyWindow(PlatformWindow* window)
{
	if (LN_REQUIRE(window)) return;
	window->dispose();
}

void GLFWPlatformWindowManager::processSystemEventQueue()
{
	glfwPollEvents();
}

} // namespace detail
} // namespace ln

#endif
