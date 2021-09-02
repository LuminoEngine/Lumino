
#include "App.h"

HelloApp::HelloApp()
{
	ln::Logger::setLevel(ln::LogLevel::Debug);
}

void HelloApp::onStart()
{
}

void HelloApp::onUpdate()
{
}

LUMINO_APP(HelloApp);

