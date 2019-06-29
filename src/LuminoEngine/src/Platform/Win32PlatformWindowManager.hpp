#pragma once

#ifdef _WIN32

#include "PlatformWindowManager.hpp"

namespace ln {
namespace detail {
class Win32PlatformWindowManager;

class AbstractWin32PlatformWindow
    : public PlatformWindow
{
public:
    AbstractWin32PlatformWindow();
    virtual void dispose() {}

    //void init(const WindowCreationSettings& settings);
    //virtual void dispose() override;
    virtual void setWindowTitle(const String& title) override;
    virtual void getSize(SizeI* size) override;
    virtual void getFramebufferSize(int* width, int* height) override;
    virtual PointI pointFromScreen(const PointI& screenPoint) override;
    virtual PointI pointToScreen(const PointI& clientPoint) override;

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

private:
};

class Win32PlatformWindowManager
	: public PlatformWindowManager
{
public:
    static const wchar_t* WindowClassName;
    static const wchar_t* PropWinProc;
    static const DWORD FullscreenStyle;

    Win32PlatformWindowManager();

    Result init();
	virtual void dispose() override;
	virtual Ref<PlatformWindow> createWindow(const WindowCreationSettings& settings) override;
	virtual void destroyWindow(PlatformWindow* window) override;
	virtual void processSystemEventQueue() override;

    HINSTANCE instanceHandle() const { return m_hInst; }

private:
    HINSTANCE m_hInst;
    HICON m_hIcon;
};

} // namespace detail
} // namespace ln

#endif
