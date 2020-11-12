
#include <LuminoCore.hpp>
#include "../Internal.hpp"
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoEngine/Platform/CocoaPlatformInterface.hpp>
#include "../../Engine/EngineManager.hpp"

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

namespace ln {

static ln::Application* g_app = nullptr;

int CocoaPlatformInterface::Main()
{
	g_app = ::LuminoCreateApplicationInstance();

	ln::detail::ApplicationHelper::init(g_app);
	ln::detail::ApplicationHelper::run(g_app);
	ln::detail::ApplicationHelper::finalize(g_app);
	ln::RefObjectHelper::release(g_app);
	g_app = nullptr;

	return 0;
}

void CocoaPlatformInterface::addAssetArchive(const StringRef& fileFullPath, const StringRef& password)
{
    detail::EngineManager::s_settings.assetArchives.add({ fileFullPath, password });
}

} // namespace ln

