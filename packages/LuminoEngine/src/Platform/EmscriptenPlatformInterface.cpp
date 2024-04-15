
#if 0
#ifdef __EMSCRIPTEN__

#include <stdio.h>
#include <emscripten.h>
#include "Internal.hpp"
#include <LuminoEngine/Engine/Application.hpp>
#include "../Engine/EngineManager.hpp"

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

static ln::Application* g_app = nullptr;

static void ln_main_loop()
{
    ln::detail::ApplicationHelper::processTick(g_app);
}

int main(int argc, char** argv)
{
    ln::Logger::addStdErrAdapter();
    
    g_app = ::LuminoCreateApplicationInstance();

    //ln::detail::EngineDomain::engineManager()->settings2().assetArchives.add({ u"Assets.lca", ln::StringView() });

    ln::detail::EngineSettings& settings = ln::detail::EngineManager::s_settings;
    settings.standaloneFpsControl = false;
    settings.assetArchives.add({ u"Assets.lca", ln::StringRef() });

    ln::detail::ApplicationHelper::init(g_app);

	emscripten_set_main_loop(ln_main_loop, 0, true);
    // the next to emscripten_set_main_loop is unreachable.

	return 0;
}

#endif
#endif
