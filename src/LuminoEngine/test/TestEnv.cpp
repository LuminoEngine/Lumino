#include "Common.hpp"
#include "TestEnv.hpp"

void TestEnv::setup()
{
	GlobalLogger::addStdErrAdapter();
	EngineSettings::setMainWindowSize(160, 120);
	EngineSettings::setMainBackBufferSize(160, 120);
	Engine::initialize();
}

void TestEnv::teardown()
{
	Engine::terminate();
}


