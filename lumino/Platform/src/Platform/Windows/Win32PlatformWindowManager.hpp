#pragma once
#ifdef _WIN32
#include <LuminoPlatform/detail/PlatformWindowManager.hpp>

namespace ln {
namespace detail {
class Win32PlatformWindowManager;

class AbstractWin32PlatformWindow : public PlatformWindow {
public:
    AbstractWin32PlatformWindow();
    virtual void dispose() {}

    void setWindowTitle(const String& title) override;
    void getSize(SizeI* size) override;
    void setSize(const SizeI& size) override { LN_NOTIMPLEMENTED(); }
    void getFramebufferSize(int* width, int* height) override;
    void setAllowDragDrop(bool value) override;
    bool isAllowDragDrop() const override;
    PointI pointFromScreen(const PointI& screenPoint) override;
    PointI pointToScreen(const PointI& clientPoint) override;
    void grabCursor() override;
    void releaseCursor() override;

    HWND windowHandle() const { return m_hWnd; }

    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled);
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static Keys Win32KeyToLNKey(DWORD winVK);
    static void setWindowClientSize(HWND hWnd, const SizeI& clientSize);
    static void abjustLocationCentering(HWND hWnd);
    static float getDpiFactor(HWND hWnd);

protected:
    HWND m_hWnd;
};

class Win32PlatformWindow : public AbstractWin32PlatformWindow {
public:
    Win32PlatformWindow();

    Result<> init(Win32PlatformWindowManager* windowManager, const WindowCreationSettings& settings);
    void dispose() override;

private:
    HACCEL m_accelerator; // for erase Alt+Enter alart
};

class WrappedWin32PlatformWindow : public AbstractWin32PlatformWindow {
public:
    WrappedWin32PlatformWindow();
    Result<> init(Win32PlatformWindowManager* windowManager, intptr_t windowHandle);
    void dispose() override;

private:
    static LRESULT CALLBACK StaticWndProcHook(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    WNDPROC m_originalWndProc;
};

class Win32PlatformWindowManager : public PlatformWindowManager {
public:
    static const wchar_t* WindowClassName;
    static const wchar_t* PropWinProc;
    static const DWORD FullscreenStyle;

    Win32PlatformWindowManager(PlatformManager* manager);

    Result<> init();
    void dispose() override;
    Ref<PlatformWindow> createWindow(const WindowCreationSettings& settings, PlatformWindow* mainWindow) override;
    void destroyWindow(PlatformWindow* window) override;
    void processSystemEventQueue(EventProcessingMode mode) override;
    OpenGLContext* getOpenGLContext() const override;

    HINSTANCE instanceHandle() const { return m_hInst; }

private:
    HINSTANCE m_hInst;
    HICON m_hIcon;
};

} // namespace detail
} // namespace ln

#endif
