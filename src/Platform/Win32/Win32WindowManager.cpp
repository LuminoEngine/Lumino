
#include "../../Internal.h"
#include <Lumino/Platform/Win32/Win32Window.h>
#include "Win32WindowManager.h"

LN_NAMESPACE_BEGIN
namespace Platform
{

//=============================================================================
// Win32WindowManager
//=============================================================================

const TCHAR*	WINDOW_CLASS_NAME = _T("_LNote_");
const TCHAR*	Win32WindowManager::PROP_WINPROC = _T("_LNoteProp_");
const DWORD		Win32WindowManager::FULLSCREEN_STYLE = WS_POPUP;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32WindowManager::Win32WindowManager(int IconResourceID)
	//: m_comInited(false)
{
	// CoInitializeEx は ShowWindow() ～ DestroyWindow() の外側で呼び出さなければならない。
	// http://blog.techlab-xe.net/archives/400
	// 例えば ウィンドウ作成→DirectInput初期化みたいにするとき、Input モジュールの中で CoInitializeEx しているとこの罠にはまる。
	// とりあえず、Platform モジュールでは COM は使わないが、他のモジュールとの連携に備え、初期化しておく。
	//if (SUCCEEDED(::CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	//{
	//	// エラーにはしない。別の設定で COM が初期化済みだったりすると失敗することがあるが、COM 自体は使えるようになっている
	//	m_comInited = true;
	//}

	m_windowClassName = WINDOW_CLASS_NAME;

	//const TCHAR*	        WinClassName;   ///< ウィンドウクラスの名前 または NULL ( NULL の場合、L"_LNote_" が使われる )
	//int                     IconResourceID; ///< タイトルバーのアイコン識別子 (IDI_ICON1 等)

	m_hInst = (HINSTANCE)::GetModuleHandle(NULL);

	// ウィンドウアイコン
	m_hIcon = NULL;
	if (IconResourceID) {
		m_hIcon = ::LoadIcon(m_hInst, MAKEINTRESOURCE(IconResourceID));
	}

	// ウィンドウクラスの設定
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),			    // この構造体のサイズ
		NULL,							    // ウインドウのスタイル
		StaticWndProc,					    // メッセージ処理関数
		0, 0,							    // 通常は使わないので常に0
		m_hInst,				                // インスタンスハンドル
		m_hIcon,							    // アイコン
		::LoadCursor(NULL, IDC_ARROW),	// カーソルの形
		(HBRUSH)(COLOR_WINDOW + 1),	    // 背景色
		NULL,							    // メニューなし
		m_windowClassName.c_str(),				// クラス名の指定
		NULL };							    // 小アイコン（なし）

	// ウィンドウクラスの登録
	if (!::RegisterClassEx(&wcex))
	{
		LN_THROW(0, Win32Exception, GetLastError());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32WindowManager::~Win32WindowManager()
{
	UnregisterClass(m_windowClassName.c_str(), m_hInst);

	//if (m_comInited)
	//{
	//	::CoUninitialize();
	//	m_comInited = false;
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32Window* Win32WindowManager::CreateNativeWindow(const NativeWindowCreationData& data)
{
	if (data.UserWindow == NULL)
	{
		/* もともとは Win32Window クラスで生成していたが、
		* RegisterClassEx() を Manager に移動したことで、ウィンドウスタイルなどの情報も Manager に移った。
		* ウィンドウも Manager で生成して、HWND を Win32Window クラスに渡したほうが
		* シンプルに実装できると見込んだためこちらに移動した。
		*/

		// ウィンドウモードのときのウィンドウスタイルの選択
		DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		if (data.Resizable) {
			dwStyle |= (WS_THICKFRAME | WS_MAXIMIZEBOX);
		}
		DWORD dwExStyle = 0;
		if (m_hIcon == NULL) {
			dwExStyle |= WS_EX_DLGMODALFRAME;	// アイコンの無いスタイル
		}

		// ウィンドウの作成
		HWND hWnd = ::CreateWindowEx(
			dwExStyle,
			m_windowClassName.c_str(),
			data.TitleText.c_str(),
			dwStyle,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, m_hInst, NULL);
		LN_THROW(hWnd, Win32Exception, GetLastError());

		// アクセラレータの作成 (Alt+Enter の警告音を消す)
		ACCEL accels[1] =
		{
			{ FALT | FVIRTKEY, VK_RETURN, 0 }
		};
		HACCEL hAccel = ::CreateAcceleratorTable(accels, 1);
		LN_THROW(hAccel, Win32Exception, GetLastError());

		// ウィンドウサイズをクライアント領域サイズから再設定
		SetWindowClientSize(hWnd, Size(data.Width, data.Height));
		AbjustLocationCentering(hWnd);

		// WM_PAINTが呼ばれないようにする
		::ValidateRect(hWnd, 0);

		// Win32Window 作成
		RefPtr<Win32NativeWindow> window(LN_NEW Win32NativeWindow(this, hWnd, dwStyle, hAccel, data.TitleText), false);

		// ウィンドウハンドルと Win32Window のポインタを関連付ける
		BOOL r = ::SetProp(hWnd, PROP_WINPROC, window);
		LN_THROW((r != FALSE), Win32Exception, GetLastError());

		//window->SetFullScreenEnabled(true);
		window->SetVisible(true);

		window.SafeAddRef();
		return window;
	}
	else
	{
		return LN_NEW Win32UserHostWindow(this, data.UserWindow);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LRESULT CALLBACK Win32WindowManager::StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//static int i = 0;
	//printf( "%d %4x\n", i++, msg_ );
	/*
	0x000C SPI_LANGDRIVER
	0x00AE ?
	0x007F SPI_SETMOUSEDOCKTHRESHOLD
	*/

	Win32NativeWindow* window = (Win32NativeWindow*)::GetProp(hwnd, PROP_WINPROC);

	if (window) {
		//if (msg == WM_SIZE) {
		//	// ウィンドウサイズを拾っておく
		//	RECT rc;
		//	::GetClientRect(window->mWindowHandle, &rc);
		//	window->mClientSize.Set(rc.right, rc.bottom);
		//}
		bool handled = false;
		LRESULT r = window->WndProc(hwnd, msg, wparam, lparam, &handled);
		if (!handled) {
			r = ::DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return r;
	}
	else {
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32WindowManager::SetWindowClientSize(HWND hWnd, const Size& clientSize)
{
	RECT rw, rc;
	::GetWindowRect(hWnd, &rw);
	::GetClientRect(hWnd, &rc);

	int new_width = (rw.right - rw.left) - (rc.right - rc.left) + clientSize.Width;
	int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + clientSize.Height;

	BOOL r = ::SetWindowPos(hWnd, NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
	LN_THROW((r != FALSE), Win32Exception, GetLastError());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32WindowManager::AbjustLocationCentering(HWND hWnd)
{
	RECT rcWindow;
	::GetWindowRect(hWnd, &rcWindow);

	// ディスプレイ全体のサイズを取得
	int sw = ::GetSystemMetrics(SM_CXSCREEN);
	int sh = ::GetSystemMetrics(SM_CYSCREEN);
	int x = (sw - (rcWindow.right - rcWindow.left)) / 2;
	int y = (sh - (rcWindow.bottom - rcWindow.top)) / 2;

	// サイズ変更せず移動だけ行う
	::SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32WindowManager::CreateMainWindow(const WindowCreationSettings& settings)
{
	Win32WindowManager::NativeWindowCreationData data;
	data.TitleText = settings.Title;
	data.Width = settings.ClientSize.Width;
	data.Height = settings.ClientSize.Height;
	data.Fullscreen = settings.Fullscreen;
	data.Resizable = settings.Resizable;
	data.UserWindow = (HWND)settings.UserWindow;
	m_mainWindow.Attach(CreateNativeWindow(data));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Window* Win32WindowManager::GetMainWindow()
{
	return m_mainWindow;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Window* Win32WindowManager::CreateSubWindow(const WindowCreationSettings& settings)
{
	Win32WindowManager::NativeWindowCreationData data;
	data.TitleText = settings.Title;
	data.Width = settings.ClientSize.Width;
	data.Height = settings.ClientSize.Height;
	data.Fullscreen = settings.Fullscreen;
	data.Resizable = settings.Resizable;
	return CreateNativeWindow(data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32WindowManager::DoEvents()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (::GetMessage(&msg, NULL, 0, 0))
		{
			if (msg.message == WM_QUIT){
				m_endRequested = true;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32WindowManager::Finalize()
{
	m_mainWindow.SafeRelease();
}

} // namespace Platform
LN_NAMESPACE_END
