
#pragma once

#include "../../Internal.h"
#include "../../../include/Lumino/Platform/PlatformSupport.h"
#include "Win32WindowBase.h"

namespace Lumino
{
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
} // namespace Lumino
