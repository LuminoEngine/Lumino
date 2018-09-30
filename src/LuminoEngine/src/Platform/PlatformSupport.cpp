
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include "GLFWPlatformWindowManager.hpp"

namespace ln {

void* PlatformSupport::getWin32WindowHandle(detail::PlatformWindow* window)
{
	if (auto* w = dynamic_cast<detail::GLFWPlatformWindow*>(window))
	{
		return w->getWin32Window();
	}

	return nullptr;
}

} // namespace ln
