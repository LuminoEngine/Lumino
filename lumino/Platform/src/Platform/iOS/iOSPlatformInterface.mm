
#include <LuminoCore.hpp>
#include "../Internal.hpp"
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoEngine/Platform/iOSPlatformInterface.hpp>
#include "../../Engine/EngineManager.hpp"

static ln::Application* g_app = nullptr;

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

namespace ln {

void iOSPlatformInterface::nativeInitialize(int viewWidth, int viewHeight)
{
	g_app = ::LuminoCreateApplicationInstance();
	ln::detail::ApplicationHelper::init(g_app);
}

void iOSPlatformInterface::nativeUpdateFrame()
{
	ln::detail::ApplicationHelper::processTick(g_app);
}

void iOSPlatformInterface::nativeFinalize()
{
	ln::detail::ApplicationHelper::finalize(g_app);
	ln::RefObjectHelper::release(g_app);
    g_app = nullptr;
}

void iOSPlatformInterface::applicationDidEnterBackground()
{
	
}

void iOSPlatformInterface::applicationWillEnterForeground()
{

}

void iOSPlatformInterface::applicationScreenSizeChanged(int viewWidth, int viewHeight)
{
}

} // namespace ln

