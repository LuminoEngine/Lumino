#define NOMINMAX
#include <Windows.h>
#include <LuminoCore.hpp>
#include <LuminoEngine/Runtime/Runtime.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
using namespace ln;

bool g_closed = false;

struct EventListener : IPlatforEventListener {
    bool onPlatformEvent(const PlatformEventArgs& e) override {
        switch (e.type) {
            case PlatformEventType::close:
                g_closed = true;
                break;
        }
        return false;
    }
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    if (!Runtime::initialize({})) return 1;
    if (!PlatformModule::initialize()) return 1;

    EventListener listener;
    Platform::mainWindow()->attachEventListener(&listener);

    while (!g_closed) {
        Platform::processEvents();
    }

    PlatformModule::terminate();
    Runtime::terminate();
    return 0;
}
