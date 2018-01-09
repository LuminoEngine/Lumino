
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

#include "../Internal.h"
#include <Lumino/Platform/Win32/Win32PlatformWindow.h>
#include "Win32PlatformWindowManager.h"
#include "../MouseCursorVisibility.h"

LN_NAMESPACE_BEGIN


class DataObjectUtils
{
public:
	/*
		IDataObject からフォーマットを取り出すには？

		フォーマットID は CF_TEXT(1) CF_HDROP(15) 等が基本として用意されている。
		また、独自フォーマットを定義することもできる。

		独自フォーマットまで取りたいときは IDataObject::EnumFormatEtc() で、データについている ID を列挙して調べる必要がある。
		単にエクスプローラからファイルを D&D しただけでも、	15 のほか、49340 とか 49977 とかたくさん得られる。

		IDataObject::EnumFormatEtc() は少し冗長なので、簡単に調べたければこれが参考になる。
		https://chromium.googlesource.com/chromium/src/base/+/a57992ebb30cb43a4c1fbb1c30ed470e89afebfd/clipboard_util.cc
	*/

	static FORMATETC* GetCFHDropFormat();
	static bool GetFilenames(IDataObject* data_object, List<String>* filenames);
};

FORMATETC* DataObjectUtils::GetCFHDropFormat()
{
	static FORMATETC format = { CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	return &format;
}

bool DataObjectUtils::GetFilenames(IDataObject* data_object, List<String>* filenames)
{
	STGMEDIUM medium;
	if (FAILED(data_object->GetData(GetCFHDropFormat(), &medium)))
	{
		return false;
	}

	HDROP hdrop = static_cast<HDROP>(GlobalLock(medium.hGlobal));
	if (!hdrop)
	{
		return false;
	}

	const int MaxFilePathLen = 4096;
	int count = DragQueryFileW(hdrop, 0xffffffff, 0, 0);
	for (int i = 0; i < count; i++)
	{
		wchar_t filepath[MaxFilePathLen];
		if (!DragQueryFileW(hdrop, i, filepath, MaxFilePathLen))
		{
			continue;
		}
		filenames->add(filepath);
	}
	DragFinish(hdrop);
	GlobalUnlock(medium.hGlobal);

	return true;
}







class OleDropTarget : public IDropTarget
{
public:
	OleDropTarget(Win32PlatformWindow* window);
	virtual ~OleDropTarget();

	void Register();
	void Revoke();

	// IUnknown
	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppvObj);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IDropTarget
	STDMETHOD(DragEnter)(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragOver)(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragLeave)();
	STDMETHOD(Drop)(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);

private:

	Win32PlatformWindow* m_window;
	ULONG m_refs;
	BOOL m_bSupportFormat;
};




OleDropTarget::OleDropTarget(Win32PlatformWindow* window)
	: m_window(window)
	, m_refs(1)
	, m_bSupportFormat(FALSE)
{
}

OleDropTarget::~OleDropTarget()
{
}

void OleDropTarget::Register()
{
	::RegisterDragDrop(m_window->getWindowHandle(), this);
}

void OleDropTarget::Revoke()
{
	::RevokeDragDrop(m_window->getWindowHandle());
}

STDMETHODIMP
OleDropTarget::QueryInterface(REFIID iid, void FAR* FAR* ppv)
{
	if (IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IDropTarget))
	{
		*ppv = this;
		AddRef();
		return NOERROR;
	}
	*ppv = NULL;
	return ResultFromScode(E_NOINTERFACE);
}

STDMETHODIMP_(ULONG)
OleDropTarget::AddRef()
{
	return ++m_refs;
}

STDMETHODIMP_(ULONG)
OleDropTarget::Release()
{
	if (--m_refs == 0)
	{
		delete this;
		return 0;
	}
	return m_refs;
}

STDMETHODIMP
OleDropTarget::DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	HRESULT   hr;
	FORMATETC formatetc;

	*pdwEffect = DROPEFFECT_COPY;

	//formatetc.cfFormat = 0;
	//formatetc.ptd = NULL;
	//formatetc.dwAspect = DVASPECT_CONTENT;
	//formatetc.lindex = -1;
	//formatetc.tymed = TYMED_ENHMF;

	hr = pDataObj->QueryGetData(DataObjectUtils::GetCFHDropFormat());
	if (hr != S_OK) {
		m_bSupportFormat = FALSE;
		*pdwEffect = DROPEFFECT_NONE;
	}
	else
		m_bSupportFormat = TRUE;

	return S_OK;
}

STDMETHODIMP
OleDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	printf("DragOver\n");
	*pdwEffect = DROPEFFECT_COPY;

	if (!m_bSupportFormat)
		*pdwEffect = DROPEFFECT_NONE;

	return S_OK;
}

STDMETHODIMP
OleDropTarget::DragLeave()
{
	return S_OK;
}

STDMETHODIMP
OleDropTarget::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	printf("Drop\n");
	//HRESULT   hr;
	//FORMATETC formatetc;
	//STGMEDIUM medium;

	//*pdwEffect = DROPEFFECT_COPY;

	//formatetc.cfFormat = CF_ENHMETAFILE;
	//formatetc.ptd = NULL;
	//formatetc.dwAspect = DVASPECT_CONTENT;
	//formatetc.lindex = -1;
	//formatetc.tymed = TYMED_ENHMF;

	//hr = pDataObj->GetData(&formatetc, &medium);
	//if (FAILED(hr)) {
	//	*pdwEffect = DROPEFFECT_NONE;
	//	return E_FAIL;
	//}

	//if (g_nDropDataCount < g_nDropDataMaxCount) {
	//	g_dropData[g_nDropDataCount].hEnhMetaFile = medium.hEnhMetaFile;
	//	g_dropData[g_nDropDataCount].pt.x = pt.x;
	//	g_dropData[g_nDropDataCount].pt.y = pt.y;
	//	ScreenToClient(m_hwnd, &g_dropData[g_nDropDataCount].pt);
	//	g_nDropDataCount++;
	//	InvalidateRect(m_hwnd, NULL, FALSE);
	//}

	List<String> filePathes;
	DataObjectUtils::GetFilenames(pDataObj, &filePathes);

	for (auto& f : filePathes)
	{
		Console::writeLine(f);
	}

	return S_OK;
}

//==============================================================================
// Win32PlatformWindow
//==============================================================================

Win32PlatformWindow::Win32PlatformWindow(Win32WindowManager* app)
	: PlatformWindow(app)
	, mLastMouseX(-1)
	, mLastMouseY(-1)
	, m_systemMouseShown(true)
	, m_dropTarget(nullptr)
{
}

Win32PlatformWindow::~Win32PlatformWindow()
{
	if (m_dropTarget)
		m_dropTarget->Release();
}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void Win32PlatformWindow::HideCursor()
//{
//	if (m_cursorShown)
//	{
//		::ShowCursor(FALSE);
//		m_cursorShown = false;
//	}
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void Win32PlatformWindow::ShowCursor()
//{
//	if (!m_cursorShown)
//	{
//		::ShowCursor(TRUE);
//		m_cursorShown = true;
//	}
//}

//------------------------------------------------------------------------------
LRESULT Win32PlatformWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled)
{
	*handled = false;

	if (!UserWndProc.isEmpty())
	{
		LRESULT dr = UserWndProc.call(hwnd, msg, wparam, lparam, handled);//raiseDelegateEvent(PreWndProc, hwnd, msg, wparam, lparam, handled);
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
		case WM_XBUTTONDOWN:
			::SetCapture(hwnd);
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
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
				sendPlatformEvent(PlatformEventArgs::makeClosingEvent(this));
				*handled = true;
				return 0;
			}

			/////////////////// ウィンドウがアクティブ・非アクティブになった場合
			case WM_ACTIVATE:
			{
				sendPlatformEvent(PlatformEventArgs::makeActivateChangedEvent(this, ((wparam & 0xffff) != 0)));
				*handled = true;
				return 0;
			}
			/////////////////////////////////////////////// ウィンドウサイズが変更された
			case WM_SIZE:
			{
				sendPlatformEvent(PlatformEventArgs::makeWindowSizeChangedEvent(this, lparam & 0xFFFF, (lparam >> 16) & 0xFFFF));
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
					e.mouse.button = MouseButtons::Left;
					break;
				case WM_LBUTTONUP:
					e.type = PlatformEventType::MouseUp;
					e.mouse.button = MouseButtons::Left;
					break;
				case WM_RBUTTONDOWN:
					e.type = PlatformEventType::MouseDown;
					e.mouse.button = MouseButtons::Right;
					break;
				case WM_RBUTTONUP:
					e.type = PlatformEventType::MouseUp;
					e.mouse.button = MouseButtons::Right;
					break;
				case WM_MBUTTONDOWN:
					e.type = PlatformEventType::MouseDown;
					e.mouse.button = MouseButtons::Middle;
					break;
				case WM_MBUTTONUP:
					e.type = PlatformEventType::MouseUp;
					e.mouse.button = MouseButtons::Middle;
					break;
				case WM_XBUTTONDOWN:
					e.type = PlatformEventType::MouseDown;
					e.mouse.button = (wparam & MK_XBUTTON1) ? MouseButtons::X1 : MouseButtons::X2;
					break;
				case WM_XBUTTONUP:
					e.type = PlatformEventType::MouseUp;
					e.mouse.button = (wparam & MK_XBUTTON1) ? MouseButtons::X1 : MouseButtons::X2;
					break;
				}

				e.mouse.x = LOWORD(lparam);
				e.mouse.y = HIWORD(lparam);
				e.mouse.moveX = (mLastMouseX >= 0) ? e.mouse.x - mLastMouseX : 0;
				e.mouse.moveY = (mLastMouseY >= 0) ? e.mouse.y - mLastMouseY : 0;
				sendPlatformEvent(e);

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
				e.mouse.button = MouseButtons::None;
				e.mouse.x = static_cast< short >(LOWORD(lparam));     // 一度 short にキャストしないと、
				e.mouse.y = static_cast< short >(HIWORD(lparam));     // マイナス値になったとき 65535 とか値が入る
				e.mouse.moveX = (mLastMouseX >= 0) ? e.mouse.x - mLastMouseX : 0;
				e.mouse.moveY = (mLastMouseY >= 0) ? e.mouse.y - mLastMouseY : 0;
				e.mouse.inClientArea = true;
				sendPlatformEvent(e);

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
					e.mouse.button = MouseButtons::None;
					e.mouse.x = (short)pt.x;
					e.mouse.y = (short)pt.y;
					e.mouse.moveX = (mLastMouseX >= 0) ? e.mouse.x - mLastMouseX : 0;
					e.mouse.moveY = (mLastMouseY >= 0) ? e.mouse.y - mLastMouseY : 0;
					e.mouse.inClientArea = false;
					sendPlatformEvent(e);

					mLastMouseX = e.mouse.x;
					mLastMouseY = e.mouse.y;

					*handled = true;
					return 0;
				}
			}
			///////////////////////////////////////////// マウスホイールが操作された
			case WM_MOUSEWHEEL:
			{
				sendPlatformEvent(PlatformEventArgs::makeMouseWheelEvent(this, GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA));
				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// キー↓
			case WM_KEYDOWN:
			{
				ModifierKeys mods =
					(::GetKeyState(VK_MENU) < 0) ? ModifierKeys::Alt : ModifierKeys::None |
					(::GetKeyState(VK_SHIFT) < 0) ? ModifierKeys::Shift : ModifierKeys::None |
					(::GetKeyState(VK_CONTROL) < 0) ? ModifierKeys::Control : ModifierKeys::None;
				sendPlatformEvent(PlatformEventArgs::makeKeyEvent(this, PlatformEventType::KeyDown, convertVirtualKeyCode(wparam), mods, 0));
				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// キー↑
			case WM_KEYUP:
			{
				ModifierKeys mods =
					(::GetKeyState(VK_MENU) < 0) ? ModifierKeys::Alt : ModifierKeys::None |
					(::GetKeyState(VK_SHIFT) < 0) ? ModifierKeys::Shift : ModifierKeys::None |
					(::GetKeyState(VK_CONTROL) < 0) ? ModifierKeys::Control : ModifierKeys::None;
				sendPlatformEvent(PlatformEventArgs::makeKeyEvent(this, PlatformEventType::KeyUp, convertVirtualKeyCode(wparam), mods, 0));
				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// Alt + KeyDown
			case WM_SYSKEYDOWN:
			{
				// Alt は ON 扱い
				ModifierKeys mods =
					ModifierKeys::Alt |
					(::GetKeyState(VK_SHIFT) < 0) ? ModifierKeys::Shift : ModifierKeys::None |
					(::GetKeyState(VK_CONTROL) < 0) ? ModifierKeys::Control : ModifierKeys::None;
				sendPlatformEvent(PlatformEventArgs::makeKeyEvent(this, PlatformEventType::KeyDown, convertVirtualKeyCode(wparam), mods, 0));
				*handled = true;
				return 0;
			}
			///////////////////////////////////////////// Alt + KeyUp
			case WM_SYSKEYUP:
			{
				// Alt は ON 扱い
				ModifierKeys mods =
					ModifierKeys::Alt |
					(::GetKeyState(VK_SHIFT) < 0) ? ModifierKeys::Shift : ModifierKeys::None |
					(::GetKeyState(VK_CONTROL) < 0) ? ModifierKeys::Control : ModifierKeys::None;
				sendPlatformEvent(PlatformEventArgs::makeKeyEvent(this, PlatformEventType::KeyUp, convertVirtualKeyCode(wparam), mods, 0));
				break;	// WM_SYSKEYUPを捕まえた場合、必ずDefWindowProcに行くようにする
			}
			///////////////////////////////////////////// 文字入力
			case WM_CHAR:
			{
				// 文字のみ送る
				if (0x20 <= wparam && wparam <= 0x7E)
				{
					ModifierKeys mods =
						(::GetKeyState(VK_MENU) < 0) ? ModifierKeys::Alt : ModifierKeys::None |
						(::GetKeyState(VK_SHIFT) < 0) ? ModifierKeys::Shift : ModifierKeys::None |
						(::GetKeyState(VK_CONTROL) < 0) ? ModifierKeys::Control : ModifierKeys::None;
					sendPlatformEvent(PlatformEventArgs::makeKeyEvent(this, PlatformEventType::KeyChar, Keys::Unknown, mods, wparam));
					*handled = true;
					return 0;
				}
			}
		}
	}

	return 0;
}

//------------------------------------------------------------------------------
void Win32PlatformWindow::setSize(const SizeI& size)
{
	Win32WindowManager::setWindowClientSize(getWindowHandle(), size);
}

//------------------------------------------------------------------------------
void Win32PlatformWindow::onPlatformEvent(const PlatformEventArgs& e)
{
	/*	マウス非表示はもっと上のレベルで共通処理できるかと思ったけど、
		割とOSにより変わりそうなので (ウィンドウ上にあるときだけカーソルが変わるのかとか)
		このあたりでやってしまう。
	*/

	// 非アクティブの場合はクライアント領域外で移動したことにして、カーソルを表示する
	if (!isActive())
	{
		m_mouseCursorVisibility->onMoveCursor(false);
	}
	// クライアント上移動によるマウスカーソル非表示処理
	else if (e.type == PlatformEventType::MouseMove)
	{
		if (e.mouse.inClientArea) {
			m_mouseCursorVisibility->onMoveCursor(true);
		}
		else {
			m_mouseCursorVisibility->onMoveCursor(false);
		}
	}

	// 時間経過によるマウスカーソルの非表示処理
	bool mc_visible = m_mouseCursorVisibility->checkVisible();
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

	//return SendEventToAllListener(e);
}

void Win32PlatformWindow::setAllowDragDrop(bool value)
{
	if (!m_dropTarget)
	{
		m_dropTarget = new OleDropTarget(this);
	}

	if (value)
	{
		static_cast<OleDropTarget*>(m_dropTarget)->Register();
	}
	else
	{
		static_cast<OleDropTarget*>(m_dropTarget)->Revoke();
	}
}

//------------------------------------------------------------------------------
Keys Win32PlatformWindow::convertVirtualKeyCode(DWORD winVK)
{
	if ('A' <= winVK && winVK <= 'Z') return (Keys)((int)Keys::A + (winVK - 'A'));
	if ('0' <= winVK && winVK <= '9') return (Keys)((int)Keys::D0 + (winVK - '0'));
	if (VK_F1 <= winVK && winVK <= VK_F12) return (Keys)((int)Keys::F1 + (winVK - VK_F1));

	switch (winVK)
	{
	case VK_SPACE:
		return Keys::Space;
	case VK_ESCAPE:
		return Keys::Escape;
	case VK_UP:
		return Keys::Up;
	case VK_DOWN:
		return Keys::Down;
	case VK_LEFT:
		return Keys::Left;
	case VK_RIGHT:
		return Keys::Right;
	case VK_LSHIFT:
		return Keys::LShift;
	case VK_RSHIFT:
		return Keys::RShift;
	case VK_LCONTROL:
		return Keys::LCtrl;
	case VK_RCONTROL:
		return Keys::RCtrl;
	case VK_LMENU:
		return Keys::LAlt;
	case VK_RMENU:
		return Keys::RAlt;
	case VK_TAB:
		return Keys::Tab;
	case VK_RETURN:
		return Keys::Enter;
	case VK_BACK:
		return Keys::BackSpace;
	case VK_INSERT:
		return Keys::Insert;
	case VK_DELETE:
		return Keys::Delete;
	case VK_PRIOR:
		return Keys::PageUp;
	case VK_NEXT:
		return Keys::PageDown;
	case VK_HOME:
		return Keys::Home;
	case VK_END:
		return Keys::End;

	case VK_OEM_1:
		return Keys::Colon;
	case VK_OEM_PLUS:
		return Keys::Semicolon;
	case VK_OEM_COMMA:
		return Keys::Comma;
	case VK_OEM_PERIOD:
		return Keys::Period;
	case VK_OEM_2:
		return Keys::Slash;
	case VK_OEM_MINUS:
		return Keys::Minus;
	case VK_OEM_102:
		return Keys::Backslash;
	case VK_OEM_5:
		return Keys::Yen;
	case VK_OEM_7:
		return Keys::Caret;
	case VK_OEM_4:
		return Keys::LBracket;
	case VK_OEM_6:
		return Keys::RBracket;
		/*
		case 'A': return Keys::A;
		case 'B': return Keys::B;
		case 'C': return Keys::C;
		case 'D': return Keys::D;
		case 'E': return Keys::E;
		case 'F': return Keys::F;
		case 'G': return Keys::G;
		case 'H': return Keys::H;
		case 'I': return Keys::I;
		case 'J': return Keys::J;
		case 'K': return Keys::K;
		case 'L': return Keys::L;
		case 'M': return Keys::M;
		case 'N': return Keys::N;
		case 'O': return Keys::O;
		case 'P': return Keys::P;
		case 'Q': return Keys::Q;
		case 'R': return Keys::R;
		case 'S': return Keys::S;
		case 'T': return Keys::T;
		case 'U': return Keys::U;
		case 'V': return Keys::V;
		case 'W': return Keys::W;
		case 'X': return Keys::X;
		case 'Y': return Keys::Y;
		case 'Z': return Keys::Z;

		Keys::0                    = '0',
		Keys::1                    = '1',
		Keys::2                    = '2',
		Keys::3                    = '3',
		Keys::4                    = '4',
		Keys::5                    = '5',
		Keys::6                    = '6',
		Keys::7                    = '7',
		Keys::8                    = '8',
		Keys::9                    = '9',

		Keys::F1					= VK_F1,
		Keys::F2					= VK_F2,
		Keys::F3					= VK_F3,
		Keys::F4					= VK_F4,
		Keys::F5					= VK_F5,
		Keys::F6					= VK_F6,
		Keys::F7					= VK_F7,
		Keys::F8					= VK_F8,
		Keys::F9					= VK_F9,
		Keys::F10					= VK_F10,
		Keys::F11					= VK_F11,
		Keys::F12					= VK_F12,
		*/
	}
	return Keys::Unknown;
}

//==============================================================================
// Win32NativeWindow
//==============================================================================

//------------------------------------------------------------------------------
Win32NativeWindow::Win32NativeWindow(Win32WindowManager* windowManager)
	: Win32PlatformWindow(windowManager)
	, mTitleText()
	, mWindowHandle(NULL)
	, mAccelerators(NULL)
	, mWindowedStyle(NULL)
	, mFullScreen(false)
{
}

//------------------------------------------------------------------------------
Win32NativeWindow::~Win32NativeWindow()
{
	if (mWindowHandle)
	{
		::DestroyWindow(mWindowHandle);
		mWindowHandle = NULL;
	}
}

//------------------------------------------------------------------------------
void Win32NativeWindow::Initilaize(
	Win32WindowManager* windowManager,
	String windowTitle,
	int width,
	int height,
	bool fullscreen,
	bool resizable)
{
	if (LN_REQUIRE(windowManager != nullptr)) return;
	PlatformWindow::initialize(SizeI(width, height));

	mTitleText = windowTitle;
	// 初期化時のサイズ記憶
	m_originalClientSize.set(width, height);

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
	if (windowManager->getHIcon() == NULL) {
		dwExStyle |= WS_EX_DLGMODALFRAME;	// アイコンの無いスタイル
	}

	// ウィンドウの作成
	mWindowHandle = ::CreateWindowEx(
		dwExStyle,
		windowManager->getWindowClassName().c_str(),
		windowTitle.c_str(),
		mWindowedStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, windowManager->getInstanceHandle(), NULL);
	if (LN_ENSURE_WIN32(mWindowHandle, GetLastError())) return;

	// アクセラレータの作成 (Alt+Enter の警告音を消す)
	ACCEL accels[1] =
	{
		{ FALT | FVIRTKEY, VK_RETURN, 0 }
	};
	mAccelerators = ::CreateAcceleratorTable(accels, 1);
	if (LN_ENSURE_WIN32(mAccelerators, GetLastError())) return;

	// ウィンドウサイズをクライアント領域サイズから再設定
	Win32WindowManager::setWindowClientSize(mWindowHandle, SizeI(width, height));
	Win32WindowManager::abjustLocationCentering(mWindowHandle);

	// WM_PAINTが呼ばれないようにする
	::ValidateRect(mWindowHandle, 0);

	// ウィンドウハンドルと Win32Window のポインタを関連付ける
	BOOL r = ::SetProp(mWindowHandle, Win32WindowManager::PROP_WINPROC, this);
	if (LN_ENSURE_WIN32((r != FALSE), GetLastError())) return;
}

//------------------------------------------------------------------------------
void Win32NativeWindow::setVisible(bool visible)
{
	::ShowWindow(mWindowHandle, (visible) ? SW_SHOW : SW_HIDE);
}

//------------------------------------------------------------------------------
void Win32NativeWindow::setTitleText(const StringRef& title)
{
	::SetWindowText(mWindowHandle, String(title).c_str());
}

//------------------------------------------------------------------------------
void Win32NativeWindow::setFullScreenEnabled(bool flag)
{
	// フルスクリーンにする場合
	if (flag)
	{
		mFullScreen = true;
		::SetWindowLong(mWindowHandle, GWL_STYLE, Win32WindowManager::FULLSCREEN_STYLE);
		::SetWindowLong(mWindowHandle, GWL_EXSTYLE, 0);
		//Resize(true);

		// 画面いっぱい
		int width = ::GetSystemMetrics(SM_CXSCREEN);
		int height = ::GetSystemMetrics(SM_CYSCREEN);

		::SetForegroundWindow(mWindowHandle);

		int x = (::GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		int y = (::GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		//res = ::SetWindowPos( mWindowHandle, HWND_TOP, 0, 0, mWidth, mHeight, SWP_SHOWWINDOW );
		::SetWindowPos(mWindowHandle, NULL, x, y, width, height, SWP_SHOWWINDOW);
	}
	// ウィンドウモードにする場合
	else
	{
		mFullScreen = false;
		::SetWindowLong(mWindowHandle, GWL_STYLE, mWindowedStyle);
		Win32WindowManager::setWindowClientSize(mWindowHandle, m_originalClientSize);
		Win32WindowManager::abjustLocationCentering(mWindowHandle);
		//mClientSize = m_originalClientSize;
	}
}

//==============================================================================
// Win32UserHostWindow
//==============================================================================
//------------------------------------------------------------------------------
Win32UserHostWindow::Win32UserHostWindow(Win32WindowManager* windowManager, HWND hWnd)
	: Win32PlatformWindow(windowManager)
	, m_hWnd(hWnd)
	, m_clientSize()
{
	RECT rc = { 0, 0, 0, 0 };
	::GetClientRect(m_hWnd, &rc);
	m_clientSize.set(rc.right, rc.bottom);
	sendPlatformEvent(PlatformEventArgs::makeWindowSizeChangedEvent(this, rc.right, rc.bottom));

	Char text[256];
	::GetWindowText(m_hWnd, text, 256);
	m_titleText = text;
}

//------------------------------------------------------------------------------
Win32UserHostWindow::~Win32UserHostWindow()
{
}

//------------------------------------------------------------------------------
void Win32UserHostWindow::setTitleText(const StringRef& title)
{
	::SetWindowText(m_hWnd, String(title).c_str());
}

LN_NAMESPACE_END
