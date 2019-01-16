
#ifdef _WIN32
#include "Internal.hpp"
#include <Windows.h>
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoEngine/Platform/Win32PlatformInterface.hpp>
#include "../Engine/EngineManager.hpp"

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

namespace ln {

static ln::Application* g_app = nullptr;

void Win32PlatformInterface::initialize()
{
    g_app = ::LuminoCreateApplicationInstance();
    ln::detail::ApplicationHelper::initialize(g_app);
}

int Win32PlatformInterface::WinMain()
{
	ln::detail::ApplicationHelper::run(g_app);
	ln::detail::ApplicationHelper::finalize(g_app);
	ln::RefObjectHelper::release(g_app);
	g_app = nullptr;

	return 0;
}

} // namespace ln

#endif
