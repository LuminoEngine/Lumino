
#include "Internal.hpp"
#include "EmptyPlatformWindowManager.hpp"
#include "GLFWPlatformWindowManager.hpp"
#include "Windows/Win32PlatformWindowManager.hpp"
#include "PlatformManager.hpp"

namespace ln {
namespace detail {


PlatformManager* PlatformManager::initialize(const Settings& settings)
{
    if (instance()) return instance();

    auto m = Ref<PlatformManager>(LN_NEW detail::PlatformManager(), false);
    if (!m->init(settings)) return nullptr;

    EngineContext2::instance()->registerModule(m);
    EngineContext2::instance()->platformManager = m;
    return m;
}

void PlatformManager::terminate()
{
    if (instance()) {
        instance()->dispose();
        EngineContext2::instance()->unregisterModule(instance());
        EngineContext2::instance()->platformManager = nullptr;
    }
}

PlatformManager::PlatformManager()
	: m_windowManager()
    , m_glfwWithOpenGLAPI(true)
    , m_messageLoopProcessing(true)
{
}

PlatformManager::~PlatformManager()
{
}

Result PlatformManager::init(const Settings& settings)
{
#ifdef LN_GLFW
    if (settings.useGLFWWindowSystem && !settings.mainWindowSettings.userWindow) {
        if (!m_windowManager) {
            auto windowManager = ln::makeRef<GLFWPlatformWindowManager>(this);
            if (!windowManager->init()) {
                return false;
            }
            m_windowManager = windowManager;
        }
    }
#endif
#ifdef LN_OS_WIN32
    if (!m_windowManager) {
        auto windowManager = ln::makeRef<Win32PlatformWindowManager>(this);
        if (!windowManager->init()) {
            return false;
        }
        m_windowManager = windowManager;
    }
#endif

    m_glfwWithOpenGLAPI = settings.glfwWithOpenGLAPI;

    // ユーザーウィンドウが指定されている場合、イベント処理は外部に任せる
    // ※ HSP3 ランタイム上では、Lumino 側がイベント処理してしまうと、termfunc() が呼ばれなくなる
    if (settings.mainWindowSettings.userWindow) {
        m_messageLoopProcessing = false;
    }

	if (!m_windowManager)
	{
		auto windowManager = ln::makeRef<EmptyPlatformWindowManager>(this);
        if (!windowManager->init()) {
            return false;
        }
		m_windowManager = windowManager;
	}

	m_mainWindow = m_windowManager->createMainWindow(settings.mainWindowSettings);

    return true;
}

void PlatformManager::dispose()
{
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

OpenGLContext* PlatformManager::openGLContext() const
{
    return m_windowManager->getOpenGLContext();
}

void PlatformManager::processSystemEventQueue()
{
    if (m_messageLoopProcessing) {
        m_windowManager->processSystemEventQueue(EventProcessingMode::Polling);
    }
}

} // namespace detail
} // namespace ln

