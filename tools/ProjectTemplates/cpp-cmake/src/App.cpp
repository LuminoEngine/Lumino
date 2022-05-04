
#include "App.h"

HelloApp::HelloApp() {
	ln::Logger::setLevel(ln::LogLevel::Debug);
}

void HelloApp::onInit() {
}

void HelloApp::onUpdate() {
}

LUMINO_APP(HelloApp);

