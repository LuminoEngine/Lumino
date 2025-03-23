#include <LuminoEngine/Engine/Engine.hpp>
#include <LuminoEngine/Platform/PlatformModule.hpp>
#include <LuminoEngine/Platform/Platform.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/RHIModule.hpp>
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/RenderingPipeline/FlatRenderingPipeline.hpp>
#include "TestEnv.hpp"


bool TestEnv::isCI = false;
Ref<PlatformWindow> TestEnv::mainWindow;
Ref<GraphicsContext> TestEnv::swapChain;
Ref<FlatRenderingPipeline> TestEnv::flatRenderingPipeline;
Ref<TestRenderVew> TestEnv::renderView;

void TestEnv::setup() {
    if (Environment::getEnvironmentVariable(U"LN_BUILD_FROM_CI")) {
        isCI = true;
        printf("LN_BUILD_FROM_CI defined.\n");
    }

    auto graphicsAPI = GraphicsAPI::Vulkan;
    auto windowSystem = ln::WindowSystem::Native;
    detail::GraphicsManager::selectDefaultSystem(&graphicsAPI, &windowSystem);

    if (!isCI) {
        //graphicsAPI = GraphicsAPI::Vulkan;
        //if (graphicsAPI == GraphicsAPI::OpenGL) {
        //    windowSystem = ln::WindowSystem::GLFWWithOpenGL;
        //}
    }

    EngineOptions options;
    options.platform.title = U"Test";
    options.platform.width = 160;
    options.platform.height = 120;
    options.platform.windowSystem = windowSystem;
    options.graphics.enabled = true;
    options.graphics.graphicsAPI = graphicsAPI;
    Engine::initialize(options);

    TestHelper::setAssetsDirPath(LN_LOCALFILE("Assets"));
    TestHelper::setTempDirPath(LN_LOCALFILE("tmp"));

    WindowCreationSettings windowOptions;
    windowOptions.title = U"Test";
    windowOptions.clientWidth = 160;
    windowOptions.clientHeight = 120;
    mainWindow = EngineManager::instance()->platformManager()->createWindow(windowOptions);

    swapChain = GraphicsContext::create(mainWindow);
}

void TestEnv::initializeRendering() {
    if (renderView) return;

    //detail::RenderingManager::Settings renderingManagerSettings;
    //renderingManagerSettings.graphicsManager = detail::GraphicsManager::instance();
    //renderingManagerSettings.fontManager = nullptr;
    //detail::RenderingManager::initialize(renderingManagerSettings);

    flatRenderingPipeline = makeObject_deprecated<FlatRenderingPipeline>();
    renderView = makeObject_deprecated<TestRenderVew>();
    renderView->setRenderingPipeline(flatRenderingPipeline);
    renderView->setClearMode(SceneClearMode::ColorAndDepth);
    renderView->setBackgroundColor(Color::Gray);
    renderView->resetView(Vector3(0, 0, 5));
}

void TestEnv::teardown() {
    renderView = nullptr;
    flatRenderingPipeline = nullptr;
    swapChain = nullptr;

    detail::RenderingManager::terminate();
    Engine::terminate();
}

void TestRenderVew::resetView(const Vector3& pos, const Vector3& lookAt) {
    m_pos = pos;
    m_lookAt = lookAt;
}

void TestRenderVew::onUpdateViewPoint(RenderViewPoint* viewPoint, RenderTargetTexture* renderTarget) {
    auto dir = Vector3::normalize(m_lookAt - m_pos);
    viewPoint->resetPerspective(
        m_pos,
        dir,
        Math::PI / 3,   // Camera class default.
        Size(renderTarget->width(), renderTarget->height()), 1.0f, 1000.0f);
}

void TestRenderVew::onRender(GraphicsCommandList* graphicsContext, RenderingContext* renderingContext, RenderTargetTexture* renderTarget) {
}
