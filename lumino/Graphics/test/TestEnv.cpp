#include <LuminoEngine/RuntimeModule.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoGraphics/RHIModule.hpp>
#include <LuminoGraphics/detail/GraphicsManager.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>
#include <LuminoGraphics/Rendering/RenderingPipeline/FlatRenderingPipeline.hpp>
#include "TestEnv.hpp"


bool TestEnv::isCI = false;
Ref<SwapChain> TestEnv::swapChain;
Ref<FlatRenderingPipeline> TestEnv::flatRenderingPipeline;
Ref<TestRenderVew> TestEnv::renderView;

void TestEnv::setup() {
    if (Environment::getEnvironmentVariable(U"LN_BUILD_FROM_CI")) {
        isCI = true;
        printf("LN_BUILD_FROM_CI defined.\n");
    }

    auto graphicsAPI = GraphicsAPI::Default;
    auto windowSystem = ln::WindowSystem::Native;
    detail::GraphicsManager::selectDefaultSystem(&graphicsAPI, &windowSystem);

    if (!isCI) {
        //graphicsAPI = GraphicsAPI::Vulkan;
        //if (graphicsAPI == GraphicsAPI::OpenGL) {
        //    windowSystem = ln::WindowSystem::GLFWWithOpenGL;
        //}
    }

    RuntimeModule::initialize();
    PlatformModule::initialize({ { U"Test", 160, 120 }, windowSystem });
    RHIModule::initialize({ graphicsAPI });

    TestHelper::setAssetsDirPath(LN_LOCALFILE("Assets"));
    TestHelper::setTempDirPath(LN_LOCALFILE("tmp"));

    swapChain = makeObject<SwapChain>(Platform::mainWindow());
}

void TestEnv::initializeRendering() {
    if (detail::RenderingManager::instance()) return;

    detail::RenderingManager::Settings renderingManagerSettings;
    renderingManagerSettings.graphicsManager = detail::GraphicsManager::instance();
    renderingManagerSettings.fontManager = nullptr;
    detail::RenderingManager::initialize(renderingManagerSettings);

    flatRenderingPipeline = makeObject<FlatRenderingPipeline>();
    renderView = makeObject<TestRenderVew>();
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
    RHIModule::terminate();
    PlatformModule::terminate();
    RuntimeModule::terminate();
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
