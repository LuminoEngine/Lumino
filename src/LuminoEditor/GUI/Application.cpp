
#include "../../LuminoEngine/src/Engine/EngineManager.hpp"
#include "../../LuminoEngine/src/Engine/EngineDomain.hpp"
#include "../../LuminoEngine/src/Platform/PlatformManager.hpp"
#include "../../LuminoEngine/src/UI/UIManager.hpp"
#include "MainWindow.hpp"
#include "Application.hpp"

static EditorApplication* s_app = nullptr;

EditorApplication* EditorApplication::instance()
{
    assert(s_app);
    return s_app;
}

EditorApplication::EditorApplication()
{
    assert(!s_app);
    s_app = this;
}

EditorApplication::~EditorApplication()
{
    s_app = nullptr;
}

ln::Result EditorApplication::init()
{
    ln::detail::EngineDomain::engineManager()->settings().defaultObjectsCreation = false;
    ln::detail::EngineDomain::engineManager()->init();
    onInit();
    return true;
}

void EditorApplication::dispose()
{
    ln::detail::EngineDomain::release();
}

void EditorApplication::run()
{
    while (!ln::detail::EngineDomain::engineManager()->isExitRequested())
    {
        ln::detail::EngineDomain::engineManager()->platformManager()->windowManager()->processSystemEventQueue(ln::detail::EventProcessingMode::Polling);
        ln::detail::EngineDomain::engineManager()->uiManager()->dispatchPostedEvents();
        ln::Thread::sleep(32);
    }
}

ln::UIMainWindow* EditorApplication::mainWindow() const
{
    return ln::detail::EngineDomain::engineManager()->mainWindow();
}

void EditorApplication::setMainWindow(ln::UIMainWindow* window)
{
	return ln::detail::EngineDomain::engineManager()->setMainWindow(window);
}

void EditorApplication::onInit()
{
	setMainWindow(ln::makeObject<MainWindow>());

	// TODO:
	mainWindow()->m_renderView->setClearMode(ln::RenderViewClearMode::ColorAndDepth);
	mainWindow()->m_renderView->setBackgroundColor(ln::Color::White);
}
