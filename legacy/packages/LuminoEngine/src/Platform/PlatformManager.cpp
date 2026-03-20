#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include <LuminoEngine/Rendering/SurfaceContext.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include "EmptyPlatformWindowManager.hpp"
#include "GLFWPlatformWindowManager.hpp"
#include "Windows/Win32PlatformWindowManager.hpp"

namespace ln {
namespace detail {

PlatformManager::PlatformManager(GraphicsManager* graphicsManager, RenderingManager* renderingManager)
    : m_graphicsManager(graphicsManager)
    , m_renderingManager(renderingManager)
    , m_windowManager()
    //, m_glfwWithOpenGLAPI(true)
    , m_messageLoopProcessing(true)
    , m_quitRequested(false) {
}

PlatformManager::~PlatformManager() {
}

MaybeResult PlatformManager::init(const Settings& settings) {
#ifdef LN_GLFW
    if (settings.windowSystem == WindowSystem::GLFWWithOpenGL || settings.windowSystem == WindowSystem::GLFWWithoutOpenGL) {
        if (!m_windowManager) {
            auto windowManager = ln::makeRef<GLFWPlatformWindowManager>(this);
            auto result = windowManager->init(settings.windowSystem == WindowSystem::GLFWWithOpenGL);
            if (!result) return LN_MAKE_ERROR();
            m_windowManager = windowManager;
        }
    }
#endif
#ifdef LN_OS_WIN32
    if (!m_windowManager && settings.windowSystem != WindowSystem::External) {
        auto windowManager = ln::makeRef<Win32PlatformWindowManager>(this);
        auto result = windowManager->init();
        if (!result) return LN_MAKE_ERROR();
        m_windowManager = windowManager;
    }
#endif

    // m_glfwWithOpenGLAPI = settings.glfwWithOpenGLAPI;

    // ユーザーウィンドウが指定されている場合、イベント処理は外部に任せる
    // ※ HSP3 ランタイム上では、Lumino 側がイベント処理してしまうと、termfunc() が呼ばれなくなる
    //if (settings.mainWindowSettings.userWindow) {
    //    m_messageLoopProcessing = false;
    //}

    if (!m_windowManager) {
        auto windowManager = ln::makeRef<EmptyPlatformWindowManager>(this);
        auto result = windowManager->init();
        if (!result) return LN_MAKE_ERROR();
        m_windowManager = windowManager;
    }

    //if (settings.mainWindowSettings.clientWidth != 0 || settings.mainWindowSettings.clientHeight != 0) {
    //    m_mainWindow = createWindow(settings.mainWindowSettings);
    //}

    return LN_MAKE_SUCCESS();
}

void PlatformManager::dispose() {
    LN_LOG_DEBUG("PlatformManager dispose started.");

    if (m_mainWindow) {
        m_windowManager->destroyWindow(m_mainWindow);
        m_mainWindow = nullptr;
    }
    if (m_windowManager) {
        m_windowManager->dispose();
        m_windowManager = nullptr;
    }

    LN_LOG_DEBUG("PlatformManager dispose finished.");
}

void PlatformManager::setMainWindow(PlatformWindow* window) {
    m_mainWindow = window;
}

Result_deprecated<Ref<PlatformWindow>> PlatformManager::createWindow(const WindowCreationSettings& settings) {
    Ref<PlatformWindow> window;
    if (!m_mainWindow) {
        window = m_windowManager->createWindow(settings, nullptr);
        m_mainWindow = window;
    }
    else {
        window = m_windowManager->createWindow(settings, m_mainWindow);
    }

    auto result = SurfaceContext::createFromWindow(m_renderingManager, window);
    if (!result) return result;
    window->m_surfaceContext = result.unwrap();

    return window;
}

OpenGLContext* PlatformManager::openGLContext() const {
    return m_windowManager->getOpenGLContext();
}

void PlatformManager::processSystemEventQueue() {
    if (m_messageLoopProcessing) {
        m_windowManager->processSystemEventQueue(EventProcessingMode::Polling);
    }
}

} // namespace detail
} // namespace ln
