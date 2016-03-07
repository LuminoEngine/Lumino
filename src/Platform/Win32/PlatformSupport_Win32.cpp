
#include "../../Internal.h"
#include "../../../include/Lumino/Platform/PlatformSupport.h"
#include "Win32Window.h"

LN_NAMESPACE_BEGIN
namespace Platform
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
HWND PlatformSupport::GetWindowHandle(Window* window)
{
	Win32WindowBase* w = dynamic_cast<Win32WindowBase*>(window);
	LN_THROW(w, ArgumentException);
	return w->GetWindowHandle();
}

} // namespace Platform
LN_NAMESPACE_END
