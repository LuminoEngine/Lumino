
#pragma once

//#ifdef LNOTE_WIN32
//	#define GLFW_EXPOSE_NATIVE_WIN32
//	#define GLFW_EXPOSE_NATIVE_WGL
//#endif
//#include <GLFW/glfw3native.h>
#include <GLFW/glfw3.h>
#include <Lumino/Platform/PlatformWindow.h>

LN_NAMESPACE_BEGIN
class GLFWPlatformWindowManager;

class GLFWPlatformWindow
	: public PlatformWindow
{
public:
	GLFWPlatformWindow(GLFWPlatformWindowManager* manager);
	virtual ~GLFWPlatformWindow();
    void Initialize(GLFWPlatformWindowManager* windowManager, String windowTitle, int width, int height, bool fullscreen, bool resizable);
	
	GLFWwindow* getGLFWWindow() { return m_glfwWindow; }
//#ifdef LNOTE_WIN32
//	HWND getWindowHandle() { return glfwGetWin32Window( mGLFWWindow ); }
//#endif

public:
	// PlatformWindow interface
	virtual Size GetSize() const override;
	virtual void SetVisible(bool visible) override;
	virtual void SetFullScreenEnabled(bool enabled) override;
	virtual bool IsFullScreenEnabled() const override;
	virtual void CaptureMouse() override;
	virtual void ReleaseMouseCapture() override;
	
private:
	void _resize(bool fullscreen);
	static void window_close_callback(GLFWwindow* window);
	static void window_focus_callback(GLFWwindow* window, int focused);	// ウィンドウアクティブ化/非アクティブ化
	static void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	GLFWwindow*			m_glfwWindow;
	bool				m_fullScreen;
	
	/*
	GLFWWindowManager*	mWindowManager;
	GLFWwindow*			mGLFWWindow;
	int					mLastMouseX;
	int					mLastMouseY;
    bool                mIsActive;

	lnString    		mTitleText;			///< ウィンドウタイトルの文字列    
    Geometry::Size		mClientSize;        ///< クライアント領域の大きさ
    int					mOrginalWidth;      ///< initialize() または setSize() で設定されたクライアント領域の幅
    int					mOrginalHeight;     ///< initialize() または setSize() で設定されたクライアント領域の高さ
    bool				mFullScreen;        ///< フルスクリーンモード ( 用のスタイル ) の場合 true
	*/
};


LN_NAMESPACE_END
