#pragma once
#include "../Platform/Common.hpp"
#include "../GraphicsRHI/Common.hpp"
#include "Common2.hpp"

namespace ln {

struct PlatformOptions {
    bool enabled = true;
    String title = U"Lumino";
    int width = 640;
    int height = 480;
    WindowSystem windowSystem = WindowSystem::Native;
};

struct GraphicsOptions {
    bool enabled = true;
    GraphicsAPI graphicsAPI = GraphicsAPI::OpenGL;
};

struct EngineOptions {
    PlatformOptions platform;
    GraphicsOptions graphics;
};

class Engine {
public:
    static MaybeResult initialize(const EngineOptions& options);
    static void terminate();
    static void mountAssetDirectory(const StringView& path);
    static void mountAssetArchive(const StringView& filePath, const StringView& password = StringView());
    static EngineContext2* getEngineContext_();
};

} // namespace ln
