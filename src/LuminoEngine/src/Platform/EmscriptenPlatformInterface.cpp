
#ifdef __EMSCRIPTEN__

#include <stdio.h>
#include <emscripten.h>
#include "Internal.hpp"
#include <LuminoEngine/Engine/Application.hpp>

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

static ln::Application* g_app = nullptr;

static void ln_main_loop()
{
    ln::detail::ApplicationHelper::processTick(g_app);
}

int main(int argc, char** argv)
{
    ln::GlobalLogger::addStdErrAdapter();
    
    g_app = ::LuminoCreateApplicationInstance();

    ln::detail::ApplicationHelper::initialize(g_app);

	emscripten_set_main_loop(ln_main_loop, 0, true);
    // the next to emscripten_set_main_loop is unreachable.

	return 0;
}

#endif
