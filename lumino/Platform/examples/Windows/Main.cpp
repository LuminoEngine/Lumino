#define NOMINMAX
#include <Windows.h>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
using namespace ln;

bool g_closed = false;;

struct EventListener : IPlatforEventListener
{
    bool onPlatformEvent(const PlatformEventArgs& e) override
    {
        switch (e.type) {
        case PlatformEventType::close:
            g_closed = true;
            break;
        }
        return false;
    }
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    if (!EngineContext2::initialize()) return 1;
    if (!PlatformModule::initialize()) return 1;

    EventListener listener;
    Platform::mainWindow()->attachEventListener(&listener);

    while (!g_closed) {
        Platform::processEvents();
    }

    PlatformModule::terminate();
    EngineContext2::terminate();
    return 0;
}
