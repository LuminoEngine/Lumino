
// TODO: LuminoPlatformInterface.lib とかに分けたい。
// 今は Engine.lib に含めているので、dll を作ろうとしたときに LuminoCreateApplicationInstance が未定義エラーになってしまう。
#if defined(_WIN32) && !defined(LUMINO_BUILD_DLL)
#include "Internal.hpp"
#include <Windows.h>
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoEngine/Platform/Win32PlatformInterface.hpp>
#include "../Engine/EngineManager.hpp"
#include "GLFWPlatformWindowManager.hpp"

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

namespace ln {

static ln::Application* g_app = nullptr;

void Win32PlatformInterface::init()
{
    detail::EngineSettings& settings = detail::EngineDomain::engineManager()->settings();
    settings.standaloneFpsControl = true;

    g_app = ::LuminoCreateApplicationInstance();
    detail::ApplicationHelper::init(g_app);
}

int Win32PlatformInterface::WinMain()
{
	detail::ApplicationHelper::run(g_app);
	detail::ApplicationHelper::finalize(g_app);
	RefObjectHelper::release(g_app);
	g_app = nullptr;

	return 0;
}

} // namespace ln

#endif
