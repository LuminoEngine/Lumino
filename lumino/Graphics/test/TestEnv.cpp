#include <LuminoEngine/RuntimeModule.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoGraphics/RHIModule.hpp>
#include "TestEnv.hpp"

void TestEnv::setup() {
    auto graphicsAPI = GraphicsAPI::OpenGL;

    auto windowSystem = ln::WindowSystem::Native;
    if (graphicsAPI == GraphicsAPI::OpenGL) {
        windowSystem = ln::WindowSystem::GLFWWithOpenGL;
    }

    RuntimeModule::initialize();
    PlatformModule::initialize({ { U"Test", 160, 120 }, windowSystem });
    RHIModule::initialize({ graphicsAPI });

    TestHelper::setAssetsDirPath(LN_LOCALFILE("Assets"));
    TestHelper::setTempDirPath(LN_LOCALFILE("tmp"));
}

void TestEnv::teardown() {
    RHIModule::terminate();
    PlatformModule::terminate();
    RuntimeModule::terminate();
}
