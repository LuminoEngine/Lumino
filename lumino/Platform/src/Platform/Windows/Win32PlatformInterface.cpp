
#include "Internal.hpp"
#include <Windows.h>
#include <LuminoEngine/Engine/CoreApplication.hpp>
//#include "../../LuminoEngine/include/LuminoEngine/Engine/Application.hpp"
#include <LuminoPlatform/Win32PlatformInterface.hpp>
#include "../GLFWPlatformWindowManager.hpp"

//#if defined(_WIN32) && !defined(LUMINO_BUILD_DLL)
//extern "C" ::ln::Application* LuminoCreateApplicationInstance();
//#endif

namespace ln {

static Ref<ln::CoreApplication> g_app = nullptr;

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
int Win32PlatformInterface::WinMain(CoreApplication* app)
{
    g_app = app;
	AppIntegration::run(g_app);
    g_app = nullptr;
	return 0;
}

//ln::Path Win32PlatformInterface::findParentDirectoryContainingSpecifiedFile(StringView file)
//{
//	return detail::EngineManager::findParentDirectoryContainingSpecifiedFile(file);
//}

} // namespace ln

