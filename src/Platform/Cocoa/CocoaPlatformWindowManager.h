
#pragma once
#include "../PlatformWindowManagerBase.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// CocoaPlatformWindowManager
//=============================================================================
class CocoaPlatformWindowManager
    : public WindowManagerBase
{
public:
    CocoaPlatformWindowManager();
	virtual ~CocoaPlatformWindowManager();
	void Initialize();

public:
	// WindowManagerBase interface
	virtual void CreateMainWindow(const WindowCreationSettings& settings) override;
	virtual PlatformWindow* GetMainWindow() override;
	virtual PlatformWindow* CreateSubWindow(const WindowCreationSettings& settings) override;
	virtual void DoEvents() override;
	virtual void Finalize() override;

private:
	CocoaPlatformWindow*	m_mainWindow;
	id						m_appDelegate;
};

//-----------------------------------------------------------------------------
CocoaPlatformWindowManager::CocoaPlatformWindowManager()
    : m_mainWindow(nullptr)
	, m_appDelegate(nil)
{
}

//-----------------------------------------------------------------------------
CocoaPlatformWindowManager::~CocoaPlatformWindowManager()
{
}

//-----------------------------------------------------------------------------
void CocoaPlatformWindowManager::Initialize()
{
	if (NSApp)
	{
		LN_THROW(0, InvalidOperationException);
	}

	// 共有アプリケーションのインスタンスを作成
	[CocoaPlatformApplication sharedApplication];

	// 一般的な GUI アプリケーション扱い
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

	// TODO: メニューバーを作るならこのへん？

	// NSApp への Delegate
	m_appDelegate = [[CocoaPlatformApplicationDelegate alloc] init];
	LN_THROW(m_appDelegate != nil, InvalidOperationException);

	[NSApp setDelegate:m_appDelegate];
	[NSApp run];
}

//-----------------------------------------------------------------------------
void CocoaPlatformWindowManager::CreateMainWindow(const WindowCreationSettings& settings)
{
    LN_CHECK_STATE(m_mainWindow == nullptr)
	m_mainWindow = LN_NEW CocoaPlatformWindowManager(this);
	m_mainWindow->Initialize(this, settings.title, settings.clientSize.width, settings.clientSize.height, settings.fullscreen, settings.resizable);
}

//-----------------------------------------------------------------------------
PlatformWindow* CocoaPlatformWindowManager::GetMainWindow()
{
	return m_mainWindow;
}

//-----------------------------------------------------------------------------
PlatformWindow* CocoaPlatformWindowManager::CreateSubWindow(const WindowCreationSettings& settings)
{
	RefPtr<CocoaPlatformWindowManager> window(LN_NEW CocoaPlatformWindowManager(this), false);
	window->Initialize(this, settings.title, settings.clientSize.width, settings.clientSize.height, settings.fullscreen, settings.resizable);
	return window.DetachMove();
}

//-----------------------------------------------------------------------------
void CocoaPlatformWindowManager::DoEvents()
{
	for (;;)
	{
		NSEvent* event = [NSApp nextEventMatchingMask:NSAnyEventMask
								untilDate:[NSDate distantPast]
								inMode:NSDefaultRunLoopMode
								dequeue:YES];
		if (event == nil)
		{
			break;
		}

		[NSApp sendEvent:event];
	}
}

//-----------------------------------------------------------------------------
void CocoaPlatformWindowManager::Finalize()
{
	LN_SAFE_RELEASE(m_mainWindow);

	if (_glfw.ns.delegate)
	{
		[NSApp setDelegate:nil];
		[m_appDelegate release];
		m_appDelegate = nil;
	}
}

LN_NAMESPACE_END
