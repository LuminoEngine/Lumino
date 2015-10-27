
#pragma once
#include <Lumino/Base/String.h>
#include <Lumino/Base/Size.h>
#include "Win32WindowBase.h"

LN_NAMESPACE_BEGIN
namespace Platform
{

class Win32Window
	: public Win32WindowBase
{
public:
	Win32Window(Win32WindowManager* windowManager, HWND hWnd, DWORD hWindowedStyle, HACCEL hAccel, const String& title);
	virtual ~Win32Window();

public:
	// override Window
	virtual const Size& GetSize() const { return mClientSize; }
	virtual void SetVisible(bool visible);
	virtual void SetFullScreenEnabled(bool enabled);
	virtual bool IsFullScreenEnabled() const { return mFullScreen; }
	virtual void CaptureMouse();
	virtual void ReleaseMouseCapture();

	// override Win32WindowBase
	virtual HWND GetWindowHandle() { return mWindowHandle; }

private:
	String		mTitleText;			///< ウィンドウタイトルの文字列  
	Size		m_originalClientSize;	///< initialize() または setSize() で設定されたクライアント領域のサイズ
	HWND		mWindowHandle;		///< ウィンドウハンドル
	RECT		mWindowRect;		///< ウィンドウ領域
	HACCEL		mAccelerators;      ///< アクセラレータ (Alt+Enter の警告音対策のために使ってる)
	DWORD		mWindowedStyle;		///< ウィンドウモード時のスタイル
	bool		mFullScreen;        ///< フルスクリーンモード ( 用のスタイル ) の場合 true

};



class Win32UserHostWindow
	: public Win32WindowBase
{
public:
	Win32UserHostWindow(Win32WindowManager* windowManager, HWND hWnd);
	virtual ~Win32UserHostWindow();

public:
	// override Window
	virtual const Size& GetSize() const;
	virtual void SetVisible(bool visible) {}
	virtual void SetFullScreenEnabled(bool enabled) {}
	virtual bool IsFullScreenEnabled() const { return false; }
	virtual void CaptureMouse();
	virtual void ReleaseMouseCapture();

	// override Win32WindowBase
	virtual HWND GetWindowHandle() { return m_hWnd; }

private:
	HWND			m_hWnd;
	mutable Size	m_clientSize;
};

} // namespace Platform
LN_NAMESPACE_END
