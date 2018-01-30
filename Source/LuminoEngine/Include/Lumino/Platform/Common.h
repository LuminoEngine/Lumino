
#pragma once
#include "../Common.h"
#include <Lumino/Base/Platform.h>

LN_NAMESPACE_BEGIN
class PlatformManager;
class WindowManagerBase;
class Win32WindowManager;
class PlatformWindow;
class Win32PlatformWindow;

/** ウィンドウシステムの API・ライブラリ */
enum class WindowSystemAPI
{
	Default,
	Win32API,
    Cocoa,
	X11,
	//GLFW,
};

namespace detail
{
	class MouseCursorVisibility;

} // namespace detail

LN_NAMESPACE_END

#include "PlatformEventArgs.h"
