#pragma once
#include <LuminoEngine/Engine/EngineInstance.hpp>
#include "../Common.hpp"

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

    Ref<PlatformWindow> createWindow(const WindowCreationSettings& settings);
    OpenGLContext* openGLContext() const;
    void processSystemEventQueue();
    void requestQuit() { m_quitRequested = true; }
    bool shouldQuit() const { return m_quitRequested; }

    virtual ~PlatformManager();

    PlatformManager();
    Result<> init(const Settings& settings);
    void dispose();

private:

    Ref<PlatformWindowManager> m_windowManager;
    Ref<PlatformWindow> m_mainWindow; // v0.5.0 で持たないことを検討したが、Graphics, UI との初期化順の関係や、Android, Emscripten など既に出来上がっている View にアタッチしたいときなどに欲しい
    // bool m_glfwWithOpenGLAPI;
    bool m_messageLoopProcessing;
    bool m_quitRequested;
};

} // namespace detail
} // namespace ln
