#include <LuminoCore.hpp>
#include <LuminoEngine/Engine/Engine.hpp>

int main() {
    ln::EngineOptions options;
    options.audio.enabled = true;
    ln::Engine::initialize(options);


    ln::Engine::terminate();
    return 0;
}
