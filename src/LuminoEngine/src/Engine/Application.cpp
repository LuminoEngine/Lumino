
#include "Internal.hpp"
#include "EngineManager.hpp"
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/UI/UICommand.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/Engine/Application.hpp>
#include "UI/UIManager.hpp"

namespace ln {

//==============================================================================
// Application

LN_OBJECT_IMPLEMENT(Application, Object) {}

Application::Application()
	: m_manager(nullptr)
{
    detail::EngineManager::s_settings.application = this;
}

void Application::init()
{
    Object::init();
}

Application::~Application()
{
}

void Application::onInit()
{
}

void Application::onStart()
{
}

void Application::onUpdate()
{
}

void Application::onStop()
{
}

void Application::onDestroy()
{
}

void Application::addApplicationCommand(UICommand* command)
{
    m_commands.add(command);
}

void Application::addAction(UIAction* action)
{
    m_actions.add(action);
}

World* Application::world() const
{
	return m_manager->mainWorld();
}

void Application::onRoutedEvent(UIEventArgs* e)
{
    // ショートカットキーの実装
    for (auto& c : m_commands) {
        if (c->testInputEvent(e)) {
            e->handled = true;
            UICommandEventArgs::raiseExecute(detail::EngineDomain::uiManager()->forcusedElement(), c);
            return;
        }
    }


    if (detail::UICommandInternal::handleCommandRoutedEvent(e, m_actions)) {
        return;
    }
}

void Application::initInternal()
{
	EngineContext::current()->initializeEngineManager();
	EngineContext::current()->engineManager()->initializeAllManagers();
	m_manager = detail::EngineDomain::engineManager();
	onInit();
	onStart();
}

bool Application::updateInertnal()
{
	detail::EngineDomain::engineManager()->updateFrame();
	detail::EngineDomain::engineManager()->renderFrame();
	onUpdate();
	detail::EngineDomain::engineManager()->presentFrame();
	return !detail::EngineDomain::engineManager()->isExitRequested();
}

void Application::finalizeInternal()
{
	onStop();
	onDestroy();
	detail::EngineDomain::release();
}

void Application::run()
{
#ifdef __EMSCRIPTEN__
	LN_UNREACHABLE();
#endif
	initInternal();

	while (updateInertnal());

	finalizeInternal();
}

//==============================================================================
// ApplicationHelper

namespace detail {

void ApplicationHelper::init(Application* app)
{
	app->initInternal();
}

bool ApplicationHelper::processTick(Application* app)
{
	return app->updateInertnal();
}

void ApplicationHelper::finalize(Application* app)
{
	app->finalizeInternal();
}

void ApplicationHelper::run(Application* app)
{
	app->run();
}

} // namespace detail
} // namespace ln

