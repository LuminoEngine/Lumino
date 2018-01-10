
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
PlatformEventArgs PlatformEventArgs::makeClosingEvent(PlatformWindow* sender)
{
	return PlatformEventArgs(PlatformEventType::close, sender);
}

PlatformEventArgs PlatformEventArgs::makeWindowSizeChangedEvent(PlatformWindow* sender, int width, int height)
{
	PlatformEventArgs e(PlatformEventType::WindowSizeChanged, sender);
	e.size.width = width;
	e.size.height = height;
	return e;
}

PlatformEventArgs PlatformEventArgs::makeActivateChangedEvent(PlatformWindow* sender, bool active)
{
	PlatformEventArgs e;
	e.sender = sender;
	e.type = (active) ? PlatformEventType::WindowActivate : PlatformEventType::WindowDeactivate;
	return e;
}

PlatformEventArgs PlatformEventArgs::makeKeyEvent(PlatformWindow* sender, PlatformEventType type, Keys keyCode, ModifierKeys modifierKeys, char keyChar)
{
	PlatformEventArgs e;
	e.type = type;
	e.sender = sender;
	e.key.keyCode = keyCode;
	e.key.modifierKeys = (ModifierKeys::value_type)modifierKeys.getValue();
	e.key.keyChar = keyChar;
	return e;
}

PlatformEventArgs PlatformEventArgs::makeMouseWheelEvent(PlatformWindow* sender, int delta)
{
	PlatformEventArgs e;
	e.type = PlatformEventType::MouseWheel;
	e.sender = sender;
	e.wheel.delta = delta;
	return e;
}

PlatformEventArgs PlatformEventArgs::makeDragDropEvent(PlatformWindow* sender, PlatformEventType type, DataObject* data, DragDropEffects* effect)
{
	PlatformEventArgs e;
	e.type = type;
	e.sender = sender;
	e.dragDrop.data = data;
	e.dragDrop.effect = effect;
	return e;
}

//==============================================================================
// PlatformManager
//==============================================================================

static PlatformManager* g_platformManager = nullptr;

//------------------------------------------------------------------------------
PlatformManager* PlatformManager::getInstance(PlatformManager* priority)
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
	dispose();
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
		auto m = Ref<Win32WindowManager>::makeRef(0);
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
        auto m = Ref<CocoaPlatformWindowManager>::makeRef();
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
		auto m = Ref<X11WindowManager>::makeRef();
		m_windowManager = m.detachMove();
	}
#endif
	LN_REQUIRE(m_windowManager != nullptr);

	if (m_useThread) {
		m_mainWindowThreadInitFinished.setFalse();
		m_mainWindowThreadEndRequested.setFalse();
		m_mainWindowThread.start(createDelegate(this, &PlatformManager::thread_MainWindow));
		m_mainWindowThreadInitFinished.wait();	// 初期化終了まで待機する
	}
	else {
		m_windowManager->createMainWindow(m_windowCreationSettings);
	}

	// MainWindow
	//m_mainWindow = LN_NEW PlatformWindow(m_windowManager->getMainWindow());
    
    m_windowManager->getMainWindow()->setVisible(true);

	if (g_platformManager == nullptr)
	{
		g_platformManager = this;
	}
}

//------------------------------------------------------------------------------
PlatformWindow* PlatformManager::getMainWindow()
{
	return m_windowManager->getMainWindow();
}

//------------------------------------------------------------------------------
bool PlatformManager::doEvents()
{
	// メインスレッドでメッセージ処理する場合は InternalDoEvents
	if (!m_useThread) {
		m_windowManager->doEvents();
	}

	return !m_windowManager->isEndRequested();
}

//------------------------------------------------------------------------------
void PlatformManager::dispose()
{
	if (m_windowManager != NULL)
	{
		// 別スレッドでメッセージ処理していた場合異はスレッド終了待機
		if (m_useThread) {
			m_mainWindowThreadEndRequested.setTrue();	// 終了要求だして、
			m_mainWindowThread.wait();					// 待つ
		}
		// メインスレッドでメッセージ処理していた場合はここで Destroy
		else {
			m_windowManager->dispose();
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
void PlatformManager::thread_MainWindow()
{
	// 初期化
	m_windowManager->createMainWindow(m_windowCreationSettings);
	m_mainWindowThreadInitFinished.setTrue();	// 初期化完了

	// メッセージループ
	while (!m_mainWindowThreadEndRequested.isTrue())
	{
		m_windowManager->doEvents();
		Thread::sleep(10);
	}

	// 終了処理
	m_windowManager->dispose();
}

LN_NAMESPACE_END
