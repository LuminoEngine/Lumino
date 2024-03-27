#include "Common.hpp"
#include <LuminoEngine/detail/GraphicsManager.hpp>
#include "../src/Engine/EngineManager.hpp"
#include "../src/Scene/SceneManager.hpp"
#include "TestEnv.hpp"

bool TestEnv::isCI = false;
String TestEnv::LuminoCLI;
Ref<DepthBuffer> TestEnv::depthBuffer;
RenderTargetTexture* TestEnv::lastBackBuffer = nullptr;


void TestEnv::initialize() {
    if (Environment::getEnvironmentVariable(U"LN_BUILD_FROM_CI")) {
        isCI = true;
        printf("LN_BUILD_FROM_CI defined.\n");
    }
}

void TestEnv::setup(GraphicsAPI graphicsAPI) {

    auto windowSystem = ln::WindowSystem::Native;
	if (graphicsAPI == GraphicsAPI::Default) {
        detail::GraphicsManager::selectDefaultSystem(&graphicsAPI, &windowSystem);
    }

    //if (!isCI) {
    //    graphicsAPI = GraphicsAPI::Vulkan; //GraphicsAPI::OpenGL;//GraphicsAPI::DirectX12;//
        if (graphicsAPI == GraphicsAPI::OpenGL) {
            windowSystem = ln::WindowSystem::GLFWWithOpenGL;
        }
    //}

    EngineFeature feature = EngineFeature::Experimental; //EngineFeature::Public; //
    EngineSettings::setMainWindowSize(160, 120);
    EngineSettings::setGraphicsAPI(graphicsAPI);
    EngineSettings::setPriorityGPUName(_TT("Microsoft Basic Render Driver"));
    EngineSettings::setGraphicsDebugEnabled(true);
    EngineSettings::setEngineFeatures(feature);
    EngineSettings::addAssetDirectory(LN_LOCALFILE(_TT("Assets")));
    Engine::initialize();
    Engine::setupMainWindow(nullptr);
    detail::EngineDomain::engineManager()->sceneManager()->autoAddingToActiveWorld = true;

    if (feature == EngineFeature::Experimental) // Experimental
    {
        Font::registerFontFromFile(LN_LOCALFILE("../../../tools/VLGothic/VL-Gothic-Regular.ttf"));
        Engine::mainCamera()->setBackgroundColor(Color(0.5, 0.5, 0.5, 1.0));

        //Engine::mainAmbientLight()->setColor(Color::White);
        //Engine::mainAmbientLight()->setIntensity(0.5);
        //Engine::mainDirectionalLight()->setColor(Color::White);
        //Engine::mainDirectionalLight()->setIntensity(0.5);
        //Engine::mainDirectionalLight()->setPosition(10, 10, -10);
        //Engine::mainDirectionalLight()->lookAt(Vector3(0, 0, 0));
    }

    const auto backbufferSize = Engine::mainWindow()->swapChain()->backbufferSize();
    depthBuffer = DepthBuffer::create(backbufferSize.width, backbufferSize.height);

#ifdef LN_OS_WIN32
    LuminoCLI = Path::combine(Path(ln::Environment::executablePath()).parent().parent().parent().parent(), _TT("tools"), _TT("LuminoCLI"), _TT("Debug"), _TT("lumino-cli.exe"));
#else
#endif
    LN_TEST_CLEAN_SCENE;
}

void TestEnv::teardown() {
    depthBuffer = nullptr;
    detail::EngineDomain::release();
}

void TestEnv::updateFrame() {
    lastBackBuffer = TestEnv::mainWindowSwapChain()->currentBackbuffer();
    detail::EngineDomain::engineManager()->updateFrame();
    detail::EngineDomain::engineManager()->presentFrame(nullptr, nullptr);
}

void TestEnv::resetScene() {
    Engine::world()->removeAllObjects();
    Engine::mainUIView()->removeAllChildren();
#ifdef LN_COORD_RH
    Engine::mainCamera()->setPosition(0, 0, 10);
#else
    Engine::mainCamera()->setPosition(0, 0, -10);
#endif
    Engine::mainCamera()->lookAt(Vector3(0, 0, 0));
    Engine::mainLight()->lookAt(0, -1, 0);
    Engine::mainLight()->setEnabled(true);
    Engine::mainLight()->setSkyColor(Color::Black);
    Engine::mainLight()->setGroundColor(Color::Black);
    Engine::mainLight()->setAmbientColor(Color(0.5, 0.5, 0.5));
    Engine::mainLight()->setColor(Color::White);
    Engine::mainLight()->setIntensity(1.0f);
}

GraphicsCommandList* TestEnv::graphicsContext() {
    return Engine::graphicsContext();
}

GraphicsContext* TestEnv::mainWindowSwapChain() {
    return Engine::mainWindow()->swapChain();
}

void TestEnv::resetGraphicsContext(GraphicsCommandList* context) {
    context->resetState();
    //context->setRenderTarget(0, Engine::mainWindow()->swapChain()->currentBackbuffer());
    //context->setDepthBuffer(depthBuffer);
}

GraphicsCommandList* TestEnv::beginFrame() {
    lastBackBuffer = TestEnv::mainWindowSwapChain()->currentBackbuffer();
    auto commandList = TestEnv::mainWindowSwapChain()->currentCommandList2();
    commandList->beginCommandRecoding();
    return commandList;
}

void TestEnv::endFrame() {
    auto commandList = TestEnv::mainWindowSwapChain()->currentCommandList2();
    commandList->endCommandRecoding();
    TestEnv::mainWindowSwapChain()->present();
}

RenderPass* TestEnv::renderPass() {
    return TestEnv::mainWindowSwapChain()->currentRenderPass();
}

bool TestEnv::checkCurrentBackbufferScreenShot(const Path& filePath, int passRate, bool save) {
    return GraphicsTestHelper::checkScreenShot(filePath, lastBackBuffer, passRate, save);
}
