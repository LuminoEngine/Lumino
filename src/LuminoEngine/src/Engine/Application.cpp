
#include "Internal.hpp"
#include "EngineManager.hpp"
#include <Lumino/Engine/Application.hpp>

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

void Application::run()
{
#ifdef __EMSCRIPTEN__
	LN_UNREACHABLE();
#endif

	do
	{
		detail::EngineDomain::engineManager()->updateFrame();
		detail::EngineDomain::engineManager()->renderFrame();

		onUpdate();

		detail::EngineDomain::engineManager()->presentFrame();

	} while (!detail::EngineDomain::engineManager()->isExitRequested());

}

//==============================================================================
// ApplicationHelper

namespace detail {

void ApplicationHelper::initialize(Application* app)
{
	app->onCreate();
	app->onStart();
}

bool ApplicationHelper::processTick(Application* app)
{
	app->onUpdate();
	return true;
}

void ApplicationHelper::finalize(Application* app)
{
	app->onStop();
	app->onDestroy();
}

void ApplicationHelper::run(Application* app)
{
	app->run();
}

} // namespace detail
} // namespace ln

