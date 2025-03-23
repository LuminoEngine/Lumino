#pragma once
#include "../Platform/Common.hpp"
#include "../Graphics/GraphicsRHI/Common.hpp"
#include "Common2.hpp"

namespace ln {

struct PlatformOptions {
    bool enabled = true;
    [[dprecated]]
    String title = U"Lumino";
    [[dprecated]]
    int width = 640;
    [[dprecated]]
    int height = 480;
    [[dprecated]]
    WindowSystem windowSystem = WindowSystem::Native;
};

struct GraphicsOptions {
    bool enabled = true;
    GraphicsAPI graphicsAPI = GraphicsAPI::OpenGL;
};

struct AudioOptions {
    bool enabled = false;
};

struct EngineOptions {
    PlatformOptions platform;
    GraphicsOptions graphics;
    AudioOptions audio;
};

class Engine {
public:
    static MaybeResult initialize(const EngineOptions& options);
    static void terminate();
    static void mountAssetDirectory(const StringView& path);
    static void mountAssetArchive(const StringView& filePath, const StringView& password = StringView());
    static EngineManager* getEngineContext_();
};

} // namespace ln
