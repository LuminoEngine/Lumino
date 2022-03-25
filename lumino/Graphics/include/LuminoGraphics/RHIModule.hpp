﻿#pragma once
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoPlatform/Common.hpp>
#include "Common.hpp"

namespace ln {
namespace detail {
class AssetManager;
class PlatformManager;
} // namespace detail

struct RHIModuleSettings {
    detail::AssetManager* assetManager = nullptr;
    detail::PlatformManager* platformManager = nullptr;
    PlatformWindow* mainWindow = nullptr;
    GraphicsAPI graphicsAPI = GraphicsAPI::Default;
    String priorityGPUName;
    bool debugMode = false;
};

class RHIModule : public Module {
public:
    /**
     * Initialize Module.
     */
    static RHIModule* initialize(const RHIModuleSettings& settings);

    /**
     * Terminate Module.
     */
    static void terminate();

private:
};

} // namespace ln