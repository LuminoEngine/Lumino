
#include "../../Internal.h"
#include <Lumino/Platform/PlatformSupport.h>
#include <Lumino/Platform/Win32/Win32Window.h>

LN_NAMESPACE_BEGIN
namespace Platform
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
HWND PlatformSupport::GetWindowHandle(Window* window)
{
	Win32Window* w = dynamic_cast<Win32Window*>(window);
	LN_THROW(w, ArgumentException);
	return w->GetWindowHandle();
}

} // namespace Platform
LN_NAMESPACE_END
