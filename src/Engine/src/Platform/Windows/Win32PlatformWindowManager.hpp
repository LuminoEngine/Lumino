#pragma once

#ifdef _WIN32

#include "../PlatformWindowManager.hpp"

namespace ln {
namespace detail {
class Win32PlatformWindowManager;

class AbstractWin32PlatformWindow
    : public PlatformWindow
{
public:
    AbstractWin32PlatformWindow();
    virtual void dispose() {}

    virtual void setWindowTitle(const String& title) override;
    virtual void getSize(SizeI* size) override;
    virtual void setSize(const SizeI& size) override { LN_NOTIMPLEMENTED(); }
    virtual void getFramebufferSize(int* width, int* height) override;
    virtual void setAllowDragDrop(bool value) override;
    virtual bool isAllowDragDrop() const override;
    virtual PointI pointFromScreen(const PointI& screenPoint) override;
    virtual PointI pointToScreen(const PointI& clientPoint) override;
    virtual void grabCursor() override;
    virtual void releaseCursor() override;

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

class Win32PlatformWindow
	: public AbstractWin32PlatformWindow
{
public:
    Win32PlatformWindow();

	Result init(Win32PlatformWindowManager* windowManager, const WindowCreationSettings& settings);
	void dispose() override;

private:
    HACCEL m_accelerator;  // for erase Alt+Enter alart
};

class WrappedWin32PlatformWindow
    : public AbstractWin32PlatformWindow
{
public:
    WrappedWin32PlatformWindow();
    Result init(Win32PlatformWindowManager* windowManager, intptr_t	windowHandle);
    void dispose() override;

private:
    static LRESULT CALLBACK StaticWndProcHook(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    
    WNDPROC m_originalWndProc;
};

class Win32PlatformWindowManager
	: public PlatformWindowManager
{
public:
    static const wchar_t* WindowClassName;
    static const wchar_t* PropWinProc;
    static const DWORD FullscreenStyle;

    Win32PlatformWindowManager(PlatformManager* manager);

    Result init();
	virtual void dispose() override;
	virtual Ref<PlatformWindow> createMainWindow(const WindowCreationSettings& settings) override;
    virtual Ref<PlatformWindow> createSubWindow(const WindowCreationSettings& settings) override;
	virtual void destroyWindow(PlatformWindow* window) override;
	virtual void processSystemEventQueue(EventProcessingMode mode) override;
    virtual OpenGLContext* getOpenGLContext() const override;

    HINSTANCE instanceHandle() const { return m_hInst; }

private:
    HINSTANCE m_hInst;
    HICON m_hIcon;
};

} // namespace detail
} // namespace ln

#endif
