#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include "../src/GraphicsManager.hpp"
using namespace ln;

int main() {
    if (!EngineContext2::initialize()) return 1;

    detail::PlatformManager::Settings platformManagerrSettings;
    auto* platformManager = detail::PlatformManager::initialize(platformManagerrSettings);

    detail::AssetManager::Settings assetManagerSettings;
    auto* assetManager = detail::AssetManager::initialize(assetManagerSettings);

    detail::GraphicsManager::Settings graphicsManagerSettings;
    graphicsManagerSettings.assetManager = assetManager;
    graphicsManagerSettings.platformManager = platformManager;
    graphicsManagerSettings.mainWindow = platformManager->mainWindow();
    graphicsManagerSettings.graphicsAPI = GraphicsAPI::Vulkan;
    graphicsManagerSettings.priorityGPUName = U"";
    graphicsManagerSettings.debugMode = true;
    auto* graphicsManager = detail::GraphicsManager::initialize(graphicsManagerSettings);

    while (Platform::processEvents()) {
    }

    detail::GraphicsManager::terminate();
    detail::AssetManager::terminate();
    detail::PlatformManager::terminate();
    EngineContext2::terminate();
}
