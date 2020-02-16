
#include "App.h"

HelloApp::HelloApp()
{
	ln::GlobalLogger::setLevel(ln::LogLevel::Debug);
}

void HelloApp::onStart()
{
}

void HelloApp::onUpdate()
{
}

LUMINO_APP(HelloApp);

