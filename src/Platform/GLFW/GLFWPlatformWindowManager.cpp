#include "../../Internal.h"
#include "GLFWPlatformWindow.h"
#include "GLFWPlatformWindowManager.h"

LN_NAMESPACE_BEGIN

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLFWPlatformWindowManager::GLFWPlatformWindowManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GLFWPlatformWindowManager::~GLFWPlatformWindowManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindowManager::Initialize()
{
	if ( !glfwInit() ) {
		LN_THROW(0, InvalidOperationException);
	}
	
	// OpenGL Version 3.2 を選択
	// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20120908
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindowManager::CreateMainWindow(const WindowCreationSettings& settings)
{
	m_mainWindow.Attach(LN_NEW GLFWPlatformWindow(this), false);
	m_mainWindow->Initialize(this, settings.Title, settings.ClientSize.width, settings.ClientSize.height, settings.Fullscreen, settings.Resizable);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PlatformWindow* GLFWPlatformWindowManager::GetMainWindow()
{
	return m_mainWindow;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PlatformWindow* GLFWPlatformWindowManager::CreateSubWindow(const WindowCreationSettings& settings)
{
	RefPtr<GLFWPlatformWindow> window(LN_NEW GLFWPlatformWindow(this), false);
	window->Initialize(this, settings.Title, settings.ClientSize.width, settings.ClientSize.height, settings.Fullscreen, settings.Resizable);
	return window.DetachMove();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindowManager::DoEvents()
{
	glfwPollEvents();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GLFWPlatformWindowManager::Finalize()
{
	glfwTerminate();
}

#if 0
//==============================================================================
// ■ GLFWWindowManager
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GLFWWindowManager::GLFWWindowManager( Manager* manager )
		: WindowManagerBase	( manager )
		, mMainWindow		( NULL )
		, mConsoleAlloced	( false )
		, mSystemMouseShown	( true )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	GLFWWindowManager::~GLFWWindowManager()
	{
		glfwTerminate();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindowManager::preInitialize( const SettingData& setting_data )
	{
		mSettingData = setting_data;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindowManager::initialize()
	{
		if ( !glfwInit() ) {
			LN_THROW_Fatal( 0 );
		}

		GLFWWindow::SettingData data;
		data.TitleText		= mSettingData.TitleText;
		data.Width			= mSettingData.Width;
		data.Height			= mSettingData.Height;
		data.Windowed		= mSettingData.Windowed;
		data.Resizable		= mSettingData.Resizable;
		GLFWWindow* window = LN_NEW GLFWWindow( this );
		window->initialize( data );
		mMainWindow = window;

		if ( mSettingData.UseConsole )
		{
			
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindowManager::finalize()
	{
		// 割り当てたコンソールが残っていれば解放
		if ( mConsoleAlloced )
		{
			printf( "Enter キーを押すと終了します...\n" );
     
			getchar();
			//::FreeConsole();
			mConsoleAlloced = false;

			// コンソールアプリとして実行している場合は終了時に「キーを押すと終了～」と
			// メッセージが出て一時停止するけど、AllocConsole() で割り当てた場合は停止しない。
			// そのため、一度メッセージを出して getchar() で停止してから終了する
		}


		if ( mMainWindow ) mMainWindow->finalize();
		LN_SAFE_RELEASE( mMainWindow );

		WindowManagerBase::finalize();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void GLFWWindowManager::processMessage()
	{
		glfwPollEvents();
		/*
		// 非アクティブの場合はクライアント領域外で移動したことにして、カーソルを表示する
		if ( mMainWindow->isActive() == false )
		{
			this->mMouseCursorVisibleCounter.onMoveCursor( false );
		}

		// 時間経過によるマウスカーソルの非表示処理
		bool mc_visible = this->mMouseCursorVisibleCounter.checkVisible();
		if ( mc_visible != mSystemMouseShown )
		{
			if ( mc_visible )
			{
				::ShowCursor( TRUE );
			}
			else
			{
				::ShowCursor( FALSE );
			}
			mSystemMouseShown = mc_visible;
		}

		// メッセージ処理
		MSG msg;
		while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if ( ::GetMessage( &msg, NULL, 0, 0 ) )
			{
				::TranslateMessage( &msg );

				bool handled = false;
				LRESULT r = sendWndMsgToAttachedListener( msg.hwnd, msg.message, msg.wParam, msg.lParam, &handled );
				if ( !handled ) 
				{
					switch ( msg.message )
					{
						case WM_MOUSEMOVE:
							this->mMouseCursorVisibleCounter.onMoveCursor( true );
							break;
						case WM_NCMOUSEMOVE:
							this->mMouseCursorVisibleCounter.onMoveCursor( false );
							break;
					}
					::DispatchMessage( &msg );
				}
			}
		}
		*/
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	Window* GLFWWindowManager::getMainWindow() const 
	{ 
		return mMainWindow; 
	}
#endif

LN_NAMESPACE_END
