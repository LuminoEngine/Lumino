
#pragma once
#include <Lumino/Base/String.h>
#include <Lumino/Base/Size.h>

LN_NAMESPACE_BEGIN
namespace Platform
{
class Win32WindowManager;
	
/**
	@brief	
*/
class Win32Window
	: public Window
{
public:
	Win32Window(Win32WindowManager* app);
	virtual ~Win32Window();

public:
	// override Window
	virtual bool IsActive() const { return mIsActive; }
	//virtual void HideCursor();
	//virtual void ShowCursor();

public:
	/// メッセージ処理のベース
	///		この処理はメインウィンドウの他、ユーザーウィンドウのホストクラスでも使用する。
	///		ホストの場合、ライブラリのユーザーが直接この関数を呼び出し、windows メッセージをライブラリに知らせる必要がある。
	///		ホストの場合、メッセージループおよび DefWndProc はユーザー側で呼ぶことになるのでこの中では行わない。代わりに handled でメッセージを処理したことを伝える。
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled);

	bool NortifyEvent(const EventArgs& e);

	/// ウィンドウハンドルの取得
	virtual HWND GetWindowHandle() = 0;

public:
	static Key ConvertVirtualKeyCode(DWORD winVK);

protected:
	Size	mClientSize;        ///< クライアント領域の大きさ
	int		mLastMouseX;
	int		mLastMouseY;
	bool	mIsActive;
	bool	m_systemMouseShown;
};

/**
	@brief	
*/
class Win32NativeWindow
	: public Win32Window
{
public:
	Win32NativeWindow(Win32WindowManager* windowManager, HWND hWnd, DWORD hWindowedStyle, HACCEL hAccel, const String& title);
	virtual ~Win32NativeWindow();

public:
	// Window interface
	virtual const Size& GetSize() const { return mClientSize; }
	virtual void SetVisible(bool visible);
	virtual void SetFullScreenEnabled(bool enabled);
	virtual bool IsFullScreenEnabled() const { return mFullScreen; }
	virtual void CaptureMouse();
	virtual void ReleaseMouseCapture();

	// Win32Window interface
	virtual HWND GetWindowHandle() { return mWindowHandle; }

private:
	String		mTitleText;				// ウィンドウタイトルの文字列  
	Size		m_originalClientSize;	// initialize() または setSize() で設定されたクライアント領域のサイズ
	HWND		mWindowHandle;			// ウィンドウハンドル
	RECT		mWindowRect;			// ウィンドウ領域
	HACCEL		mAccelerators;			// アクセラレータ (Alt+Enter の警告音対策のために使ってる)
	DWORD		mWindowedStyle;			// ウィンドウモード時のスタイル
	bool		mFullScreen;			// フルスクリーンモード ( 用のスタイル ) の場合 true
};

/**
	@brief	
*/
class Win32UserHostWindow
	: public Win32Window
{
public:
	Win32UserHostWindow(Win32WindowManager* windowManager, HWND hWnd);
	virtual ~Win32UserHostWindow();

public:
	// Window interface
	virtual const Size& GetSize() const;
	virtual void SetVisible(bool visible) {}
	virtual void SetFullScreenEnabled(bool enabled) {}
	virtual bool IsFullScreenEnabled() const { return false; }
	virtual void CaptureMouse();
	virtual void ReleaseMouseCapture();

	// Win32Window interface
	virtual HWND GetWindowHandle() { return m_hWnd; }

private:
	HWND			m_hWnd;
	mutable Size	m_clientSize;
};

} // namespace Platform
LN_NAMESPACE_END
