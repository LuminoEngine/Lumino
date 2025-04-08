#pragma once
#include <LuminoEngine/Engine/EngineInstance.hpp>
#include "../Common.hpp"
#include "../../Rendering/Common.hpp"

namespace ln {
namespace detail {
class PlatformWindowManager;

class PlatformManager : public RefObject {
public:
    struct Settings {
        [[deprecated]]
        WindowCreationSettings mainWindowSettings;
        [[deprecated]]
        WindowSystem windowSystem = WindowSystem::Native;
    };

    //static PlatformManager* initialize(const Settings& settings);
    //static void terminate();

    const Ref<PlatformWindowManager>& windowManager() const { return m_windowManager; }

    void setMainWindow(PlatformWindow* window);
    const Ref<PlatformWindow>& mainWindow() const { return m_mainWindow; }
    // bool glfwWithOpenGLAPI() const { return m_glfwWithOpenGLAPI; }

    Result_deprecated<Ref<PlatformWindow>> createWindow(const WindowCreationSettings& settings);
    OpenGLContext* openGLContext() const;
    void processSystemEventQueue();
    void requestQuit() { m_quitRequested = true; }
    bool shouldQuit() const { return m_quitRequested; }


    PlatformManager(GraphicsManager* graphicsManager, RenderingManager* renderingManager);
    virtual ~PlatformManager();
    MaybeResult init(const Settings& settings);
    void dispose();

private:
    GraphicsManager* m_graphicsManager;
    RenderingManager* m_renderingManager;
    Ref<PlatformWindowManager> m_windowManager;
    Ref<PlatformWindow> m_mainWindow; // v0.5.0 で持たないことを検討したが、Graphics, UI との初期化順の関係や、Android, Emscripten など既に出来上がっている View にアタッチしたいときなどに欲しい
    // bool m_glfwWithOpenGLAPI;
    bool m_messageLoopProcessing;
    bool m_quitRequested;
};

} // namespace detail
} // namespace ln
