#include <emscripten.h>
#include <LuminoCore.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
using namespace ln;

struct EventListener : IPlatforEventListener
{
    bool onPlatformEvent(const PlatformEventArgs& e) override
    {
        printf("event %d\n", e.type);
        return false;
    }
};

static void mainLoop()
{
    Platform::processEvents();
}

int main(int argc, char** argv)
{
    Logger::addStdErrAdapter();
    if (!PlatformModule::initialize()) return 1;

    EventListener listener;
    Platform::mainWindow()->attachEventListener(&listener);

	emscripten_set_main_loop(mainLoop, 60, true);

	return 0;
}


/*
cd build/buildtrees/wasm32-emscripten/lumino/lumino/Platform/examples/Web
python -m http.server 8080
http://localhost:8080

https://alex-wasm.appspot.com/threads/index.html
*/
