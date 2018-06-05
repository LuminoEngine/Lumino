
#include <stdio.h>
#include <emscripten.h>

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

static ln::Application* g_app = nullptr;

static void ln_main_loop()
{
    ln::detail::ApplicationHelper::processTick(g_app);
    ln::Engine::update();
}

int main(int argc, char** argv)
{
    printf("run test.");

    ln::Engine::initialize();

    g_app = ::LuminoCreateApplicationInstance();

    ln::detail::ApplicationHelper::initialize(g_app);

    printf("initialized.");

	emscripten_set_main_loop(ln_main_loop, 0, true);

    // the next to emscripten_set_main_loop is unreachable.
    
	//printf("run test.");
    //main2();
	return 0;
}

