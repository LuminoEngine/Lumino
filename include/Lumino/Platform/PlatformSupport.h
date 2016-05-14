
#pragma once
#include <Lumino/Base/Common.h>
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	プラットフォーム固有の機能にアクセスします。
*/
class PlatformSupport
{
public:
	static void ShowAlertMessageBox(const TCHAR* message) LN_NOEXCEPT;

#ifdef LN_OS_WIN32
	/**
		@brief		ウィンドウのウィンドウハンドルを取得します。
	*/
	static HWND GetWindowHandle(PlatformWindow* window);
#endif
#ifdef LN_X11
	/**
		@brief		ウィンドウのウィンドウIDを取得します。
	*/
	static ::PlatformWindow GetX11WindowID(Window* window);

	/**
		@brief		ウィンドウの GLXFBConfig 構造体のポインタを取得します。
	*/
	static const GLXFBConfig* GetGLXFBConfig(PlatformWindow* window);
	
	/**
		@brief		ウィンドウのビジュアルを取得します。
	*/
	static XVisualInfo* GetX11VisualInfo(PlatformWindow* window);

	/**
		@brief		ウィンドウが接続されているXサーバを取得します。
	*/
	static Display* GetX11Display(PlatformWindow* window);
#endif
};

LN_NAMESPACE_END
