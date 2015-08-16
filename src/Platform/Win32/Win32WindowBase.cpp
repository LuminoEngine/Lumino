
/*
	[2015/01/10] メッセージ処理は非同期にするべきか？

		するべきではない。ウィンドウドラッグ中に描画更新などができるが、
		煩雑なメッセージ管理やコールバックが必要で、メリットよりもデメリットの方が大きい。

		デメリット
		・イベントは必ず PostEvent で一度メインスレッドに渡した後、
		  メイン側から各 Window にアタッチされている Listener に送信する必要がある。少し複雑。
		・PostEvent しなければならないということは、キューに入れるイベントデータは
		  new したオブジェクトかサイズ固定の構造体でなければならない。
		  イベント1つごとに new はコストが高いし、サイズ固定構造体は拡張性が無い。
		・Closing イベントでアプリの終了確認メッセージボックスとかが出せない。
		  ツール系ではメインスレッドが動いたままだと都合が悪い。
		  →ゲームならまぁいいけど。
*/


#include "../../Internal.h"
#include "Win32WindowManager.h"
#include "Win32WindowBase.h"

namespace Lumino
{
namespace Platform
{

//=============================================================================
// Win32WindowBase
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32WindowBase::Win32WindowBase(Win32WindowManager* app)
	: WindowBase(app)
	, mLastMouseX(-1)
	, mLastMouseY(-1)
	, mIsActive(true)	// 初期値 true。WM_ACTIVATE は初回表示で最前面になった時は呼ばれない
	, m_systemMouseShown(true)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32WindowBase::~Win32WindowBase()
{
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void Win32WindowBase::HideCursor()
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
//void Win32WindowBase::ShowCursor()
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
LRESULT Win32WindowBase::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled)
{
	*handled = false;

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

				EventArgs e(EventType_Close, this);
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

					//EventArgs e;
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
				mClientSize.Set(lparam & 0xFFFF, (lparam >> 16) & 0xFFFF);



				// ウィンドウサイズを拾っておく
				//RECT rc;
				//::GetClientRect(mWindowHandle, &rc);
				//window->mClientSize.Set(rc.right, rc.bottom);

				//EventArgs e;
				//e.Type = LN_EVENT_WINDOW_SIZE_CHANGED;
				//e.Sender = this;
				//SendEventToAllListener(e);		// 同期処理の場合はこの場で通知
				//mApplication->PostEvent(&e);	// 非同期処理の場合は一度キューに入れる

				//*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// Alt + Enter 確認
			//case WM_SYSKEYDOWN:
			//{
			//	if ( wparam == VK_RETURN )
			//	{
			//		EventArgs e;
			//		e.Type			= LN_EVENT_ALTENTER;
			//		e.Sender		= this;
			//		e.StructSize	= sizeof(EventArgs);
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
			{
				EventArgs e;
				e.Sender = this;

				switch (msg)
				{
				case WM_LBUTTONDOWN:
					e.Type = EventType_MouseDown;
					e.Mouse.Button = MouseButton_Left;
					break;
				case WM_LBUTTONUP:
					e.Type = EventType_MouseUp;
					e.Mouse.Button = MouseButton_Left;
					break;
				case WM_RBUTTONDOWN:
					e.Type = EventType_MouseDown;
					e.Mouse.Button = MouseButton_Right;
					break;
				case WM_RBUTTONUP:
					e.Type = EventType_MouseUp;
					e.Mouse.Button = MouseButton_Right;
					break;
				case WM_MBUTTONDOWN:
					e.Type = EventType_MouseDown;
					e.Mouse.Button = MouseButton_Middle;
					break;
				case WM_MBUTTONUP:
					e.Type = EventType_MouseUp;
					e.Mouse.Button = MouseButton_Middle;
					break;
				}

				e.Mouse.X = LOWORD(lparam);
				e.Mouse.Y = HIWORD(lparam);
				e.Mouse.Delta = 0;
				e.Mouse.MoveX = (mLastMouseX >= 0) ? e.Mouse.X - mLastMouseX : 0;
				e.Mouse.MoveY = (mLastMouseY >= 0) ? e.Mouse.Y - mLastMouseY : 0;
				NortifyEvent(e);

				mLastMouseX = e.Mouse.X;
				mLastMouseY = e.Mouse.Y;

				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// マウス移動
			case WM_MOUSEMOVE:
			{
				EventArgs e;
				e.Type = EventType_MouseMove;
				e.Sender = this;
				e.Mouse.Button = MouseButton_None;
				e.Mouse.Delta = 0;
				e.Mouse.X = static_cast< short >(LOWORD(lparam));     // 一度 short にキャストしないと、
				e.Mouse.Y = static_cast< short >(HIWORD(lparam));     // マイナス値になったとき 65535 とか値が入る
				e.Mouse.MoveX = (mLastMouseX >= 0) ? e.Mouse.X - mLastMouseX : 0;
				e.Mouse.MoveY = (mLastMouseY >= 0) ? e.Mouse.Y - mLastMouseY : 0;
				e.Mouse.InClientArea = true;
				NortifyEvent(e);

				mLastMouseX = e.Mouse.X;
				mLastMouseY = e.Mouse.Y;

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

					EventArgs e;
					e.Type = EventType_MouseMove;
					e.Sender = this;
					e.Mouse.Button = MouseButton_None;
					e.Mouse.Delta = 0;
					e.Mouse.X = pt.x;
					e.Mouse.Y = pt.y;
					e.Mouse.MoveX = (mLastMouseX >= 0) ? e.Mouse.X - mLastMouseX : 0;
					e.Mouse.MoveY = (mLastMouseY >= 0) ? e.Mouse.Y - mLastMouseY : 0;
					e.Mouse.InClientArea = false;
					NortifyEvent(e);

					mLastMouseX = e.Mouse.X;
					mLastMouseY = e.Mouse.Y;

					*handled = true;
					return 0;
				}
			}
			///////////////////////////////////////////// マウスホイールが操作された
			case WM_MOUSEWHEEL:
			{
				EventArgs e;
				e.Type = EventType_MouseWheel;
				e.Sender = this;
				e.Mouse.Button = MouseButton_None;
				e.Mouse.Delta = GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA;
				e.Mouse.X = static_cast< short >(LOWORD(lparam));
				e.Mouse.Y = static_cast< short >(HIWORD(lparam));
				e.Mouse.MoveX = (mLastMouseX >= 0) ? e.Mouse.X - mLastMouseX : 0;
				e.Mouse.MoveY = (mLastMouseY >= 0) ? e.Mouse.Y - mLastMouseY : 0;
				NortifyEvent(e);

				mLastMouseX = e.Mouse.X;
				mLastMouseY = e.Mouse.Y;

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// キー↓
			case WM_KEYDOWN:
			{
				EventArgs e;
				e.Type = EventType_KeyDown;
				e.Sender = this;
				e.Key.KeyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.Key.IsAlt = ::GetKeyState(VK_MENU) < 0;
				e.Key.IsShift = ::GetKeyState(VK_SHIFT) < 0;
				e.Key.IsControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// キー↑
			case WM_KEYUP:
			{
				EventArgs e;
				e.Type = EventType_KeyUp;
				e.Sender = this;
				e.Key.KeyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.Key.IsAlt = ::GetKeyState(VK_MENU) < 0;
				e.Key.IsShift = ::GetKeyState(VK_SHIFT) < 0;
				e.Key.IsControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// Alt + KeyDown
			case WM_SYSKEYDOWN:
			{
				EventArgs e;
				e.Type = EventType_KeyDown;
				e.Sender = this;
				e.Key.KeyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.Key.IsAlt = true;								// Alt on
				e.Key.IsShift = ::GetKeyState(VK_SHIFT) < 0;
				e.Key.IsControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);

				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// Alt + KeyUp
			case WM_SYSKEYUP:
			{
				EventArgs e;
				e.Type = EventType_KeyUp;
				e.Sender = this;
				e.Key.KeyCode = ConvertVirtualKeyCode(wparam);	// 仮想キーコード
				e.Key.IsAlt = true;								// Alt on
				e.Key.IsShift = ::GetKeyState(VK_SHIFT) < 0;
				e.Key.IsControl = ::GetKeyState(VK_CONTROL) < 0;
				NortifyEvent(e);
				break;	// WM_SYSKEYUPを捕まえた場合、必ずDefWindowProcに行くようにする
			}
			///////////////////////////////////////////// 文字入力
			//case WM_CHAR:
			//{
			//	// 文字のみ送る
			//	if (0x20 <= wparam && wparam <= 0x7E)
			//	{
			//		KeyEventArgs e;
			//		e.Type = EventType_KeyChar;
			//		e.Sender = this;
			//		e.KeyCode = (Key)wparam;
			//		e.IsAlt = ::GetKeyState(VK_MENU) < 0;
			//		e.IsShift = ::GetKeyState(VK_SHIFT) < 0;
			//		e.IsControl = ::GetKeyState(VK_CONTROL) < 0;
			//		mWindowManager->getManager()->postEventFromWindowThread((EventArgs*)&e);

			//		return 0;
			//	}
			//}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Win32WindowBase::NortifyEvent(const EventArgs& e)
{
	/*	マウス非表示はもっと上のレベルで共通処理できるかと思ったけど、
		割とOSにより変わりそうなので (ウィンドウ上にあるときだけカーソルが変わるのかとか)
		このあたりでやってしまう。
	*/

	// 非アクティブの場合はクライアント領域外で移動したことにして、カーソルを表示する
	if (!mIsActive)
	{
		m_mouseCursorVisibility.OnMoveCursor(false);
	}
	// クライアント上移動によるマウスカーソル非表示処理
	else if (e.Type == EventType_MouseMove)
	{
		if (e.Mouse.InClientArea) {
			m_mouseCursorVisibility.OnMoveCursor(true);
		}
		else {
			m_mouseCursorVisibility.OnMoveCursor(false);
		}
	}

	// 時間経過によるマウスカーソルの非表示処理
	bool mc_visible = m_mouseCursorVisibility.CheckVisible();
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
Key Win32WindowBase::ConvertVirtualKeyCode(DWORD winVK)
{
	if ('A' <= winVK && winVK <= 'Z') return (Key)(Key_A + (winVK - 'A'));
	if ('0' <= winVK && winVK <= '9') return (Key)(Key_0 + (winVK - '0'));
	if (VK_F1 <= winVK && winVK <= VK_F12) return (Key)(Key_F1 + (winVK - VK_F1));

	switch (winVK)
	{
	case VK_SPACE:
		return Key_Space;
	case VK_ESCAPE:
		return Key_Escape;
	case VK_UP:
		return Key_Up;
	case VK_DOWN:
		return Key_Down;
	case VK_LEFT:
		return Key_Left;
	case VK_RIGHT:
		return Key_Right;
	case VK_LSHIFT:
		return Key_LShift;
	case VK_RSHIFT:
		return Key_RShift;
	case VK_LCONTROL:
		return Key_LCtrl;
	case VK_RCONTROL:
		return Key_RCtrl;
	case VK_LMENU:
		return Key_LAlt;
	case VK_RMENU:
		return Key_RAlt;
	case VK_TAB:
		return Key_Tab;
	case VK_RETURN:
		return Key_Enter;
	case VK_BACK:
		return Key_BackSpace;
	case VK_INSERT:
		return Key_Insert;
	case VK_DELETE:
		return Key_Delete;
	case VK_PRIOR:
		return Key_PageUp;
	case VK_NEXT:
		return Key_PageDown;
	case VK_HOME:
		return Key_Home;
	case VK_END:
		return Key_End;

	case VK_OEM_1:
		return Key_Colon;
	case VK_OEM_PLUS:
		return Key_SemiColon;
	case VK_OEM_COMMA:
		return Key_Comma;
	case VK_OEM_PERIOD:
		return Key_Period;
	case VK_OEM_2:
		return Key_Slash;
	case VK_OEM_MINUS:
		return Key_Minus;
	case VK_OEM_102:
		return Key_BackSlash;
	case VK_OEM_5:
		return Key_Yen;
	case VK_OEM_7:
		return Key_Caret;
	case VK_OEM_4:
		return Key_LBracket;
	case VK_OEM_6:
		return Key_RBracket;
		/*
		case 'A': return Key_A;
		case 'B': return Key_B;
		case 'C': return Key_C;
		case 'D': return Key_D;
		case 'E': return Key_E;
		case 'F': return Key_F;
		case 'G': return Key_G;
		case 'H': return Key_H;
		case 'I': return Key_I;
		case 'J': return Key_J;
		case 'K': return Key_K;
		case 'L': return Key_L;
		case 'M': return Key_M;
		case 'N': return Key_N;
		case 'O': return Key_O;
		case 'P': return Key_P;
		case 'Q': return Key_Q;
		case 'R': return Key_R;
		case 'S': return Key_S;
		case 'T': return Key_T;
		case 'U': return Key_U;
		case 'V': return Key_V;
		case 'W': return Key_W;
		case 'X': return Key_X;
		case 'Y': return Key_Y;
		case 'Z': return Key_Z;

		Key_0                    = '0',
		Key_1                    = '1',
		Key_2                    = '2',
		Key_3                    = '3',
		Key_4                    = '4',
		Key_5                    = '5',
		Key_6                    = '6',
		Key_7                    = '7',
		Key_8                    = '8',
		Key_9                    = '9',

		Key_F1					= VK_F1,
		Key_F2					= VK_F2,
		Key_F3					= VK_F3,
		Key_F4					= VK_F4,
		Key_F5					= VK_F5,
		Key_F6					= VK_F6,
		Key_F7					= VK_F7,
		Key_F8					= VK_F8,
		Key_F9					= VK_F9,
		Key_F10					= VK_F10,
		Key_F11					= VK_F11,
		Key_F12					= VK_F12,
		*/
	}
	return Key_Unknown;
}


} // namespace Platform
} // namespace Lumino