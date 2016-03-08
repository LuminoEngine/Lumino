
#include "../../Internal.h"
#include <Lumino/Platform/PlatformSupport.h>
#include <Lumino/Platform/Win32/Win32PlatformWindow.h>

LN_NAMESPACE_BEGIN

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
HWND PlatformSupport::GetWindowHandle(PlatformWindow* window)
{
	Win32PlatformWindow* w = dynamic_cast<Win32PlatformWindow*>(window);
	LN_THROW(w, ArgumentException);
	return w->GetWindowHandle();
}

LN_NAMESPACE_END
