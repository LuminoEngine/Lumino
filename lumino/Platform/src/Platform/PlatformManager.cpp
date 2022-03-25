
#include "Internal.hpp"
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include "EmptyPlatformWindowManager.hpp"
#include "GLFWPlatformWindowManager.hpp"
#include "Windows/Win32PlatformWindowManager.hpp"

namespace ln {
namespace detail {

PlatformManager* PlatformManager::initialize(const Settings& settings) {
    if (instance()) return instance();

    auto m = Ref<PlatformManager>(LN_NEW detail::PlatformManager(), false);
    if (!m->init(settings)) return nullptr;

    EngineContext2::instance()->registerModule(m);
    EngineContext2::instance()->platformManager = m;
    return m;
}

void PlatformManager::terminate() {
    if (instance()) {
        instance()->dispose();
        EngineContext2::instance()->unregisterModule(instance());
        EngineContext2::instance()->platformManager = nullptr;
    }
}

PlatformManager::PlatformManager()
    : m_windowManager()
    //, m_glfwWithOpenGLAPI(true)
    , m_messageLoopProcessing(true)
    , m_quitRequested(false) {
}

PlatformManager::~PlatformManager() {
}

Result PlatformManager::init(const Settings& settings) {
#ifdef LN_GLFW
    if (settings.windowSystem == WindowSystem::GLFWWithOpenGL || settings.windowSystem == WindowSystem::GLFWWithoutOpenGL) {
        if (!m_windowManager) {
            auto windowManager = ln::makeRef<GLFWPlatformWindowManager>(this);
            LN_TRY(windowManager->init(settings.windowSystem == WindowSystem::GLFWWithOpenGL));
            m_windowManager = windowManager;
        }
    }
#endif
#ifdef LN_OS_WIN32
    if (!m_windowManager) {
        auto windowManager = ln::makeRef<Win32PlatformWindowManager>(this);
        if (!windowManager->init()) {
            return err();
        }
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
        if (!windowManager->init()) {
            return err();
        }
        m_windowManager = windowManager;
    }

    if (settings.mainWindowSettings.clientWidth != 0 || settings.mainWindowSettings.clientHeight != 0) {
        m_mainWindow = createWindow(settings.mainWindowSettings);
    }

    return ok();
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

Ref<PlatformWindow> PlatformManager::createWindow(const WindowCreationSettings& settings) {
    if (!m_mainWindow) {
        auto window = m_windowManager->createWindow(settings, nullptr);
        m_mainWindow = window;
        return window;
    }
    else {
        return m_windowManager->createWindow(settings, m_mainWindow);
    }
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
