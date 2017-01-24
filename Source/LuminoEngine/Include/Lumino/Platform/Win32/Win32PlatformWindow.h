
#pragma once
#include <Lumino/Base/String.h>
#include "../../Base/GeometryStructs.h"
#include "../Common.h"
#include "../PlatformWindow.h"

LN_NAMESPACE_BEGIN
	
/**
	@brief	
*/
class Win32PlatformWindow
	: public PlatformWindow
{
public:

	/** ウィンドウメッセージが処理される前に実行されるイベント */
	Delegate<LRESULT(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled)>	UserWndProc;
	//tr::DelegateEvent<LRESULT(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled)>	PreWndProc;

	// override PlatformWindow
	//virtual bool IsActive() const { return mIsActive; }
	//virtual void HideCursor();
	//virtual void ShowCursor();

public:
	/// メッセージ処理のベース
	///		この処理はメインウィンドウの他、ユーザーウィンドウのホストクラスでも使用する。
	///		ホストの場合、ライブラリのユーザーが直接この関数を呼び出し、windows メッセージをライブラリに知らせる必要がある。
	///		ホストの場合、メッセージループおよび DefWndProc はユーザー側で呼ぶことになるのでこの中では行わない。代わりに handled でメッセージを処理したことを伝える。
	LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled);

	//bool NortifyEvent(const PlatformEventArgs& e);

	/// ウィンドウハンドルの取得
	virtual HWND GetWindowHandle() = 0;

	virtual void SetSize(const SizeI& size) override;
	virtual void OnPlatformEvent(const PlatformEventArgs& e) override;

public:
	static Keys ConvertVirtualKeyCode(DWORD winVK);

protected:
	int		mLastMouseX;
	int		mLastMouseY;
	bool	m_systemMouseShown;

LN_INTERNAL_ACCESS:
	Win32PlatformWindow(Win32WindowManager* app);
	virtual ~Win32PlatformWindow();
};

/**
	@brief	
*/
class Win32NativeWindow
	: public Win32PlatformWindow
{
public:
	Win32NativeWindow(Win32WindowManager* windowManager);
	virtual ~Win32NativeWindow();
	void Initilaize(
		Win32WindowManager* windowManager,
		String windowTitle,
		int width,
		int height,
		bool fullscreen,
		bool resizable);


public:
	// PlatformWindow interface
	virtual void SetVisible(bool visible) override;
	virtual void SetTitleText(const StringRef& title) override;
	virtual const String& GetTitleText() const override { return mTitleText; }
	virtual void SetFullScreenEnabled(bool enabled) override;
	virtual bool IsFullScreenEnabled() const override { return mFullScreen; }

	// Win32Window interface
	virtual HWND GetWindowHandle() override { return mWindowHandle; }

private:
	String		mTitleText;				// ウィンドウタイトルの文字列  
	SizeI		m_originalClientSize;	// initialize() または setSize() で設定されたクライアント領域のサイズ
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
	: public Win32PlatformWindow
{
public:
	Win32UserHostWindow(Win32WindowManager* windowManager, HWND hWnd);
	virtual ~Win32UserHostWindow();

public:
	// PlatformWindow interface
	virtual void SetVisible(bool visible) override {}
	virtual void SetTitleText(const StringRef& title) override;
	virtual const String& GetTitleText() const override { return m_titleText; }
	virtual void SetFullScreenEnabled(bool enabled) override {}
	virtual bool IsFullScreenEnabled() const override { return false; }

	// Win32Window interface
	virtual HWND GetWindowHandle() override { return m_hWnd; }

private:
	HWND			m_hWnd;
	String			m_titleText;
	mutable SizeI	m_clientSize;
};

LN_NAMESPACE_END
