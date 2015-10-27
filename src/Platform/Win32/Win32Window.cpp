
#include "../../Internal.h"
#include "Win32WindowManager.h"
#include "Win32Window.h"

LN_NAMESPACE_BEGIN
namespace Platform
{

//=============================================================================
// Win32Window
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32Window::Win32Window(Win32WindowManager* windowManager, HWND hWnd, DWORD hWindowedStyle, HACCEL hAccel, const String& title)
	: Win32WindowBase(windowManager)
	, mTitleText(title)
	, mWindowHandle(hWnd)
	, mAccelerators(hAccel)
	, mWindowedStyle(hWindowedStyle)
	, mFullScreen(false)
{
	// 初期化時のサイズ記憶
	RECT rc;
	GetClientRect(mWindowHandle, &rc);
	mClientSize.Width = rc.right - rc.left;
	mClientSize.Height = rc.bottom - rc.top;
	m_originalClientSize = mClientSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Win32Window::~Win32Window()
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
void Win32Window::SetVisible(bool visible)
{
	::ShowWindow(mWindowHandle, (visible) ? SW_SHOW : SW_HIDE);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32Window::SetFullScreenEnabled(bool flag)
{
	// フルスクリーンにする場合
	if (flag)
	{
		mFullScreen = true;
		::SetWindowLong(mWindowHandle, GWL_STYLE, Win32WindowManager::FULLSCREEN_STYLE);
		::SetWindowLong(mWindowHandle, GWL_EXSTYLE, 0);
		//Resize(true);

		// 画面いっぱい
		mClientSize.Width = ::GetSystemMetrics(SM_CXSCREEN);
		mClientSize.Height = ::GetSystemMetrics(SM_CYSCREEN);

		::SetForegroundWindow(mWindowHandle);

		int x = (::GetSystemMetrics(SM_CXSCREEN) - mClientSize.Width) / 2;
		int y = (::GetSystemMetrics(SM_CYSCREEN) - mClientSize.Height) / 2;

		//res = ::SetWindowPos( mWindowHandle, HWND_TOP, 0, 0, mWidth, mHeight, SWP_SHOWWINDOW );
		::SetWindowPos(mWindowHandle, NULL, x, y, mClientSize.Width, mClientSize.Height, SWP_SHOWWINDOW);
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
void Win32Window::CaptureMouse()
{
	::SetCapture(mWindowHandle);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Win32Window::ReleaseMouseCapture()
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
	: Win32WindowBase(windowManager)
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

} // namespace Platform
LN_NAMESPACE_END
