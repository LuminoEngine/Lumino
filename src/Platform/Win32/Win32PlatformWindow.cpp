
/*
	[2015/01/10] メッセージ処理は非同期にするべきか？

		するべきではない。ウィンドウドラッグ中に描画更新などができるが、
		煩雑なメッセージ管理やコールバックが必要で、メリットよりもデメリットの方が大きい。

		デメリット
		・イベントは必ず PostEvent で一度メインスレッドに渡した後、
		  メイン側から各 PlatformWindow にアタッチされている Listener に送信する必要がある。少し複雑。
		・PostEvent しなければならないということは、キューに入れるイベントデータは
		  new したオブジェクトかサイズ固定の構造体でなければならない。
		  イベント1つごとに new はコストが高いし、サイズ固定構造体は拡張性が無い。
		・Closing イベントでアプリの終了確認メッセージボックスとかが出せない。
		  ツール系ではメインスレッドが動いたままだと都合が悪い。
		  →ゲームならまぁいいけど。
*/

#include "../../Internal.h"
#include <Lumino/Platform/Win32/Win32PlatformWindow.h>
#include "Win32PlatformWindowManager.h"
#include "../MouseCursorVisibility.h"

LN_NAMESPACE_BEGIN
	
//=============================================================================
// Win32PlatformWindow
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32PlatformWindow::Win32PlatformWindow(Win32WindowManager* app)
	: PlatformWindow(app)
	, mLastMouseX(-1)
	, mLastMouseY(-1)
	, mIsActive(true)	// 初期値 true。WM_ACTIVATE は初回表示で最前面になった時は呼ばれない
	, m_systemMouseShown(true)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
	Win32PlatformWindow::~Win32PlatformWindow()
{
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void Win32PlatformWindow::HideCursor()
//{
//	if (m_cursorShown)
//	{
//		::ShowCursor(FALSE);
//		m_cursorShown = false;
//	}
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void Win32PlatformWindow::ShowCursor()
//{
//	if (!m_cursorShown)
//	{
//		::ShowCursor(TRUE);
//		m_cursorShown = true;
//	}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LRESULT Win32PlatformWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled)
{
	*handled = false;

	if (!UserWndProc.IsEmpty())
	{
		LRESULT dr = UserWndProc.Call(hwnd, msg, wparam, lparam, handled);//RaiseDelegateEvent(PreWndProc, hwnd, msg, wparam, lparam, handled);
		if (*handled) {
			return dr;
		}
	}

	// WM_SYSKEYDOWN 関係をむやみに postMessage() すると強制終了するので必要なものだけフィルタリング
	//if ( msg != WM_SYSKEYDOWN || (msg == WM_SYSKEYDOWN && wparam == VK_RETURN) )
	{
		// マウスキャプチャの処理
		switch (msg)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			::SetCapture(hwnd);
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			::ReleaseCapture();
			break;
		}

		switch (msg)
		{
			/////////////////// ウィンドウが閉じられようとしている
			case WM_CLOSE:
			{
				/*
				ここは DefWindowProc() を呼ばないで終了する。
				こうしないと、×ボタンが押された瞬間にウィンドウが破棄されて、
				終了直前の最後の描画時に DirectX の EndScene() で
				「描画先のウィンドウがありません」ということでエラーが出る。

				代わりに終了フラグだけを立てて、それをメインループで検出、
				その後の finalize() 呼び出しで DestroyWindow() を呼び出す。
				*/

				PlatformEventArgs e(PlatformEventType::Close, this);
				if (NortifyEvent(e)) {
					*handled = true;
					return 0;
				}

				// TODO
				if (this == m_windowManager->GetMainWindow()) {
					m_windowManager->Exit();
				}

				*handled = true;
				return 0;
			}

			/////////////////// ウィンドウがアクティブ・非アクティブになった場合
			case WM_ACTIVATE:
			{
				bool active = ((wparam & 0xffff) != 0);
				if (active != mIsActive)
				{
					mIsActive = active;

					//PlatformEventArgs e;
					//e.Type = (mIsActive) ? LN_EVENT_APP_ACTIVATE : LN_EVENT_APP_DEACTIVATE;
					//e.Sender = this;
					//SendEventToAllListener(e);		// 同期処理の場合はこの場で通知
					//mApplication->PostEvent(&e);	// 非同期処理の場合は一度キューに入れる
				}
				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// ウィンドウサイズが変更された
			case WM_SIZE:
			{
				// ウィンドウサイズを拾っておく
				mClientSize.Set(lparam & 0xFFFF, (lparam >> 16) & 0xFFFF);

				PlatformEventArgs e(PlatformEventType::WindowSizeChanged, this);
				e.size.width = (lparam & 0xFFFF);
				e.size.height = ((lparam >> 16) & 0xFFFF);
				NortifyEvent(e);

				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// Alt + Enter 確認
			//case WM_SYSKEYDOWN:
			//{
			//	if ( wparam == VK_RETURN )
			//	{
			//		PlatformEventArgs e;
			//		e.Type			= LN_EVENT_ALTENTER;
			//		e.Sender		= this;
			//		e.StructSize	= sizeof(PlatformEventArgs);
			//		mWindowManager->getManager()->postEventFromWindowThread( &e );

			//		return MAKELONG( -1, MNC_EXECUTE );     // 第一引数はメニューの属性。今回はメニュー使ってないのでとりあえずこのまま
			//	}
			//	// ここは以降に DefWindowProc() を呼ぶ。
			//	// こうしないと Alt + F4 とかが無効になってそれで終了できなくなる。
			//	return 0;
			//}
			/////////////////////////////////////////////// Alt + Enter の警告音カット http://www26.atwiki.jp/gunsyu3/pages/11.html
			case WM_SYSCHAR:
			{
				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// マウスボタン入力
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			{
				PlatformEventArgs e;
				e.sender = this;

				switch (msg)
				{
				case WM_LBUTTONDOWN:
					e.type = PlatformEventType::MouseDown;
					e.mouse.button = MouseButton::Left;
					break;
				case WM_LBUTTONUP:
					e.type = PlatformEventType::MouseUp;
					e.mouse.button = MouseButton::Left;
					break;
				case WM_RBUTTONDOWN:
					e.type = PlatformEventType::MouseDown;
					e.mouse.button = MouseButton::Right;
					break;
				case WM_RBUTTONUP:
					e.type = PlatformEventType::MouseUp;
					e.mouse.button = MouseButton::Right;
					break;
				case WM_MBUTTONDOWN:
					e.type = PlatformEventType::MouseDown;
					e.mouse.button = MouseButton::Middle;
					break;
				case WM_MBUTTONUP:
					e.type = PlatformEventType::MouseUp;
					e.mouse.button = MouseButton::Middle;
					break;
				case WM_XBUTTONDOWN:
					e.type = PlatformEventType::MouseDown;
					e.mouse.button = (wparam & MK_XBUTTON1) ? MouseButton::X1 : MouseButton::X2;
					break;
				case WM_XBUTTONUP:
					e.type = PlatformEventType::MouseUp;
					e.mouse.button = (wparam & MK_XBUTTON1) ? MouseButton::X1 : MouseButton::X2;
					break;
				}

				e.mouse.x = LOWORD(lparam);
				e.mouse.y = HIWORD(lparam);
				e.mouse.wheelDelta = 0;
				e.mouse.moveX = (mLastMouseX >= 0) ? e.mouse.x - mLastMouseX : 0;
				e.mouse.moveY = (mLastMouseY >= 0) ? e.mouse.y - mLastMouseY : 0;
				NortifyEvent(e);

				mLastMouseX = e.mouse.x;
				mLastMouseY = e.mouse.y;

				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// マウス移動
			case WM_MOUSEMOVE:
			{
				PlatformEventArgs e;
				e.type = PlatformEventType::MouseMove;
				e.sender = this;
				e.mouse.button = MouseButton::None;
				e.mouse.wheelDelta = 0;
				e.mouse.x = static_cast< short >(LOWORD(lparam));     // 一度 short にキャストしないと、
				e.mouse.y = static_cast< short >(HIWORD(lparam));     // マイナス値になったとき 65535 とか値が入る
				e.mouse.moveX = (mLastMouseX >= 0) ? e.mouse.x - mLastMouseX : 0;
				e.mouse.moveY = (mLastMouseY >= 0) ? e.mouse.y - mLastMouseY : 0;
				e.mouse.inClientArea = true;
				NortifyEvent(e);

				mLastMouseX = e.mouse.x;
				mLastMouseY = e.mouse.y;

				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// クライアント領域外でマウスが移動した
			case WM_NCMOUSEMOVE:
			{
				// ホントにクライアント領域外かチェック
				if (wparam != HTCLIENT)
				{
					POINT pt;
					pt.x = static_cast<short>(LOWORD(lparam));
					pt.y = static_cast<short>(HIWORD(lparam));
					::ScreenToClient(hwnd, &pt);

					PlatformEventArgs e;
					e.type = PlatformEventType::MouseMove;
					e.sender = this;
					e.mouse.button = MouseButton::None;
					e.mouse.wheelDelta = 0;
					e.mouse.x = (short)pt.x;
					e.mouse.y = (short)pt.y;
					e.mouse.moveX = (mLastMouseX >= 0) ? e.mouse.x - mLastMouseX : 0;
					e.mouse.moveY = (mLastMouseY >= 0) ? e.mouse.y - mLastMouseY : 0;
					e.mouse.inClientArea = false;
					NortifyEvent(e);

					mLastMouseX = e.mouse.x;
					mLastMouseY = e.mouse.y;

					*handled = true;
					return 0;
				}
			}
			///////////////////////////////////////////// マウスホイールが操作された
			case WM_MOUSEWHEEL:
			{
				PlatformEventArgs e;
				e.type = PlatformEventType::MouseWheel;
				e.sender = this;
				e.mouse.button = MouseButton::None;
				e.mouse.wheelDelta = GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA;
				e.mouse.x = static_cast< short >(LOWORD(lparam));
				e.mouse.y = static_cast< short >(HIWORD(lparam));
				e.mouse.moveX = (mLastMouseX >= 0) ? e.mouse.x - mLastMouseX : 0;
				e.mouse.moveY = (mLastMouseY >= 0) ? e.mouse.y - mLastMouseY : 0;
				NortifyEvent(e);

				mLastMouseX = e.mouse.x;
				mLastMouseY = e.mouse.y;

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// キー↓
			case WM_KEYDOWN:
			{
				PlatformEventArgs e;
				e.type = PlatformEventType::KeyDown;
				e.sender = this;
				e.key.keyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.key.isAlt = ::GetKeyState(VK_MENU) < 0;
				e.key.isShift = ::GetKeyState(VK_SHIFT) < 0;
				e.key.isControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// キー↑
			case WM_KEYUP:
			{
				PlatformEventArgs e;
				e.type = PlatformEventType::KeyUp;
				e.sender = this;
				e.key.keyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.key.isAlt = ::GetKeyState(VK_MENU) < 0;
				e.key.isShift = ::GetKeyState(VK_SHIFT) < 0;
				e.key.isControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// Alt + KeyDown
			case WM_SYSKEYDOWN:
			{
				PlatformEventArgs e;
				e.type = PlatformEventType::KeyDown;
				e.sender = this;
				e.key.keyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.key.isAlt = true;								// Alt on
				e.key.isShift = ::GetKeyState(VK_SHIFT) < 0;
				e.key.isControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// Alt + KeyUp
			case WM_SYSKEYUP:
			{
				PlatformEventArgs e;
				e.type = PlatformEventType::KeyUp;
				e.sender = this;
				e.key.keyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.key.isAlt = true;								// Alt on
				e.key.isShift = ::GetKeyState(VK_SHIFT) < 0;
				e.key.isControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);
				break;	// WM_SYSKEYUPを捕まえた場合、必ずDefWindowProcに行くようにする
			}
			///////////////////////////////////////////// 文字入力
			case WM_CHAR:
			{
				// 文字のみ送る
				if (0x20 <= wparam && wparam <= 0x7E)
				{
					PlatformEventArgs e;
					e.type = PlatformEventType::KeyChar;
					e.sender = this;
					e.key.keyCode = Key::Unknown;
					e.key.isAlt = ::GetKeyState(VK_MENU) < 0;
					e.key.isShift = ::GetKeyState(VK_SHIFT) < 0;
					e.key.isControl = ::GetKeyState(VK_CONTROL) < 0;
					e.key.keyChar = wparam;
					NortifyEvent(e);
					return 0;
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Win32PlatformWindow::NortifyEvent(const PlatformEventArgs& e)
{
	/*	マウス非表示はもっと上のレベルで共通処理できるかと思ったけど、
		割とOSにより変わりそうなので (ウィンドウ上にあるときだけカーソルが変わるのかとか)
		このあたりでやってしまう。
	*/

	// 非アクティブの場合はクライアント領域外で移動したことにして、カーソルを表示する
	if (!mIsActive)
	{
		m_mouseCursorVisibility->OnMoveCursor(false);
	}
	// クライアント上移動によるマウスカーソル非表示処理
	else if (e.type == PlatformEventType::MouseMove)
	{
		if (e.mouse.inClientArea) {
			m_mouseCursorVisibility->OnMoveCursor(true);
		}
		else {
			m_mouseCursorVisibility->OnMoveCursor(false);
		}
	}

	// 時間経過によるマウスカーソルの非表示処理
	bool mc_visible = m_mouseCursorVisibility->CheckVisible();
	if (mc_visible != m_systemMouseShown)
	{
		if (mc_visible)
		{
			::ShowCursor(TRUE);
		}
		else
		{
			::ShowCursor(FALSE);
		}
		m_systemMouseShown = mc_visible;
	}

	return SendEventToAllListener(e);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Key Win32PlatformWindow::ConvertVirtualKeyCode(DWORD winVK)
{
	if ('A' <= winVK && winVK <= 'Z') return (Key)((int)Key::A + (winVK - 'A'));
	if ('0' <= winVK && winVK <= '9') return (Key)((int)Key::D0 + (winVK - '0'));
	if (VK_F1 <= winVK && winVK <= VK_F12) return (Key)((int)Key::F1 + (winVK - VK_F1));

	switch (winVK)
	{
	case VK_SPACE:
		return Key::Space;
	case VK_ESCAPE:
		return Key::Escape;
	case VK_UP:
		return Key::Up;
	case VK_DOWN:
		return Key::Down;
	case VK_LEFT:
		return Key::Left;
	case VK_RIGHT:
		return Key::Right;
	case VK_LSHIFT:
		return Key::LShift;
	case VK_RSHIFT:
		return Key::RShift;
	case VK_LCONTROL:
		return Key::LCtrl;
	case VK_RCONTROL:
		return Key::RCtrl;
	case VK_LMENU:
		return Key::LAlt;
	case VK_RMENU:
		return Key::RAlt;
	case VK_TAB:
		return Key::Tab;
	case VK_RETURN:
		return Key::Enter;
	case VK_BACK:
		return Key::BackSpace;
	case VK_INSERT:
		return Key::Insert;
	case VK_DELETE:
		return Key::Delete;
	case VK_PRIOR:
		return Key::PageUp;
	case VK_NEXT:
		return Key::PageDown;
	case VK_HOME:
		return Key::Home;
	case VK_END:
		return Key::End;

	case VK_OEM_1:
		return Key::Colon;
	case VK_OEM_PLUS:
		return Key::SemiColon;
	case VK_OEM_COMMA:
		return Key::Comma;
	case VK_OEM_PERIOD:
		return Key::Period;
	case VK_OEM_2:
		return Key::Slash;
	case VK_OEM_MINUS:
		return Key::Minus;
	case VK_OEM_102:
		return Key::BackSlash;
	case VK_OEM_5:
		return Key::Yen;
	case VK_OEM_7:
		return Key::Caret;
	case VK_OEM_4:
		return Key::LBracket;
	case VK_OEM_6:
		return Key::RBracket;
		/*
		case 'A': return Key::A;
		case 'B': return Key::B;
		case 'C': return Key::C;
		case 'D': return Key::D;
		case 'E': return Key::E;
		case 'F': return Key::F;
		case 'G': return Key::G;
		case 'H': return Key::H;
		case 'I': return Key::I;
		case 'J': return Key::J;
		case 'K': return Key::K;
		case 'L': return Key::L;
		case 'M': return Key::M;
		case 'N': return Key::N;
		case 'O': return Key::O;
		case 'P': return Key::P;
		case 'Q': return Key::Q;
		case 'R': return Key::R;
		case 'S': return Key::S;
		case 'T': return Key::T;
		case 'U': return Key::U;
		case 'V': return Key::V;
		case 'W': return Key::W;
		case 'X': return Key::X;
		case 'Y': return Key::Y;
		case 'Z': return Key::Z;

		Key::0                    = '0',
		Key::1                    = '1',
		Key::2                    = '2',
		Key::3                    = '3',
		Key::4                    = '4',
		Key::5                    = '5',
		Key::6                    = '6',
		Key::7                    = '7',
		Key::8                    = '8',
		Key::9                    = '9',

		Key::F1					= VK_F1,
		Key::F2					= VK_F2,
		Key::F3					= VK_F3,
		Key::F4					= VK_F4,
		Key::F5					= VK_F5,
		Key::F6					= VK_F6,
		Key::F7					= VK_F7,
		Key::F8					= VK_F8,
		Key::F9					= VK_F9,
		Key::F10					= VK_F10,
		Key::F11					= VK_F11,
		Key::F12					= VK_F12,
		*/
	}
	return Key::Unknown;
}

//=============================================================================
// Win32NativeWindow
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32NativeWindow::Win32NativeWindow(Win32WindowManager* windowManager)
	: Win32PlatformWindow(windowManager)
	, mTitleText()
	, mWindowHandle(NULL)
	, mAccelerators(NULL)
	, mWindowedStyle(NULL)
	, mFullScreen(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32NativeWindow::~Win32NativeWindow()
{
	if (mWindowHandle)
	{
		::DestroyWindow(mWindowHandle);
		mWindowHandle = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32NativeWindow::Initilaize(
	Win32WindowManager* windowManager,
	String windowTitle,
	int width,
	int height,
	bool fullscreen,
	bool resizable)
{
	LN_CHECK_ARGS_RETURN(windowManager != nullptr);


	mTitleText = windowTitle;
	// 初期化時のサイズ記憶
	mClientSize.width = width;
	mClientSize.height = height;
	m_originalClientSize = mClientSize;

	/* もともとは Win32Window クラスで生成していたが、
	* RegisterClassEx() を Manager に移動したことで、ウィンドウスタイルなどの情報も Manager に移った。
	* ウィンドウも Manager で生成して、HWND を Win32Window クラスに渡したほうが
	* シンプルに実装できると見込んだためこちらに移動した。
	*
	*	↑やっぱり混乱する。
	*/

	// ウィンドウモードのときのウィンドウスタイルの選択
	DWORD mWindowedStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	if (resizable) {
		mWindowedStyle |= (WS_THICKFRAME | WS_MAXIMIZEBOX);
	}
	DWORD dwExStyle = 0;
	if (windowManager->GetHIcon() == NULL) {
		dwExStyle |= WS_EX_DLGMODALFRAME;	// アイコンの無いスタイル
	}

	// ウィンドウの作成
	mWindowHandle = ::CreateWindowEx(
		dwExStyle,
		windowManager->GetWindowClassName().c_str(),
		windowTitle.c_str(),
		mWindowedStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, windowManager->GetInstanceHandle(), NULL);
	LN_THROW(mWindowHandle, Win32Exception, GetLastError());

	// アクセラレータの作成 (Alt+Enter の警告音を消す)
	ACCEL accels[1] =
	{
		{ FALT | FVIRTKEY, VK_RETURN, 0 }
	};
	mAccelerators = ::CreateAcceleratorTable(accels, 1);
	LN_THROW(mAccelerators, Win32Exception, GetLastError());

	// ウィンドウサイズをクライアント領域サイズから再設定
	Win32WindowManager::SetWindowClientSize(mWindowHandle, Size(width, height));
	Win32WindowManager::AbjustLocationCentering(mWindowHandle);

	// WM_PAINTが呼ばれないようにする
	::ValidateRect(mWindowHandle, 0);

	// ウィンドウハンドルと Win32Window のポインタを関連付ける
	BOOL r = ::SetProp(mWindowHandle, Win32WindowManager::PROP_WINPROC, this);
	LN_THROW((r != FALSE), Win32Exception, GetLastError());

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32NativeWindow::SetVisible(bool visible)
{
	::ShowWindow(mWindowHandle, (visible) ? SW_SHOW : SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32NativeWindow::SetFullScreenEnabled(bool flag)
{
	// フルスクリーンにする場合
	if (flag)
	{
		mFullScreen = true;
		::SetWindowLong(mWindowHandle, GWL_STYLE, Win32WindowManager::FULLSCREEN_STYLE);
		::SetWindowLong(mWindowHandle, GWL_EXSTYLE, 0);
		//Resize(true);

		// 画面いっぱい
		mClientSize.width = ::GetSystemMetrics(SM_CXSCREEN);
		mClientSize.height = ::GetSystemMetrics(SM_CYSCREEN);

		::SetForegroundWindow(mWindowHandle);

		int x = (::GetSystemMetrics(SM_CXSCREEN) - mClientSize.width) / 2;
		int y = (::GetSystemMetrics(SM_CYSCREEN) - mClientSize.height) / 2;

		//res = ::SetWindowPos( mWindowHandle, HWND_TOP, 0, 0, mWidth, mHeight, SWP_SHOWWINDOW );
		::SetWindowPos(mWindowHandle, NULL, x, y, mClientSize.width, mClientSize.height, SWP_SHOWWINDOW);
	}
	// ウィンドウモードにする場合
	else
	{
		mFullScreen = false;
		::SetWindowLong(mWindowHandle, GWL_STYLE, mWindowedStyle);
		Win32WindowManager::SetWindowClientSize(mWindowHandle, m_originalClientSize);
		Win32WindowManager::AbjustLocationCentering(mWindowHandle);
		mClientSize = m_originalClientSize;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32NativeWindow::CaptureMouse()
{
	::SetCapture(mWindowHandle);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32NativeWindow::ReleaseMouseCapture()
{
	::ReleaseCapture();
}

//=============================================================================
// Win32UserHostWindow
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32UserHostWindow::Win32UserHostWindow(Win32WindowManager* windowManager, HWND hWnd)
	: Win32PlatformWindow(windowManager)
	, m_hWnd(hWnd)
	, m_clientSize()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32UserHostWindow::~Win32UserHostWindow()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& Win32UserHostWindow::GetSize() const
{
	// コンストラクタで mClientSize に格納しておいてもいいと思ったけど、
	// フルスクリーン化等でウィンドウサイズが変わった時の対応が面倒そうなのでこのまま。
	RECT rc = { 0, 0, 0, 0 };
	::GetClientRect(m_hWnd, &rc);
	m_clientSize.Set(rc.right, rc.bottom);
	return m_clientSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32UserHostWindow::CaptureMouse()
{
	::SetCapture(m_hWnd);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32UserHostWindow::ReleaseMouseCapture()
{
	::ReleaseCapture();
}

LN_NAMESPACE_END
