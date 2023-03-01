#pragma once
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoPlatform/Common.hpp>
#include "GPU/Common.hpp"

namespace ln {
namespace detail {
class AssetManager;
class PlatformManager;
} // namespace detail

struct GraphicsModuleSettings {
    GraphicsAPI graphicsAPI = GraphicsAPI::Default;
    String priorityGPUName;
    bool debugMode = false;
};

class GraphicsModule : public Module {
public:
    /**
     * Initialize Module.
     */
    static GraphicsModule* initialize(const GraphicsModuleSettings& settings = {});

    /**
     * Terminate Module.
     */
    static void terminate();

private:
};

} // namespace ln
