
#ifdef _WIN32

#include "Internal.hpp"
#include <Windowsx.h>
#include <shellscalingapi.h>
#include <LuminoPlatform//PlatformSupport.hpp>
#include "Win32PlatformWindowManager.hpp"
#pragma comment(lib, "Shcore.lib")

namespace ln {
namespace detail {


//==============================================================================
// AbstractWin32PlatformWindow

AbstractWin32PlatformWindow::AbstractWin32PlatformWindow()
    : m_hWnd(NULL)
{
}

void AbstractWin32PlatformWindow::setWindowTitle(const String& title)
{
    ::SetWindowTextW(m_hWnd, title.toStdWString().c_str());
}

void AbstractWin32PlatformWindow::getSize(SizeI* size)
{
    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(m_hWnd, &rect);
    size->width = rect.right - rect.left;
    size->height = rect.bottom - rect.top;
}

void AbstractWin32PlatformWindow::getFramebufferSize(int* width, int* height)
{
    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(m_hWnd, &rect);
    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}

void AbstractWin32PlatformWindow::setAllowDragDrop(bool value)
{
    LN_NOTIMPLEMENTED();
}

bool AbstractWin32PlatformWindow::isAllowDragDrop() const
{
    return false;
}

PointI AbstractWin32PlatformWindow::pointFromScreen(const PointI& screenPoint)
{
    POINT pt = { screenPoint.x, screenPoint.y };
    ::ScreenToClient(m_hWnd, &pt);
    return PointI(pt.x, pt.y);
}

PointI AbstractWin32PlatformWindow::pointToScreen(const PointI& clientPoint)
{
    POINT pt = { clientPoint.x, clientPoint.y };
    ::ClientToScreen(m_hWnd, &pt);
    return PointI(pt.x, pt.y);
}

void AbstractWin32PlatformWindow::grabCursor()
{
    LN_NOTIMPLEMENTED();
}

void AbstractWin32PlatformWindow::releaseCursor()
{
    LN_NOTIMPLEMENTED();
}

LRESULT AbstractWin32PlatformWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled)
{
    *handled = false;

    //if (!UserWndProc.isEmpty())
    //{
    //    LRESULT dr = UserWndProc.call(hwnd, msg, wparam, lparam, handled);//raiseDelegateEvent(PreWndProc, hwnd, msg, wparam, lparam, handled);
    //    if (*handled) {
    //        return dr;
    //    }
    //}

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
        case WM_DESTROY:
            return 0;

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
            sendEventToAllListener(PlatformEventArgs::makeClosingEvent(this));
            *handled = true;
            return 0;
        }

        /////////////////// ウィンドウがアクティブ・非アクティブになった場合
        case WM_ACTIVATE:
        {
            sendEventToAllListener(PlatformEventArgs::makeActivateChangedEvent(this, ((wparam & 0xffff) != 0)));
            *handled = true;
            return 0;
        }
        /////////////////////////////////////////////// ウィンドウサイズが変更された
        case WM_SIZE:
        {
            sendEventToAllListener(PlatformEventArgs::makeWindowSizeChangedEvent(this, lparam & 0xFFFF, (lparam >> 16) & 0xFFFF));
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

            // TOOD: ModifyKeys
            sendEventToAllListener(e);

            *handled = true;
            return 0;
        }
        /////////////////////////////////////////////// マウス移動
        case WM_MOUSEMOVE:
        {
            POINT pt;
            ::GetCursorPos(&pt);

            PlatformEventArgs e = PlatformEventArgs::makeMouseMoveEvent(
                this,
                PlatformEventType::MouseMove,
                pt.x, pt.y,
                0, 0);  // TODO:
            sendEventToAllListener(e);

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
                e.mouseMove.screenX = (short)pt.x;
                e.mouseMove.screenY = (short)pt.y;
                sendEventToAllListener(e);

                *handled = true;
                return 0;
            }
        }
        ///////////////////////////////////////////// マウスホイールが操作された
        case WM_MOUSEWHEEL:
        {
            sendEventToAllListener(PlatformEventArgs::makeMouseWheelEvent(this, GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA));
            *handled = true;
            return 0;
        }
        ///////////////////////////////////////////// キー↓
        case WM_KEYDOWN:
        {
            ModifierKeys mods =
                ((::GetKeyState(VK_MENU) < 0) ? ModifierKeys::Alt : ModifierKeys::None) |
                ((::GetKeyState(VK_SHIFT) < 0) ? ModifierKeys::Shift : ModifierKeys::None) |
                ((::GetKeyState(VK_CONTROL) < 0) ? ModifierKeys::Control : ModifierKeys::None);
            sendEventToAllListener(PlatformEventArgs::makeKeyEvent(this, PlatformEventType::KeyDown, Win32KeyToLNKey(wparam), mods, 0));
            *handled = true;
            return 0;
        }
        ///////////////////////////////////////////// キー↑
        case WM_KEYUP:
        {
            ModifierKeys mods =
                ((::GetKeyState(VK_MENU) < 0) ? ModifierKeys::Alt : ModifierKeys::None) |
                ((::GetKeyState(VK_SHIFT) < 0) ? ModifierKeys::Shift : ModifierKeys::None) |
                ((::GetKeyState(VK_CONTROL) < 0) ? ModifierKeys::Control : ModifierKeys::None);
            sendEventToAllListener(PlatformEventArgs::makeKeyEvent(this, PlatformEventType::KeyUp, Win32KeyToLNKey(wparam), mods, 0));
            *handled = true;
            return 0;
        }
        ///////////////////////////////////////////// Alt + KeyDown
        case WM_SYSKEYDOWN:
        {
            ModifierKeys mods =
                ModifierKeys::Alt |
                ((::GetKeyState(VK_SHIFT) < 0) ? ModifierKeys::Shift : ModifierKeys::None) |
                ((::GetKeyState(VK_CONTROL) < 0) ? ModifierKeys::Control : ModifierKeys::None);
            sendEventToAllListener(PlatformEventArgs::makeKeyEvent(this, PlatformEventType::KeyDown, Win32KeyToLNKey(wparam), mods, 0));
            *handled = true;
            return 0;
        }
        ///////////////////////////////////////////// Alt + KeyUp
        case WM_SYSKEYUP:
        {
            ModifierKeys mods =
                ModifierKeys::Alt |
                ((::GetKeyState(VK_SHIFT) < 0) ? ModifierKeys::Shift : ModifierKeys::None) |
                ((::GetKeyState(VK_CONTROL) < 0) ? ModifierKeys::Control : ModifierKeys::None);
            sendEventToAllListener(PlatformEventArgs::makeKeyEvent(this, PlatformEventType::KeyUp, Win32KeyToLNKey(wparam), mods, 0));
            break;	// WM_SYSKEYUPを捕まえた場合、必ずDefWindowProcに行くようにする
        }
        ///////////////////////////////////////////// 文字入力
        case WM_CHAR:
        {
            // 文字のみ送る
            //if (0x20 <= wparam && wparam <= 0x7E)
            if (wparam > 0 && wparam < 0x10000)
            {
                ModifierKeys mods =
                    ((::GetKeyState(VK_MENU) < 0) ? ModifierKeys::Alt : ModifierKeys::None) |
                    ((::GetKeyState(VK_SHIFT) < 0) ? ModifierKeys::Shift : ModifierKeys::None) |
                    ((::GetKeyState(VK_CONTROL) < 0) ? ModifierKeys::Control : ModifierKeys::None);
                sendEventToAllListener(PlatformEventArgs::makeKeyEvent(this, PlatformEventType::KeyChar, Keys::Unknown, mods, wparam));
                *handled = true;
                return 0;
            }
        }
        }
    }

    return 0;
}

LRESULT CALLBACK AbstractWin32PlatformWindow::StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    AbstractWin32PlatformWindow* window = reinterpret_cast<AbstractWin32PlatformWindow*>(::GetProp(hwnd, Win32PlatformWindowManager::PropWinProc));

    if (window) {
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

Keys AbstractWin32PlatformWindow::Win32KeyToLNKey(DWORD winVK)
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
    }
    return Keys::Unknown;
}

void AbstractWin32PlatformWindow::setWindowClientSize(HWND hWnd, const SizeI& clientSize)
{
    RECT clientRect, windowRect;
    ::GetClientRect(hWnd, &clientRect);
    ::GetWindowRect(hWnd, &windowRect);

    clientRect.right = clientSize.width;
    clientRect.bottom = clientSize.height;

    ::AdjustWindowRect(
        &clientRect,
        static_cast<DWORD>(GetWindowLongPtr(hWnd, GWL_STYLE)),
        FALSE
    );

    ::SetWindowPos(
        hWnd,
        0,
        windowRect.left + clientRect.left,
        windowRect.top + clientRect.top,
        clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top,
        0
    );
}

void AbstractWin32PlatformWindow::abjustLocationCentering(HWND hWnd)
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

float AbstractWin32PlatformWindow::getDpiFactor(HWND hWnd)
{
	// システム設定の [テキスト、アプリ、その他の項目のサイズを変更する] で変更できるユーザースケーリングを気にする。
	// 物理サイズは気にしない。（WPF と同じ動作）

	HMONITOR hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monInfo;
	monInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hmon, &monInfo);

	UINT effX = 0, effY = 0;
	GetDpiForMonitor(hmon, MDT_EFFECTIVE_DPI, &effX, &effY);
	// 100%: raw=165, eff=96
	// 150%: raw=165, eff=144

	return static_cast<float>(effX) / 96.0f;
}


//==============================================================================
// Win32PlatformWindow

Win32PlatformWindow::Win32PlatformWindow()
    : m_accelerator(NULL)
{
}

Result Win32PlatformWindow::init(Win32PlatformWindowManager* windowManager, const WindowCreationSettings& settings)
{
    // ウィンドウモードのときのウィンドウスタイルの選択
    DWORD mWindowedStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    if (settings.resizable) {
        mWindowedStyle |= (WS_THICKFRAME | WS_MAXIMIZEBOX);
    }
    DWORD dwExStyle = 0;
    if (settings.win32IconResourceId == 0) {
        dwExStyle |= WS_EX_DLGMODALFRAME;	// アイコンの無いスタイル
    }

    RECT clientRect;
    clientRect.left = 0;
    clientRect.top = 0;
    clientRect.right = settings.clientSize.width;
    clientRect.bottom = settings.clientSize.height;
    ::AdjustWindowRect(&clientRect, mWindowedStyle, FALSE);

    // ウィンドウの作成
    m_hWnd = ::CreateWindowEx(
        dwExStyle,
        Win32PlatformWindowManager::WindowClassName,
        String(settings.title).toStdWString().c_str(),
        mWindowedStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
        NULL, NULL, windowManager->instanceHandle(), NULL);
    if (LN_ENSURE(m_hWnd, "ErrorCode: %d", GetLastError())) return err();

    // アクセラレータの作成 (Alt+Enter の警告音を消す)
    ACCEL accels[1] =
    {
        { FALT | FVIRTKEY, VK_RETURN, 0 }
    };
    m_accelerator = ::CreateAcceleratorTable(accels, 1);
    if (LN_ENSURE(m_accelerator, "ErrorCode: %d", GetLastError())) return err();

    //AbstractWin32PlatformWindow::setWindowClientSize(m_hWnd, settings.clientSize);
    AbstractWin32PlatformWindow::abjustLocationCentering(m_hWnd);

    // WM_PAINTが呼ばれないようにする
    ::ValidateRect(m_hWnd, 0);

    // ウィンドウハンドルと Win32Window のポインタを関連付ける
    BOOL r = ::SetProp(m_hWnd, Win32PlatformWindowManager::PropWinProc, this);
    if (LN_ENSURE((r != FALSE), "ErrorCode: %d", GetLastError())) return err();

    ::ShowWindow(m_hWnd, SW_SHOW);

    return ok();
}

void Win32PlatformWindow::dispose()
{
    if (m_accelerator) {
        ::DestroyAcceleratorTable(m_accelerator);
        m_accelerator = NULL;
    }

    if (m_hWnd) {
        ::DestroyWindow(m_hWnd);
        m_hWnd = NULL;
    }
}

//==============================================================================
// WrappedWin32PlatformWindow

WrappedWin32PlatformWindow::WrappedWin32PlatformWindow()
    : m_originalWndProc(nullptr)
{
}

Result WrappedWin32PlatformWindow::init(Win32PlatformWindowManager* windowManager, intptr_t	windowHandle)
{
    m_hWnd = (HWND)windowHandle;

    BOOL r = ::SetProp(m_hWnd, Win32PlatformWindowManager::PropWinProc, this);
    if (LN_ENSURE((r != FALSE), "ErrorCode: %d", GetLastError())) return err();

    m_originalWndProc = (WNDPROC)::GetWindowLong(m_hWnd, GWLP_WNDPROC);
    ::SetWindowLong(m_hWnd, GWLP_WNDPROC, (LONG)StaticWndProcHook);
    LN_LOG_DEBUG("Hook WndProc (original: {})", (intptr_t)m_originalWndProc);

    return ok();
}

void WrappedWin32PlatformWindow::dispose()
{
    if (m_originalWndProc) {
        ::SetWindowLong(m_hWnd, GWLP_WNDPROC, (LONG)m_originalWndProc);
        m_originalWndProc = nullptr;
        LN_LOG_DEBUG("Unhooked WndProc");
    }
}

LRESULT WrappedWin32PlatformWindow::StaticWndProcHook(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    WrappedWin32PlatformWindow* window = reinterpret_cast<WrappedWin32PlatformWindow*>(::GetProp(hwnd, Win32PlatformWindowManager::PropWinProc));

    bool handled = false;
    window->WndProc(hwnd, msg, wparam, lparam, &handled);
    // NOTE: いまのところ handled を確認する必要はなさそうなので様子見

    return CallWindowProc(window->m_originalWndProc, hwnd, msg, wparam, lparam);
}

//==============================================================================
// Win32PlatformWindowManager

const wchar_t*	Win32PlatformWindowManager::WindowClassName = L"LuminoWindow";
const wchar_t*	Win32PlatformWindowManager::PropWinProc = L"LuminoWindowProp";
const DWORD	Win32PlatformWindowManager::FullscreenStyle = WS_POPUP;

Win32PlatformWindowManager::Win32PlatformWindowManager(PlatformManager* manager)
    : PlatformWindowManager(manager)
{
}

Result Win32PlatformWindowManager::init()
{
    LN_LOG_INFO("Win32 native window manager");

    m_hInst = (HINSTANCE)::GetModuleHandle(NULL);

    // TODO: ウィンドウアイコン
    m_hIcon = NULL;
    //if (IconResourceID) {
    //    m_hIcon = ::LoadIcon(m_hInst, MAKEINTRESOURCE(IconResourceID));
    //}

    // ウィンドウクラスの設定
    WNDCLASSEX	wcex = {
        sizeof(WNDCLASSEX),
        NULL,
        AbstractWin32PlatformWindow::StaticWndProc,
        0, 0,
        m_hInst,
        m_hIcon,
        ::LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        WindowClassName,
        NULL
    };

    // ウィンドウクラスの登録
    ATOM wc = ::RegisterClassEx(&wcex);
    if (LN_ENSURE(wc, "ErrorCode: %d", ::GetLastError())) return err();

    return ok();
}

void Win32PlatformWindowManager::dispose()
{
    UnregisterClass(WindowClassName, m_hInst);
}

Ref<PlatformWindow> Win32PlatformWindowManager::createMainWindow(const WindowCreationSettings& settings)
{
    return createSubWindow(settings);
}

Ref<PlatformWindow> Win32PlatformWindowManager::createSubWindow(const WindowCreationSettings& settings)
{
    if (settings.userWindow) {
        auto ptr = makeRef<WrappedWin32PlatformWindow>();
        if (!ptr->init(this, settings.userWindow)) {
            return nullptr;
        }
        return ptr;
    }
    else {
        auto ptr = makeRef<Win32PlatformWindow>();
        if (!ptr->init(this, settings)) {
            return nullptr;
        }
        return ptr;
    }
}

void Win32PlatformWindowManager::destroyWindow(PlatformWindow* window)
{
    if (LN_REQUIRE(window)) return;
    static_cast<AbstractWin32PlatformWindow*>(window)->dispose();
}

void Win32PlatformWindowManager::processSystemEventQueue(EventProcessingMode mode)
{
    if (mode == EventProcessingMode::Wait) {
        LN_NOTIMPLEMENTED();
    }
    else {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (::GetMessage(&msg, NULL, 0, 0))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
    }
}

OpenGLContext* Win32PlatformWindowManager::getOpenGLContext() const
{
    return nullptr;
}

} // namespace detail
} // namespace ln

#endif
