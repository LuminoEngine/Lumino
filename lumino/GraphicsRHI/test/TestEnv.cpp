#include <LuminoEngine/RuntimeModule.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoGraphicsRHI/detail/GraphicsRHIManager.hpp>
#include "TestEnv.hpp"

bool TestEnv::isCI = false;

void TestEnv::setup() {
    if (Environment::getEnvironmentVariable(U"LN_BUILD_FROM_CI")) {
        isCI = true;
        printf("LN_BUILD_FROM_CI defined.\n");
    }

    //auto graphicsAPI = GraphicsAPI::Vulkan;
    auto windowSystem = ln::WindowSystem::Native;
    //detail::GraphicsManager::selectDefaultSystem(&graphicsAPI, &windowSystem);

    //if (!isCI) {
    //    //graphicsAPI = GraphicsAPI::Vulkan;
    //    //if (graphicsAPI == GraphicsAPI::OpenGL) {
    //    //    windowSystem = ln::WindowSystem::GLFWWithOpenGL;
    //    //}
    //}

    RuntimeModule::initialize();
    PlatformModule::initialize({ { U"Test", 160, 120 }, windowSystem });
    //GraphicsModule::initialize({ graphicsAPI });

    TestHelper::setAssetsDirPath(LN_LOCALFILE("Assets"));
    TestHelper::setTempDirPath(LN_LOCALFILE("tmp"));

    //swapChain = makeObject_deprecated<SwapChain>(Platform::mainWindow());
}

void TestEnv::initializeRendering() {
    //if (detail::RenderingManager::instance()) return;

    //detail::RenderingManager::Settings renderingManagerSettings;
    //renderingManagerSettings.graphicsManager = detail::GraphicsManager::instance();
    //renderingManagerSettings.fontManager = nullptr;
    //detail::RenderingManager::initialize(renderingManagerSettings);

    //flatRenderingPipeline = makeObject_deprecated<FlatRenderingPipeline>();
    //renderView = makeObject_deprecated<TestRenderVew>();
    //renderView->setRenderingPipeline(flatRenderingPipeline);
    //renderView->setClearMode(SceneClearMode::ColorAndDepth);
    //renderView->setBackgroundColor(Color::Gray);
    //renderView->resetView(Vector3(0, 0, 5));
}

void TestEnv::teardown() {
    //GraphicsModule::terminate();
    PlatformModule::terminate();
    RuntimeModule::terminate();
}
