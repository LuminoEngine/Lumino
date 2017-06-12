
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#if defined(LN_OS_WIN32)
	#include "Win32/Win32PlatformWindowManager.h"
	#include <Lumino/Platform/Win32/Win32PlatformWindow.h>
#elif defined(LN_OS_MAC)
    #include "Cocoa/CocoaPlatformWindowManager.h"
#elif defined(LN_X11)
	#include "X11/X11WindowManager.h"
#endif
#include "PlatformManager.h"


LN_NAMESPACE_BEGIN

//==============================================================================
// PlatformEventArgs
//==============================================================================
PlatformEventArgs PlatformEventArgs::MakeClosingEvent(PlatformWindow* sender)
{
	return PlatformEventArgs(PlatformEventType::close, sender);
}

PlatformEventArgs PlatformEventArgs::MakeWindowSizeChangedEvent(PlatformWindow* sender, int width, int height)
{
	PlatformEventArgs e(PlatformEventType::WindowSizeChanged, sender);
	e.size.width = width;
	e.size.height = height;
	return e;
}

PlatformEventArgs PlatformEventArgs::MakeActivateChangedEvent(PlatformWindow* sender, bool active)
{
	PlatformEventArgs e;
	e.sender = sender;
	e.type = (active) ? PlatformEventType::WindowActivate : PlatformEventType::WindowDeactivate;
	return e;
}

PlatformEventArgs PlatformEventArgs::MakeKeyEvent(PlatformWindow* sender, PlatformEventType type, Keys keyCode, ModifierKeys modifierKeys, char keyChar)
{
	PlatformEventArgs e;
	e.type = type;
	e.sender = sender;
	e.key.keyCode = keyCode;
	e.key.modifierKeys = (ModifierKeys::value_type)modifierKeys.getValue();
	e.key.keyChar = keyChar;
	return e;
}

PlatformEventArgs PlatformEventArgs::MakeMouseWheelEvent(PlatformWindow* sender, int delta)
{
	PlatformEventArgs e;
	e.type = PlatformEventType::MouseWheel;
	e.sender = sender;
	e.wheel.delta = delta;
	return e;
}

//==============================================================================
// PlatformManager
//==============================================================================

static PlatformManager* g_platformManager = nullptr;

//------------------------------------------------------------------------------
PlatformManager* PlatformManager::GetInstance(PlatformManager* priority)
{
	return (priority != nullptr) ? priority : g_platformManager;
}

//------------------------------------------------------------------------------
PlatformManager::PlatformManager()
	: m_useThread(false)
	, m_windowManager(NULL)
{
}

//------------------------------------------------------------------------------
PlatformManager::PlatformManager(const Settings& settings)
	: m_useThread(false)
	, m_windowManager(NULL)
{
	initialize(settings);
}

//------------------------------------------------------------------------------
PlatformManager::~PlatformManager()
{
	Dispose();
}

//------------------------------------------------------------------------------
void PlatformManager::initialize(const Settings& settings)
{
	m_windowCreationSettings = settings.mainWindowSettings;
	m_useThread = settings.useInternalUIThread;

	WindowSystemAPI api = settings.windowSystemAPI;

#if defined(LN_OS_WIN32)
	// select default
	if (api == WindowSystemAPI::Default)
	{
		api = WindowSystemAPI::Win32API;
	}
	// create window manager
	if (api == WindowSystemAPI::Win32API)
	{
		auto m = RefPtr<Win32WindowManager>::makeRef(0);
		m_windowManager = m.detachMove();
    }
#elif defined(LN_OS_MAC)
    // select default
    if (api == WindowSystemAPI::Default)
    {
        api = WindowSystemAPI::Cocoa;
    }
    // create window manager
    if (api == WindowSystemAPI::Cocoa)
    {
        auto m = RefPtr<CocoaPlatformWindowManager>::makeRef();
        m->initialize();
        m_windowManager = m.detachMove();
    }

#elif defined(LN_X11)
	// select default
	if (api == WindowSystemAPI::Default)
	{
		api = WindowSystemAPI::X11;
	}
	// create window manager
	if (api == WindowSystemAPI::X11)
	{
		auto m = RefPtr<X11WindowManager>::makeRef();
		m_windowManager = m.detachMove();
	}
#endif
	LN_THROW(m_windowManager != nullptr, ArgumentException);

	if (m_useThread) {
		m_mainWindowThreadInitFinished.SetFalse();
		m_mainWindowThreadEndRequested.SetFalse();
		m_mainWindowThread.start(createDelegate(this, &PlatformManager::Thread_MainWindow));
		m_mainWindowThreadInitFinished.Wait();	// 初期化終了まで待機する
	}
	else {
		m_windowManager->CreateMainWindow(m_windowCreationSettings);
	}

	// MainWindow
	//m_mainWindow = LN_NEW PlatformWindow(m_windowManager->GetMainWindow());
    
    m_windowManager->GetMainWindow()->SetVisible(true);

	if (g_platformManager == nullptr)
	{
		g_platformManager = this;
	}
}

//------------------------------------------------------------------------------
PlatformWindow* PlatformManager::GetMainWindow()
{
	return m_windowManager->GetMainWindow();
}

//------------------------------------------------------------------------------
bool PlatformManager::DoEvents()
{
	// メインスレッドでメッセージ処理する場合は InternalDoEvents
	if (!m_useThread) {
		m_windowManager->DoEvents();
	}

	return !m_windowManager->IsEndRequested();
}

//------------------------------------------------------------------------------
void PlatformManager::Dispose()
{
	if (m_windowManager != NULL)
	{
		// 別スレッドでメッセージ処理していた場合異はスレッド終了待機
		if (m_useThread) {
			m_mainWindowThreadEndRequested.SetTrue();	// 終了要求だして、
			m_mainWindowThread.Wait();					// 待つ
		}
		// メインスレッドでメッセージ処理していた場合はここで Destroy
		else {
			m_windowManager->Finalize();
		}
		//LN_SAFE_RELEASE(m_mainWindow);
		LN_SAFE_RELEASE(m_windowManager);
	}

	if (g_platformManager == this)
	{
		g_platformManager = nullptr;
	}
}

//------------------------------------------------------------------------------
void PlatformManager::Thread_MainWindow()
{
	// 初期化
	m_windowManager->CreateMainWindow(m_windowCreationSettings);
	m_mainWindowThreadInitFinished.SetTrue();	// 初期化完了

	// メッセージループ
	while (!m_mainWindowThreadEndRequested.IsTrue())
	{
		m_windowManager->DoEvents();
		Thread::Sleep(10);
	}

	// 終了処理
	m_windowManager->Finalize();
}

LN_NAMESPACE_END
