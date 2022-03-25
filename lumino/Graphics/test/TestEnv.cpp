#include <LuminoEngine/RuntimeModule.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoGraphics/RHIModule.hpp>
#include "TestEnv.hpp"

void TestEnv::setup() {
    RuntimeModule::initialize();
    PlatformModule::initialize({ {U"Test", 160, 120 }, ln::WindowSystem::Native });
    RHIModule::initialize({ GraphicsAPI::Vulkan });

    TestHelper::setAssetsDirPath(LN_LOCALFILE("Assets"));
    TestHelper::setTempDirPath(LN_LOCALFILE("tmp"));
}

void TestEnv::teardown() {
    RHIModule::terminate();
    PlatformModule::terminate();
    RuntimeModule::terminate();
}
