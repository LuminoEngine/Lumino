
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include "GLFWPlatformWindowManager.hpp"

namespace ln {

void* PlatformSupport::getWin32WindowHandle(detail::PlatformWindow* window)
{
#ifdef LN_GLFW
	if (auto* w = dynamic_cast<detail::GLFWPlatformWindow*>(window))
	{
		return w->getWin32Window();
	}
#endif

	return nullptr;
}

} // namespace ln
