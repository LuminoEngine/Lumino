
#include "Internal.hpp"
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

} // namespace detail
} // namespace ln

