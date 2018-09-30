
#include "Internal.hpp"
#include "EngineManager.hpp"
#include <LuminoEngine/Engine/Application.hpp>

namespace ln {

//==============================================================================
// Application

Application::Application()
{
}

Application::~Application()
{
}

void Application::onCreate()
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

void Application::initInternal()
{
	detail::EngineDomain::engineManager()->initialize();
	onCreate();
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

void ApplicationHelper::initialize(Application* app)
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

