
#include "../../Internal.h"
#include "GLFWPlatformWindowManager.h"
#include "GLFWPlatformWindow.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// GLFWPlatformWindow
//=============================================================================

static Key GLFWKeyToLNKey[GLFW_KEY_LAST];
static void initKeyTable()
{
	static bool mInitedKeyTable = false;
	if ( mInitedKeyTable ) return;

	GLFWKeyToLNKey[GLFW_KEY_UNKNOWN] = Key::Unknown;

	GLFWKeyToLNKey[GLFW_KEY_A] = Key::A;
	GLFWKeyToLNKey[GLFW_KEY_B] = Key::B;
	GLFWKeyToLNKey[GLFW_KEY_C] = Key::C;
	GLFWKeyToLNKey[GLFW_KEY_B] = Key::D;
	GLFWKeyToLNKey[GLFW_KEY_E] = Key::E;
	GLFWKeyToLNKey[GLFW_KEY_F] = Key::F;
	GLFWKeyToLNKey[GLFW_KEY_G] = Key::G;
	GLFWKeyToLNKey[GLFW_KEY_H] = Key::H;
	GLFWKeyToLNKey[GLFW_KEY_I] = Key::I;
	GLFWKeyToLNKey[GLFW_KEY_J] = Key::J;
	GLFWKeyToLNKey[GLFW_KEY_K] = Key::K;
	GLFWKeyToLNKey[GLFW_KEY_L] = Key::L;
	GLFWKeyToLNKey[GLFW_KEY_M] = Key::M;
	GLFWKeyToLNKey[GLFW_KEY_N] = Key::N;
	GLFWKeyToLNKey[GLFW_KEY_O] = Key::O;
	GLFWKeyToLNKey[GLFW_KEY_P] = Key::P;
	GLFWKeyToLNKey[GLFW_KEY_Q] = Key::Q;
	GLFWKeyToLNKey[GLFW_KEY_R] = Key::R;
	GLFWKeyToLNKey[GLFW_KEY_S] = Key::S;
	GLFWKeyToLNKey[GLFW_KEY_T] = Key::T;
	GLFWKeyToLNKey[GLFW_KEY_U] = Key::U;
	GLFWKeyToLNKey[GLFW_KEY_V] = Key::V;
	GLFWKeyToLNKey[GLFW_KEY_W] = Key::W;
	GLFWKeyToLNKey[GLFW_KEY_X] = Key::X;
	GLFWKeyToLNKey[GLFW_KEY_Y] = Key::Y;
	GLFWKeyToLNKey[GLFW_KEY_Z] = Key::Z;

	GLFWKeyToLNKey[GLFW_KEY_0] = Key::D0;
	GLFWKeyToLNKey[GLFW_KEY_1] = Key::D1;
	GLFWKeyToLNKey[GLFW_KEY_2] = Key::D2;
	GLFWKeyToLNKey[GLFW_KEY_3] = Key::D3;
	GLFWKeyToLNKey[GLFW_KEY_4] = Key::D4;
	GLFWKeyToLNKey[GLFW_KEY_5] = Key::D5;
	GLFWKeyToLNKey[GLFW_KEY_6] = Key::D6;
	GLFWKeyToLNKey[GLFW_KEY_7] = Key::D7;
	GLFWKeyToLNKey[GLFW_KEY_8] = Key::D8;
	GLFWKeyToLNKey[GLFW_KEY_9] = Key::D9;

	GLFWKeyToLNKey[GLFW_KEY_F1] = Key::F1;
	GLFWKeyToLNKey[GLFW_KEY_F2] = Key::F2;
	GLFWKeyToLNKey[GLFW_KEY_F3] = Key::F3;
	GLFWKeyToLNKey[GLFW_KEY_F4] = Key::F4;
	GLFWKeyToLNKey[GLFW_KEY_F5] = Key::F5;
	GLFWKeyToLNKey[GLFW_KEY_F6] = Key::F6;
	GLFWKeyToLNKey[GLFW_KEY_F7] = Key::F7;
	GLFWKeyToLNKey[GLFW_KEY_F8] = Key::F8;
	GLFWKeyToLNKey[GLFW_KEY_F9] = Key::F9;
	GLFWKeyToLNKey[GLFW_KEY_F10] = Key::F10;
	GLFWKeyToLNKey[GLFW_KEY_F11] = Key::F11;
	GLFWKeyToLNKey[GLFW_KEY_F12] = Key::F12;

	GLFWKeyToLNKey[GLFW_KEY_SPACE] = Key::Space;
	GLFWKeyToLNKey[GLFW_KEY_ESCAPE] = Key::Escape;
	GLFWKeyToLNKey[GLFW_KEY_UP] = Key::Up;
	GLFWKeyToLNKey[GLFW_KEY_DOWN] = Key::Down;
	GLFWKeyToLNKey[GLFW_KEY_LEFT] = Key::Left;
	GLFWKeyToLNKey[GLFW_KEY_RIGHT] = Key::Right;
	GLFWKeyToLNKey[GLFW_KEY_LEFT_SHIFT] = Key::LShift;
	GLFWKeyToLNKey[GLFW_KEY_RIGHT_SHIFT] = Key::RShift;
	GLFWKeyToLNKey[GLFW_KEY_LEFT_CONTROL] = Key::LCtrl;
	GLFWKeyToLNKey[GLFW_KEY_RIGHT_CONTROL] = Key::RCtrl;
	GLFWKeyToLNKey[GLFW_KEY_LEFT_ALT] = Key::LAlt;
	GLFWKeyToLNKey[GLFW_KEY_RIGHT_ALT] = Key::RAlt;
	GLFWKeyToLNKey[GLFW_KEY_TAB] = Key::Tab;
	GLFWKeyToLNKey[GLFW_KEY_ENTER] = Key::Enter;
	GLFWKeyToLNKey[GLFW_KEY_BACKSPACE] = Key::BackSpace;
	GLFWKeyToLNKey[GLFW_KEY_INSERT] = Key::Insert;
	GLFWKeyToLNKey[GLFW_KEY_DELETE] = Key::Delete;
	GLFWKeyToLNKey[GLFW_KEY_PAGE_UP] = Key::PageUp;
	GLFWKeyToLNKey[GLFW_KEY_PAGE_DOWN] = Key::PageDown;
	GLFWKeyToLNKey[GLFW_KEY_HOME] = Key::Home;
	GLFWKeyToLNKey[GLFW_KEY_END] = Key::End;

	GLFWKeyToLNKey[GLFW_KEY_SEMICOLON] = Key::Colon;
	GLFWKeyToLNKey[GLFW_KEY_EQUAL] = Key::SemiColon;
	GLFWKeyToLNKey[GLFW_KEY_COMMA] = Key::Comma;
	GLFWKeyToLNKey[GLFW_KEY_PERIOD] = Key::Period;
	GLFWKeyToLNKey[GLFW_KEY_SLASH] = Key::Slash;
	GLFWKeyToLNKey[GLFW_KEY_MINUS] = Key::Minus;
	GLFWKeyToLNKey[GLFW_KEY_BACKSLASH] = Key::BackSlash;
	GLFWKeyToLNKey[GLFW_KEY_WORLD_2] = Key::Yen;
	GLFWKeyToLNKey[GLFW_KEY_APOSTROPHE] = Key::Caret;
	GLFWKeyToLNKey[GLFW_KEY_LEFT_BRACKET] = Key::LBracket;
	GLFWKeyToLNKey[GLFW_KEY_RIGHT_BRACKET] = Key::RBracket;

	mInitedKeyTable = true;
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLFWPlatformWindow::GLFWPlatformWindow(GLFWPlatformWindowManager* manager)
	: PlatformWindow(manager)
	, m_glfwWindow(NULL)
	, m_fullScreen(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLFWPlatformWindow::~GLFWPlatformWindow()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindow::Initialize(GLFWPlatformWindowManager* windowManager, String windowTitle, int width, int height, bool fullscreen, bool resizable)
{
	m_fullScreen = fullscreen;

	// とりあえず生成時固定。GLFW は実行中に変更したければ、
	// glfwCreateWindow() で新しいウィンドウを開きなおす必要がある。
	if (m_fullScreen)
	{
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_DECORATED, GL_FALSE);
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
		StringA s;
		s.AssignCStr(windowTitle.c_str());
		m_glfwWindow = glfwCreateWindow(vidmode->width, vidmode->height, s.c_str(), NULL/*glfwGetPrimaryMonitor()*/, NULL);
	}
	else
	{
		glfwWindowHint(GLFW_RESIZABLE, (resizable) ? GL_TRUE : GL_FALSE);
		glfwWindowHint(GLFW_DECORATED, GL_TRUE);
		StringA s;
		s.AssignCStr(windowTitle.c_str());
		m_glfwWindow = glfwCreateWindow(width, height, s.c_str(), NULL, NULL);
	}
	LN_THROW(m_glfwWindow != NULL, InvalidOperationException);

	glfwSetWindowUserPointer(m_glfwWindow, this);
	glfwSetWindowCloseCallback(m_glfwWindow, window_close_callback);
	glfwSetWindowFocusCallback(m_glfwWindow, window_focus_callback);
	glfwSetKeyCallback(m_glfwWindow, window_key_callback);

	glfwMakeContextCurrent(m_glfwWindow);
	glewInit();
	//GLuint mProgram = glCreateProgram();// LN_CHECK_GLERROR();
	// 初回クリア (しておかないと、背景が透明なままになる)
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(m_glfwWindow);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Size GLFWPlatformWindow::GetSize() const
{
	Size size;
	glfwGetWindowSize(m_glfwWindow, &size.width, &size.height);
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void GLFWPlatformWindow::SetVisible(bool visible)
//{
//	if (visible) {
//		glfwShowWindow(m_glfwWindow);
//	}
//	else {
//		glfwHideWindow(m_glfwWindow);
//	}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindow::SetFullScreenEnabled(bool enabled)
{
	LN_NOTIMPLEMENTED();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GLFWPlatformWindow::IsFullScreenEnabled() const
{
	return m_fullScreen;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindow::CaptureMouse()
{
	// TODO: GLFW では MouseDown/Upイベントのときに一緒にキャプチャしている。それにあわせるべき？
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindow::ReleaseMouseCapture()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindow::window_close_callback(GLFWwindow* glfw_window)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(glfw_window);
	thisWindow->SendPlatformClosingEvent(thisWindow);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindow::window_focus_callback(GLFWwindow* glfw_window, int focused)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(glfw_window);
	thisWindow->SendPlatformActivateChangedEvent(thisWindow, (focused != 0));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindow::window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GLFWPlatformWindow* thisWindow = (GLFWPlatformWindow*)glfwGetWindowUserPointer(window);

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		thisWindow->SendPlatformKeyEvent(
			PlatformEventType::KeyDown, thisWindow, GLFWKeyToLNKey[key],
			(key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT),
			(key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT),
			(key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL), 0);
	}
	else if (action == GLFW_RELEASE)
	{
		thisWindow->SendPlatformKeyEvent(
			PlatformEventType::KeyUp, thisWindow, GLFWKeyToLNKey[key],
			(key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT),
			(key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT),
			(key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL), 0);
	}
}

LN_NAMESPACE_END


#if 0
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GLFWWindow::GLFWWindow( GLFWWindowManager* manager )
		: WindowBase		( manager )
		, mWindowManager	( manager )
		, mLastMouseX		( -1 )
		, mLastMouseY		( -1 )
		, mIsActive			( false )
		, mOrginalWidth     ( 640 )
		, mOrginalHeight    ( 480 )
		, mFullScreen       ( false )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GLFWWindow::~GLFWWindow()
	{
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::initialize( const SettingData& setting_data )
	{
		mTitleText      = setting_data.TitleText;
		mClientSize.w   = setting_data.Width;
		mClientSize.h   = setting_data.Height;
		mFullScreen     = !setting_data.Windowed;
		mOrginalWidth   = mClientSize.w;
		mOrginalHeight  = mClientSize.h;

		initKeyTable();

		
		
		// OpenGL Version 3.2 を選択
		// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20120908
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );
		glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE );

		// とりあえず生成時固定。GLFW は実行中に変更したければ、
		// glfwCreateWindow() で新しいウィンドウを開きなおす必要がある。
		if ( mFullScreen )
		{
			glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
			glfwWindowHint( GLFW_DECORATED, GL_FALSE );
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* vidmode = glfwGetVideoMode( monitor );
			mGLFWWindow = glfwCreateWindow( vidmode->width, vidmode->height, mTitleText.c_str(), NULL/*glfwGetPrimaryMonitor()*/, NULL );
		}
		else
		{
			glfwWindowHint( GLFW_RESIZABLE, (setting_data.Resizable) ? GL_TRUE : GL_FALSE );
			glfwWindowHint( GLFW_DECORATED, GL_TRUE );
			mGLFWWindow = glfwCreateWindow( mClientSize.w, mClientSize.h, mTitleText.c_str(), NULL, NULL );
		}
		LN_THROW_SystemCall( mGLFWWindow );

		mIsActive = true;

		glfwSetWindowUserPointer( mGLFWWindow, this );
		glfwSetWindowCloseCallback( mGLFWWindow, window_close_callback );
		glfwSetWindowFocusCallback( mGLFWWindow, window_focus_callback );
		glfwSetKeyCallback( mGLFWWindow, window_key_callback );
		
		glfwMakeContextCurrent( mGLFWWindow );
		glewInit();
		//GLuint mProgram = glCreateProgram();// LN_CHECK_GLERROR();
		// 初回クリア (しておかないと、背景が透明なままになる)
		glClear( GL_COLOR_BUFFER_BIT );
        glfwSwapBuffers( mGLFWWindow );

		//printf( "GL_EXTENSIONS: %s\n", glGetString( GL_EXTENSIONS ) );
  //  printf( "GL_VERSION: %s\n", glGetString( GL_VERSION ) );
  //  printf( "GL_REBDER: %s\n", glGetString( GL_RENDERER ) );
  //  printf( "GL_VENDOR: %s\n", glGetString( GL_VENDOR ) );

		// ウィンドウの大きさを変更する
		//_resize( !setting_data.Windowed );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::finalize()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	const Geometry::Size& GLFWWindow::getSize()
	{
		int w = 0;
		int h = 0;
		glfwGetWindowSize( mGLFWWindow, &w, &h );
		mClientSize.set( w, h );
		return mClientSize;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::setVisible( bool flag )
	{
		if ( flag ) {
			glfwShowWindow( mGLFWWindow );
		}
		else {
			glfwHideWindow( mGLFWWindow );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::setEnableFullScreen( bool flag )
	{
		LN_THROW_NotImpl( 0 );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::setWindowTextFooter( const lnChar* format, ... )
	{
		lnChar buf[ MAX_WINDOW_TEXT_FOOTER ];
		va_list args;
		int len;
		va_start( args, format );
		len = _vsctprintf( format, args ) + 1;
		_vstprintf_s( buf, len, format, args );
		va_end( args );

		lnChar text[ MAX_WINDOW_TEXT ] = { 0 };
		_stprintf_s( text, MAX_WINDOW_TEXT, _T( "%s%s" ), mTitleText.c_str(), buf );
		
		glfwSetWindowTitle( mGLFWWindow, text );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::_resize( bool fullscreen )
	{
		LN_THROW_NotImpl( 0 );

		//int x, y, sw, sh;
		//int win_w = mOrginalWidth;
		//int win_h = mOrginalHeight;

		
			

			//glfwWindowHint( GLFW_DECORATED , GL_TRUE );
			//glfwGetWindowAttrib
		// フルスクリーンモードの場合
		if ( fullscreen )
		{
			//glfwGetDesktopMode();
			
		}
		// ウィンドウモードの場合は実際にウィンドウの位置を設定する
		else
		{
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::captureMouse()
	{
		//glfwSetInputMode
	}
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::releaseMouseCapture()
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::window_close_callback( GLFWwindow* glfw_window )
	{
		GLFWWindow* thisWindow = (GLFWWindow*)glfwGetWindowUserPointer( glfw_window );

		EventArgs e;
		e.Type			= LN_EVENT_CLOSE;
		e.Sender		= thisWindow;
		e.StructSize	= sizeof(EventArgs);
		thisWindow->mWindowManager->getManager()->postEventFromWindowThread( &e );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::window_focus_callback( GLFWwindow* glfw_window, int focused )
	{
		GLFWWindow* thisWindow = (GLFWWindow*)glfwGetWindowUserPointer( glfw_window );
		thisWindow->mIsActive = (focused != 0);

		EventArgs e;
		e.Type			= ( thisWindow->mIsActive ) ? LN_EVENT_APP_ACTIVATE : LN_EVENT_APP_DEACTIVATE;
		e.Sender		= thisWindow;
		e.StructSize	= sizeof(EventArgs);
		thisWindow->mWindowManager->getManager()->postEventFromWindowThread( &e );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindow::window_key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
	{
		GLFWWindow* thisWindow = (GLFWWindow*)glfwGetWindowUserPointer( window );

		if ( action == GLFW_PRESS || action == GLFW_REPEAT )
		{
			KeyEventArgs e;
			e.Type			= LN_EVENT_KEY_DOWN;
			e.Sender		= thisWindow;
			e.StructSize	= sizeof(KeyEventArgs);
			e.KeyCode		= GLFWKeyToLNKey[key];
			e.IsAlt			= (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT);
			e.IsShift		= (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT);
			e.IsControl		= (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL);
			thisWindow->mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );
		}
		else if ( action == GLFW_RELEASE )
		{
			KeyEventArgs e;
			e.Type			= LN_EVENT_KEY_UP;
			e.Sender		= thisWindow;
			e.StructSize	= sizeof(KeyEventArgs);
			e.KeyCode		= GLFWKeyToLNKey[key];
			e.IsAlt			= (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT);
			e.IsShift		= (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT);
			e.IsControl		= (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL);
			thisWindow->mWindowManager->getManager()->postEventFromWindowThread( (EventArgs*)&e );
		}
	}

} // namespace System
} // namespace Core
} // namespace LNote


//==============================================================================
//
//==============================================================================
#endif