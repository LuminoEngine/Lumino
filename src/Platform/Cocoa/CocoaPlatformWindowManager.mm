
#include "../Internal.h"
#include "CocoaPlatformWindow.h"
#include "CocoaPlatformWindowManager.h"


//=============================================================================
// CocoaPlatformApplication
//=============================================================================
@interface CocoaPlatformApplication : NSApplication
@end

@implementation CocoaPlatformApplication

// From http://cocoadev.com/index.pl?GameKeyboardHandlingAlmost
// This works around an AppKit bug, where key up events while holding
// down the command key don't get sent to the key window.
- (void)sendEvent:(NSEvent *)event
{
    if ([event type] == NSKeyUp && ([event modifierFlags] & NSCommandKeyMask))
        [[self keyWindow] sendEvent:event];
    else
        [super sendEvent:event];
}

@end

//=============================================================================
// CocoaPlatformApplicationDelegate
//=============================================================================
@interface CocoaPlatformApplicationDelegate : NSObject
@end

@implementation CocoaPlatformApplicationDelegate

// アプリを終了しようとしているときに呼び出される
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    return NSTerminateCancel;
}

// 色深度などスクリーンのパラメータを変更したときに呼ばれる
- (void)applicationDidChangeScreenParameters:(NSNotification *) notification
{
    // GLFW では _glfwInputMonitorChange() を呼び出していた。
    // UE4 には applicationDidChangeScreenParameters()　は無かった。
}

// アプリケーションの一般的な起動処理が終わると呼び出される
- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
}

// アプリケーションが隠された直後呼ばれる
- (void)applicationDidHide:(NSNotification *)notification
{
}

@end


LN_NAMESPACE_BEGIN

//=============================================================================
// CocoaPlatformWindowManager
//=============================================================================

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
    
    // TODO: 必要？
	//[NSApp run];
}

//-----------------------------------------------------------------------------
void CocoaPlatformWindowManager::CreateMainWindow(const WindowCreationSettings& settings)
{
	m_mainWindow = LN_NEW CocoaPlatformWindow(this);
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
	RefPtr<CocoaPlatformWindow> window(LN_NEW CocoaPlatformWindow(this), false);
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

	if (m_appDelegate != 0)
	{
		[NSApp setDelegate:nil];
		[m_appDelegate release];
		m_appDelegate = nil;
	}
}


LN_NAMESPACE_END
