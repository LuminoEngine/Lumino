#include <emscripten.h>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoShaderCompiler/detail/ShaderManager.hpp>
#include <LuminoGraphics/Shader.hpp>
#include <LuminoGraphics/ShaderDescriptor.hpp>
#include <LuminoGraphics/VertexLayout.hpp>
#include <LuminoGraphics/VertexBuffer.hpp>
#include <LuminoGraphics/SwapChain.hpp>
#include <LuminoGraphics/GraphicsContext.hpp>
#include "../src/GraphicsManager.hpp"
using namespace ln;

#define ASSETFILE(x) ln::Path(ASSETS_DIR, U##x)

Ref<Shader> g_shader;
Ref<VertexLayout> g_vertexLayout;
Ref<VertexBuffer> g_vertexBuffer;
Ref<SwapChain> g_swapChain;

void init() {
    EngineContext2::initialize();
    Logger::setLevel(LogLevel::Debug);

    detail::PlatformManager::Settings platformManagerrSettings;
    platformManagerrSettings.glfwWithOpenGLAPI = true;
    auto platformManager = detail::PlatformManager::initialize(platformManagerrSettings);

    detail::AssetManager::Settings assetManagerSettings;
    auto assetManager = detail::AssetManager::initialize(assetManagerSettings);
    assetManager->mountAssetArchive(U"Assets.lna", StringView());

    detail::GraphicsManager::Settings graphicsManagerSettings;
    graphicsManagerSettings.assetManager = assetManager;
    graphicsManagerSettings.platformManager = platformManager;
    graphicsManagerSettings.mainWindow = platformManager->mainWindow();
    graphicsManagerSettings.graphicsAPI = GraphicsAPI::OpenGL;
    graphicsManagerSettings.priorityGPUName = U"";
    graphicsManagerSettings.debugMode = true;
    auto graphicsManager = detail::GraphicsManager::initialize(graphicsManagerSettings);

    detail::ShaderManager::Settings shaderManagerSettings;
    shaderManagerSettings.graphicsManager = graphicsManager;
    auto shaderManager = detail::ShaderManager::initialize(shaderManagerSettings);
}

void initApp() {
    auto window = detail::PlatformManager::instance()->mainWindow();

    g_shader = Shader::load(U"simple");

    g_vertexLayout = makeObject<VertexLayout>();
    g_vertexLayout->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

    // CCW
    Vector4 v[] = {
        Vector4(0, 0.5, 0, 1),
        Vector4(-0.5, -0.25, 0, 1),
        Vector4(0.5, -0.25, 0, 1),
    };
    g_vertexBuffer = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);
    // TODO: ローカス変数にして解放すると、のちに null アクセスエラーになる

    int backbufferWidth;
    int backbufferHeight;
    window->getFramebufferSize(&backbufferWidth, &backbufferHeight);
    g_swapChain = makeObject<SwapChain>(window, SizeI(backbufferWidth, backbufferHeight));
}

void cleanupApp() {
    g_vertexBuffer = nullptr;
    g_swapChain = nullptr;
}

void cleanup() {
    detail::GraphicsManager::terminate();
    detail::AssetManager::terminate();
    detail::PlatformManager::terminate();
    EngineContext2::terminate();
}

void run() {
    Platform::processEvents();
    
    auto descriptorLayout = g_shader->descriptorLayout();
    auto shaderPass = g_shader->techniques()[0]->passes()[0];

    auto ctx = g_swapChain->beginFrame2();
    auto descriptor = ctx->allocateShaderDescriptor(shaderPass);
    descriptor->setVector(descriptorLayout->findUniformMemberIndex(U"_Color"), Vector4(1, 0, 0, 1));
    auto renderPass = g_swapChain->currentRenderPass();
    ctx->beginRenderPass(renderPass);
    ctx->clear(ClearFlags::All, Color::Azure);
    ctx->setVertexLayout(g_vertexLayout);
    ctx->setVertexBuffer(0, g_vertexBuffer);
    ctx->setShaderPass(shaderPass);
    ctx->setShaderDescriptor(descriptor);
    ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    ctx->drawPrimitive(0, 1);
    ctx->endRenderPass();

    g_swapChain->endFrame();
    printf("run 7\n");
}

int main() {
    init();
    initApp();
    emscripten_set_main_loop(run, 0, true);
    cleanupApp();
    cleanup();
    return 0;
}
