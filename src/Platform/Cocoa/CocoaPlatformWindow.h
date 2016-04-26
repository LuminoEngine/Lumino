
#pragma once
#include <Lumino/Platform/PlatformWindow.h>

LN_NAMESPACE_BEGIN
class CocoaPlatformWindowManager;


static Key TranslateKey(std::uint16_t keyCode)
{
    constexpr Keys keyTable[127] = {
        /* 0x00 */ Key::A, // kVK_ANSI_A
        /* 0x01 */ Key::S, // kVK_ANSI_S
        /* 0x02 */ Key::D, // kVK_ANSI_D
        /* 0x03 */ Key::F, // kVK_ANSI_F
        /* 0x04 */ Key::H, // kVK_ANSI_H
        /* 0x05 */ Key::G, // kVK_ANSI_G
        /* 0x06 */ Key::Z, // kVK_ANSI_Z
        /* 0x07 */ Key::X, // kVK_ANSI_X
        /* 0x08 */ Key::C, // kVK_ANSI_C
        /* 0x09 */ Key::V, // kVK_ANSI_V
        /* 0x0A */ Key::Unknown, // kVK_ISO_Section
        /* 0x0B */ Key::B, // kVK_ANSI_B
        /* 0x0C */ Key::Q, // kVK_ANSI_Q
        /* 0x0D */ Key::W, // kVK_ANSI_W
        /* 0x0E */ Key::E, // kVK_ANSI_E
        /* 0x0F */ Key::R, // kVK_ANSI_R
        /* 0x10 */ Key::Y, // kVK_ANSI_Y
        /* 0x11 */ Key::T, // kVK_ANSI_T
        /* 0x12 */ Key::D1, // kVK_ANSI_1
        /* 0x13 */ Key::D2, // kVK_ANSI_2
        /* 0x14 */ Key::D3, // kVK_ANSI_3
        /* 0x15 */ Key::D4, // kVK_ANSI_4
        /* 0x16 */ Key::D6, // kVK_ANSI_6
        /* 0x17 */ Key::D5, // kVK_ANSI_5
        /* 0x18 */ Key::Equals, // kVK_ANSI_Equal
        /* 0x19 */ Key::D9, // kVK_ANSI_9
        /* 0x1A */ Key::D7, // kVK_ANSI_7
        /* 0x1B */ Key::Minus, // kVK_ANSI_Minus
        /* 0x1C */ Key::D8, // kVK_ANSI_8
        /* 0x1D */ Key::D0, // kVK_ANSI_0
        /* 0x1E */ Key::RBracket, // kVK_ANSI_RightBracket
        /* 0x1F */ Key::O, // kVK_ANSI_O
        /* 0x20 */ Key::U, // kVK_ANSI_U
        /* 0x21 */ Key::LBracket, // kVK_ANSI_LeftBracket
        /* 0x22 */ Key::I, // kVK_ANSI_I
        /* 0x23 */ Key::P, // kVK_ANSI_P
        /* 0x24 */ Key::Enter, // kVK_Return
        /* 0x25 */ Key::L, // kVK_ANSI_L
        /* 0x26 */ Key::J, // kVK_ANSI_J
        /* 0x27 */ Key::Unknown, // kVK_ANSI_Quote
        /* 0x28 */ Key::K, // kVK_ANSI_K
        /* 0x29 */ Key::Semicolon, // kVK_ANSI_Semicolon
        /* 0x2A */ Key::Backslash, // kVK_ANSI_Backslash
        /* 0x2B */ Key::Comma, // kVK_ANSI_Comma
        /* 0x2C */ Key::Slash, // kVK_ANSI_Slash
        /* 0x2D */ Key::N, // kVK_ANSI_N
        /* 0x2E */ Key::M, // kVK_ANSI_M
        /* 0x2F */ Key::Period, // kVK_ANSI_Period
        /* 0x30 */ Key::Tab, // kVK_Tab
        /* 0x31 */ Key::Space, // kVK_Space
        /* 0x32 */ Key::AccentGrave, // kVK_ANSI_Grave
        /* 0x33 */ Key::Delete, // kVK_Delete
        /* 0x34 */ Key::Unknown,
        /* 0x35 */ Key::Escape, // kVK_Escape
        /* 0x36 */ Key::RightApple, // NOTE: maybe
        /* 0x37 */ Key::LeftApple, // kVK_Command
        /* 0x38 */ Key::LeftShift, // kVK_Shift
        /* 0x39 */ Key::CapsLock, // kVK_CapsLock
        /* 0x3A */ Key::LeftAlt, // kVK_Option
        /* 0x3B */ Key::LeftControl, // kVK_Control
        /* 0x3C */ Key::RightShift, // kVK_RightShift
        /* 0x3D */ Key::RightAlt, // kVK_RightOption
        /* 0x3E */ Key::RightControl, // kVK_RightControl
        /* 0x3F */ Key::Unknown, // kVK_Function
        /* 0x40 */ Key::Unknown, // kVK_F17
        /* 0x41 */ Key::KeypadDecimal, // kVK_ANSI_KeypadDecimal
        /* 0x42 */ Key::Unknown,
        /* 0x43 */ Key::KeypadMultiply, // kVK_ANSI_KeypadMultiply
        /* 0x44 */ Key::Unknown,
        /* 0x45 */ Key::KeypadAdd, // kVK_ANSI_KeypadPlus
        /* 0x46 */ Key::Unknown,
        /* 0x47 */ Key::Clear, // kVK_ANSI_KeypadClear
        /* 0x48 */ Key::Unknown, // kVK_VolumeUp
        /* 0x49 */ Key::Unknown, // kVK_VolumeDown
        /* 0x4A */ Key::Unknown, // kVK_Mute
        /* 0x4B */ Key::KeypadDivide, // kVK_ANSI_KeypadDivide
        /* 0x4C */ Key::Enter, // kVK_ANSI_KeypadEnter
        /* 0x4D */ Key::Unknown,
        /* 0x4E */ Key::KeypadSubtract, // kVK_ANSI_KeypadMinus
        /* 0x4F */ Key::Unknown, // kVK_F18
        /* 0x50 */ Key::Unknown, // kVK_F19
        /* 0x51 */ Key::Equals, // kVK_ANSI_KeypadEquals
        /* 0x52 */ Key::Keypad0, // kVK_ANSI_Keypad0
        /* 0x53 */ Key::Keypad1, // kVK_ANSI_Keypad1
        /* 0x54 */ Key::Keypad2, // kVK_ANSI_Keypad2
        /* 0x55 */ Key::Keypad3, // kVK_ANSI_Keypad3
        /* 0x56 */ Key::Keypad4, // kVK_ANSI_Keypad4
        /* 0x57 */ Key::Keypad5, // kVK_ANSI_Keypad5
        /* 0x58 */ Key::Keypad6, // kVK_ANSI_Keypad6
        /* 0x59 */ Key::Keypad7, // kVK_ANSI_Keypad7
        /* 0x5A */ Key::Unknown, // kVK_F20
        /* 0x5B */ Key::Keypad8, // kVK_ANSI_Keypad8
        /* 0x5C */ Key::Keypad9, // kVK_ANSI_Keypad9
        /* 0x5D */ Key::Backslash, // kVK_JIS_Yen
        /* 0x5E */ Key::Unknown, // kVK_JIS_Underscore
        /* 0x5F */ Key::Comma, // kVK_JIS_KeypadComma
        /* 0x60 */ Key::F5, // kVK_F5
        /* 0x61 */ Key::F6, // kVK_F6
        /* 0x62 */ Key::F7, // kVK_F7
        /* 0x63 */ Key::F3, // kVK_F3
        /* 0x64 */ Key::F8, // kVK_F8
        /* 0x65 */ Key::F9, // kVK_F9
        /* 0x66 */ Key::Unknown, // kVK_JIS_Eisu
        /* 0x67 */ Key::F11, // kVK_F11
        /* 0x68 */ Key::KanaMode, // kVK_JIS_Kana
        /* 0x69 */ Key::F13, // kVK_F13
        /* 0x6A */ Key::Unknown, // kVK_F16
        /* 0x6B */ Key::F14, // kVK_F14
        /* 0x6C */ Key::Unknown,
        /* 0x6D */ Key::F10, // kVK_F10
        /* 0x6E */ Key::Unknown,
        /* 0x6F */ Key::F12, // kVK_F12
        /* 0x70 */ Key::Unknown,
        /* 0x71 */ Key::F15, // kVK_F15
        /* 0x72 */ Key::Help, // kVK_Help
        /* 0x73 */ Key::Home, // kVK_Home
        /* 0x74 */ Key::PageUp, // kVK_PageUp
        /* 0x75 */ Key::Delete, // kVK_ForwardDelete
        /* 0x76 */ Key::F4, // kVK_F4
        /* 0x77 */ Key::End, // kVK_End
        /* 0x78 */ Key::F2, // kVK_F2
        /* 0x79 */ Key::PageDown, // kVK_PageDown
        /* 0x7A */ Key::F1, // kVK_F1
        /* 0x7B */ Key::Left, // kVK_LeftArrow
        /* 0x7C */ Key::Right, // kVK_RightArrow
        /* 0x7D */ Key::Down, // kVK_DownArrow
        /* 0x7E */ Key::Up, // kVK_UpArrow
    };

    if (keyCode < sizeof(keyTable)) {
        return keyTable[keyCode];
    }
    return Key::Unknown;
}

static ModifierKeys TranslateModifierKeyFlags(NSUInteger flags)
{
    ModifierKeys mods = 0;

    if (flags & NSShiftKeyMask)
        mods |= ModifierKeys::Shift;
    if (flags & NSControlKeyMask)
        mods |= ModifierKeys::Control;
    if (flags & NSAlternateKeyMask)
        mods |= ModifierKeys::Alt;
    if (flags & NSCommandKeyMask)
        mods |= GLFW_MOD_SUPER;

    return mods;
}

//=============================================================================
// CocoaPlatformCoreView
//=============================================================================
@interface CocoaPlatformCoreView : NSView
{
	CocoaPlatformWindow*	m_host;
	NSTrackingArea*			m_trackingArea;		// マウスイベントを受け取るために必要
}

- (id)initWithPlatformWindow:(CocoaPlatformWindow *)host;

@end

@implementation GLFWContentView

+ (void)initialize
{
	if (self == [GLFWContentView class])
	{
		if (_glfw.ns.cursor == nil)
		{
			NSImage* data = [[NSImage alloc] initWithSize:NSMakeSize(16, 16)];
			_glfw.ns.cursor = [[NSCursor alloc] initWithImage:data
													  hotSpot:NSZeroPoint];
			[data release];
		}
	}
}

- (id)initWithPlatformWindow:(_GLFWwindow *)host
{
	self = [super init];
	if (self != nil)
	{
		m_host = host;
		m_trackingArea = nil;

		[self updateTrackingAreas];
		[self registerForDraggedTypes:[NSArray arrayWithObjects:
									   NSFilenamesPboardType, nil]];
	}

	return self;
}

-　(void)dealloc
{
	[m_trackingArea release];
	[super dealloc];
}

// 不透明か？
- (BOOL)isOpaque
{
	return YES;
}

// キービューになることが出来るか？
- (BOOL)canBecomeKeyView
{
	return YES;
}

// ファーストレスポンダになれるか？(キーイベントを受け取るために必要)
- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (void)cursorUpdate:(NSEvent *)event
{
	//updateModeCursor(window);
}

- (void)mouseDown:(NSEvent *)event
{
	// TODO:
	// http://ryo1miya.hatenablog.com/entry/20110727/1311765160
	// translateFlags([event modifierFlags])
}

- (void)mouseDragged:(NSEvent *)event
{
	[self mouseMoved:event];
}

- (void)mouseUp:(NSEvent *)event
{
	// TODO:
}

- (void)mouseMoved:(NSEvent *)event
{
	// TODO:
	const NSRect contentRect = [self frame];
	const NSPoint pos = [event locationInWindow];
	_glfwInputCursorMotion(window, pos.x, contentRect.size.height - pos.y);
}

- (void)rightMouseDown:(NSEvent *)event
{
	// TODO:
}

- (void)rightMouseDragged:(NSEvent *)event
{
	[self mouseMoved:event];
}

- (void)rightMouseUp:(NSEvent *)event
{
	// TODO:
}

- (void)otherMouseDown:(NSEvent *)event
{
	// TODO:
}

- (void)otherMouseDragged:(NSEvent *)event
{
	[self mouseMoved:event];
}

- (void)otherMouseUp:(NSEvent *)event
{
	// TODO:
	//_glfwInputMouseClick(window,
	//					 (int) [event buttonNumber],
	//					 GLFW_RELEASE,
	//					 translateFlags([event modifierFlags]));
}

- (void)mouseExited:(NSEvent *)event
{
	// TODO:
	//_glfwInputCursorEnter(window, GL_FALSE);
}

- (void)mouseEntered:(NSEvent *)event
{
	// TODO:
	//_glfwInputCursorEnter(window, GL_TRUE);
}

/*
https://developer.apple.com/library/mac/documentation/Cocoa/Reference/ApplicationKit/Classes/NSView_Class/#//apple_ref/occ/instm/NSView/viewDidChangeBackingProperties
ビューのバッキングストアのプロパティが変更されたときに応答します。

ビューには、ときにバッキングストアスケールまたはカラースペースの変更、このメッセージを取得します。
それは資産を交換またはビューのバッキングストアのプロパティが変更されたときに、
他の調整を行う必要がある場合アプリは実装を提供する必要があります。
*/
- (void)viewDidChangeBackingProperties
{
	// GLFW ではここでフレームバッファサイズの再設定を行っていた
}

- (void)drawRect:(NSRect)rect
{
	_glfwInputWindowDamage(window);
}

- (void)updateTrackingAreas
{
	if (m_trackingArea != nil)
	{
		[self removeTrackingArea:m_trackingArea];
		[m_trackingArea release];
	}

	const NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited |
										  NSTrackingActiveInKeyWindow |
										  NSTrackingEnabledDuringMouseDrag |
										  NSTrackingCursorUpdate |
										  NSTrackingInVisibleRect |
										  NSTrackingAssumeInside;

	m_trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
												options:options
												owner:self
												userInfo:nil];

	[self addTrackingArea:m_trackingArea];
	[super updateTrackingAreas];
}

- (void)keyDown:(NSEvent *)event
{
	const Key key = TranslateKey([event keyCode]);
	const ModifierKeys mods = TranslateModifierKeyFlags([event modifierFlags]);
	m_host->SendPlatformKeyEvent(PlatformEventType::KeyDown, m_host, key, mods, 0);

	NSString* characters = [event characters];
	NSUInteger i, length = [characters length];
	const int plain = !(mods & GLFW_MOD_SUPER);

	for (i = 0; i < length; i++)
	{
		const unichar codepoint = [characters characterAtIndex:i];
		if ((codepoint & 0xff00) == 0xf700)
			continue;

		m_host->SendPlatformKeyEvent(PlatformEventType::KeyDown, m_host, key, mods, codepoint);
	}
}

- (void)flagsChanged:(NSEvent *)event
{
	// TODO: 修飾キーが変化すると呼ばれる
}

- (void)keyUp:(NSEvent *)event
{
	const int key = translateKey([event keyCode]);
	const int mods = translateFlags([event modifierFlags]);
	m_host->SendPlatformKeyEvent(PlatformEventType::KeyUp, m_host, key, mods, 0);
}

- (void)scrollWheel:(NSEvent *)event
{
	double deltaX, deltaY;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
	{
		deltaX = [event scrollingDeltaX];
		deltaY = [event scrollingDeltaY];

		if ([event hasPreciseScrollingDeltas])
		{
			deltaX *= 0.1;
			deltaY *= 0.1;
		}
	}
	else
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/
	{
		deltaX = [event deltaX];
		deltaY = [event deltaY];
	}

	if (fabs(deltaX) > 0.0 || fabs(deltaY) > 0.0)
	{
		m_host->SendPlatformMouseWheelEvent((int)deltaY);
	}
}

#if 0	// D&D 実装するならこの辺が必要そう
- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender
{
	return NSDragOperationNone;
}

- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender
{
	return YES;
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
	return YES;
}

- (void)concludeDragOperation:(id <NSDraggingInfo>)sender
{
}
#endif

@end

//=============================================================================
// CocoaPlatformCoreWindow
//=============================================================================
@interface CocoaPlatformCoreWindow : NSWindow {}
@end

@implementation CocoaPlatformCoreWindow

- (BOOL)canBecomeKeyWindow
{
	// キーウィンドウにする。キーイベントを受け取れるようになる。
	return YES;
}

@end

//=============================================================================
// CocoaPlatformCoreWindowDelegate
//=============================================================================
@interface CocoaPlatformCoreWindowDelegate : NSObject
{
	CocoaPlatformWindow* m_host;
}

- (id)initWithPlatformWindow:(_GLFWwindow*)host;

@end

@implementation GLFWWindowDelegate

- (id)initWithPlatformWindow:(CocoaPlatformWindow*)host
{
	self = [super init];
	if (self != nil)
	{
		m_host = host;
	}
	return self;
}

// ウインドウを閉じようとする時に呼ばれる
- (BOOL)windowShouldClose:(id)sender
{
	m_host->SendPlatformClosingEvent(m_host);
	return NO;
}

// ウィンドウサイズが変わったときに呼ばれる
- (void)windowDidResize:(NSNotification*)notification
{
	/* TODO: Resize/Move 時には context の update が必要になる。
	[ _context update ]; // コンテキストの更新！
   [ _context makeCurrentContext ];
   glViewport( 0, 0, w, h );
   return;
   */
	const NSRect contentRect = [window->ns.view frame];
	m_host->SendPlatformWindowSizeChangedEvent(contentRect.size.width, contentRect.size.height);
}

// ウインドウを動かし終わったときに呼び出される
- (void)windowDidMove:(NSNotification *)notification
{
}

// 最小化（ドック入り）した時に呼び出される
- (void)windowDidMiniaturize:(NSNotification *)notification
{
}

// 最小化（ドック入り）状態が解除されたときに呼び出される
- (void)windowDidDeminiaturize:(NSNotification *)notification
{
}

// キーウインドウになったときに呼ばれる
- (void)windowDidBecomeKey:(NSNotification *)notification
{
	m_host->SendPlatformActivateChangedEvent(m_host, true);
}

// ウインドウがキーウインドウでなくなったときに呼び出される
- (void)windowDidResignKey:(NSNotification *)notification
{
	m_host->SendPlatformActivateChangedEvent(m_host, false);
}

@end




//=============================================================================
// CocoaPlatformWindow
//=============================================================================
class CocoaPlatformWindow
	: public PlatformWindow
{
public:
	CocoaPlatformWindow(CocoaPlatformWindowManager* manager);
	virtual ~CocoaPlatformWindow();
	void Initialize(CocoaPlatformWindowManager* windowManager, String windowTitle, int width, int height, bool fullscreen, bool resizable);

public:
	// PlatformWindow interface
	virtual void SetVisible(bool visible) override;
	virtual void SetFullScreenEnabled(bool enabled) override;
	virtual bool IsFullScreenEnabled() const override;

private:
	id		m_window;
	id		m_delegate;
};

//-----------------------------------------------------------------------------
CocoaPlatformWindow::CocoaPlatformWindow(CocoaPlatformWindowManager* manager)
	: m_window(nil)
	, m_delegate(nil)
{

}

//-----------------------------------------------------------------------------
CocoaPlatformWindow::~CocoaPlatformWindow()
{
	// ウインドウを表示しなくなりますでもメモリは解放しません
	[m_window orderOut:nil];

	// ウィンドウから delegate を取り外して release
	[m_window setDelegate:nil];
	[m_delegate release];
	m_delegate = nil;

	[m_view release];
	m_view = nil;

	[m_window close];
	m_window = nil;
}

//-----------------------------------------------------------------------------
void CocoaPlatformWindow::Initialize(CocoaPlatformWindowManager* windowManager, String windowTitle, int width, int height, bool fullscreen, bool resizable)
{
	// ウィンドウのイベントを受信する delegate
	m_delegate = [[CocoaPlatformCoreWindowDelegate alloc] initWithPlatformWindow:window];
	LN_THROW(m_delegate != nil, InvalidateOperationException);

	// ウィンドウスタイルを決める
	unsigned int windowStyle = 0;
	//if ()
	//	styleMask = NSBorderlessWindowMask;	// フルスクリーンにするならこれ
	//else
	{
		styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
		if (resizable)
			styleMask |= NSResizableWindowMask;
	}

	// ウィンドウの位置・サイズを決める
	NSRect contentRect;
	contentRect = NSMakeRect(0, 0, width, height);

	// ウィンドウを作る
	m_window = [[CocoaPlatformCoreWindow alloc]
				initWithContentRect:contentRect
				styleMask:windowStyle
				backing:NSBackingStoreBuffered
				defer:NO];
	LN_THROW(m_window != nil, InvalidateOperationException);

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
	{
		if (resizable)
		{
			// フルスクリーン用のボタンがタイトルバー右端に付いて、フルスクリーンになれるようになる。
			[m_window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
		}
	}
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/

	// 前後関係や位置の調整
	if (0)
	{
		// TODO: フルスクリーン
		[m_window setLevel:NSMainMenuWindowLevel + 1];
	}
	else
	{
		[m_window center];
	}

	// View 作成
	m_view = [[CocoaPlatformCoreView alloc] initWithPlatformWindow:m_window];

#if 0	// Retina Display 対応
	// GLFW ではさらに #ifdef _GLFW_USE_RETINA されていた
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
		[window->ns.view setWantsBestResolutionOpenGLSurface:YES];
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/
#endif

	StringA local = windowTitle;
	const char* title = local.c_str();
	[m_window setTitle:[NSString stringWithUTF8String:title]];
	[m_window setDelegate:m_delegate];
	[m_window setAcceptsMouseMovedEvents:YES];
	[m_window setContentView:m_view];

	// 再度アプリ起動した際、前回終了時のウィンドウ状態を復元するか？
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
	{
		[m_window setRestorable:NO];	// とりあえずいらないです
	}
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/

	return GL_TRUE;
}

//-----------------------------------------------------------------------------
void CocoaPlatformWindow::SetVisible(bool visible)
{
	if (visible)
	{
		// makeKeyAndOrderFront だけだとウィンドウが前面に出ないことがあるようだ
		// http://xcatsan.blogspot.jp/2009/02/blog-post_12.html
		// 最終手段はこれ
		// [NSApp activateIgnoringOtherApps:YES];
		// GLFW でもこれを行っているが、「表示のたびに毎回やるのはまずいんじゃないの？」みたいな注意書きがある。

	    [m_window makeKeyAndOrderFront:nil];
	}
	else
	{
		[m_window orderOut:nil];
	}
}

//-----------------------------------------------------------------------------
void CocoaPlatformWindow::SetFullScreenEnabled(bool enabled)
{
	//　TODO:
}

//-----------------------------------------------------------------------------
bool CocoaPlatformWindow::IsFullScreenEnabled() const
{
	return false;
}

LN_NAMESPACE_END
