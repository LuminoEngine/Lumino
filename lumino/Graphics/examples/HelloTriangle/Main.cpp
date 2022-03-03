#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
using namespace ln;

int main() {
    if (!EngineContext2::initialize()) return 1;
    if (!PlatformModule::initialize()) return 1;


    //while (!g_closed) {
    //    Platform::processEvents();
    //}

    PlatformModule::terminate();
    EngineContext2::terminate();

}
