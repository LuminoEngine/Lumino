
#include "Internal.hpp"
#include <Windows.h>
//#include <LuminoEngine/Engine/Application.hpp>
#include "../../LuminoEngine/include/LuminoEngine/Engine/Application.hpp"
#include <LuminoPlatform/Win32PlatformInterface.hpp>
#include "../GLFWPlatformWindowManager.hpp"

#if defined(_WIN32) && !defined(LUMINO_BUILD_DLL)
extern "C" ::ln::Application* LuminoCreateApplicationInstance();
#endif

namespace ln {

static ln::Application* g_app = nullptr;

//void Win32PlatformInterface::init(Application* app)
//{
//
//    if (app) {
//        g_app = app;
//    }
//    else {
//#if defined(_WIN32) && !defined(LUMINO_BUILD_DLL)
//        g_app = ::LuminoCreateApplicationInstance();
//#endif
//    }
//    detail::ApplicationHelper::init(g_app);
//}
//
int Win32PlatformInterface::WinMain(Application* app)
{
    g_app = app;

	detail::ApplicationHelper::run(g_app);
	detail::ApplicationHelper::finalize(g_app);
	RefObjectHelper::release(g_app);
	g_app = nullptr;

	return 0;
}

//ln::Path Win32PlatformInterface::findParentDirectoryContainingSpecifiedFile(StringRef file)
//{
//	return detail::EngineManager::findParentDirectoryContainingSpecifiedFile(file);
//}

} // namespace ln

