
#pragma once
#include <Lumino/Base/Common.h>

LN_NAMESPACE_BEGIN
namespace Platform
{
class Window;

/**
	@brief	プラットフォーム固有の機能にアクセスします。
*/
class PlatformSupport
{
public:
#ifdef LN_OS_WIN32
	/**
		@brief		ウィンドウのウィンドウハンドルを取得します。
	*/
	static HWND GetWindowHandle(Window* window);
#endif
#ifdef LN_X11
	/**
		@brief		ウィンドウのウィンドウIDを取得します。
	*/
	static ::Window GetX11WindowID(Window* window);

	/**
		@brief		ウィンドウの GLXFBConfig 構造体のポインタを取得します。
	*/
	static const GLXFBConfig* GetGLXFBConfig(Window* window);
	
	/**
		@brief		ウィンドウのビジュアルを取得します。
	*/
	static XVisualInfo* GetX11VisualInfo(Window* window);

	/**
		@brief		ウィンドウが接続されているXサーバを取得します。
	*/
	static Display* GetX11Display(Window* window);
#endif
};

} // namespace Platform
LN_NAMESPACE_END
