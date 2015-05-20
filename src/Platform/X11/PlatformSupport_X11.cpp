
#include <GL/glx.h>
#include "../../Internal.h"
#include "../../../include/Lumino/Platform/PlatformSupport.h"
#include "X11WindowManager.h"
#include "X11Window.h"

namespace Lumino
{
namespace Platform
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
::Window PlatformSupport::GetX11WindowID(Window* window)
{
	X11Window* w = dynamic_cast<X11Window*>(window);
	LN_THROW(w, ArgumentException);
	return w->GetX11WindowID();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const GLXFBConfig* PlatformSupport::GetGLXFBConfig(Window* window)
{
	X11Window* w = dynamic_cast<X11Window*>(window);
	LN_THROW(w, ArgumentException);
	return w->GetGLXFBConfig();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
XVisualInfo* PlatformSupport::GetX11VisualInfo(Window* window)
{
	X11Window* w = dynamic_cast<X11Window*>(window);
	LN_THROW(w, ArgumentException);
	return w->GetX11VisualInfo();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Display* PlatformSupport::GetX11Display(Window* window)
{
	X11Window* w = dynamic_cast<X11Window*>(window);
	LN_THROW(w, ArgumentException);
	return w->GetWindowManager()->GetX11Display();
}

} // namespace Platform
} // namespace Lumino
