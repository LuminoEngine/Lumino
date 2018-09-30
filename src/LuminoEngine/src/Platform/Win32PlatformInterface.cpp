#pragma once

#ifdef _WIN32
#include "Internal.hpp"
#include <Windows.h>
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoEngine/Platform/Win32PlatformInterface.hpp>

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

namespace ln {

static ln::Application* g_app = nullptr;

int Win32PlatformInterface::WinMain()
{
	g_app = ::LuminoCreateApplicationInstance();

	ln::detail::ApplicationHelper::initialize(g_app);
	ln::detail::ApplicationHelper::run(g_app);
	ln::detail::ApplicationHelper::finalize(g_app);
	ln::RefObjectHelper::release(g_app);
	g_app = nullptr;

	return 0;
}

} // namespace ln

#endif
