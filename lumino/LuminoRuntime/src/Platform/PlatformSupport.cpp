
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include "GLFWPlatformWindowManager.hpp"
#include "Windows/Win32PlatformWindowManager.hpp"

namespace ln {


void* PlatformSupport::getWin32WindowHandle(PlatformWindow* window)
{
#ifdef LN_OS_WIN32
    if (auto* w = dynamic_cast<detail::AbstractWin32PlatformWindow*>(window)) {
        return w->windowHandle();
    }
#endif

#ifdef LN_GLFW
    if (auto* w = dynamic_cast<detail::GLFWPlatformWindow*>(window)) {
        return w->getWin32Window();
    }
#endif

    return nullptr;
}

} // namespace ln
