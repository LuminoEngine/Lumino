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

void init() {
    EngineContext2::initialize();

    detail::PlatformManager::Settings platformManagerrSettings;
    auto platformManager = detail::PlatformManager::initialize(platformManagerrSettings);

    detail::AssetManager::Settings assetManagerSettings;
    auto assetManager = detail::AssetManager::initialize(assetManagerSettings);

    detail::GraphicsManager::Settings graphicsManagerSettings;
    graphicsManagerSettings.assetManager = assetManager;
    graphicsManagerSettings.platformManager = platformManager;
    graphicsManagerSettings.mainWindow = platformManager->mainWindow();
    graphicsManagerSettings.graphicsAPI = GraphicsAPI::Vulkan;
    graphicsManagerSettings.priorityGPUName = U"";
    graphicsManagerSettings.debugMode = true;
    auto graphicsManager = detail::GraphicsManager::initialize(graphicsManagerSettings);

    detail::ShaderManager::Settings shaderManagerSettings;
    shaderManagerSettings.graphicsManager = graphicsManager;
    auto shaderManager = detail::ShaderManager::initialize(shaderManagerSettings);
}

void cleanup() {
    detail::GraphicsManager::terminate();
    detail::AssetManager::terminate();
    detail::PlatformManager::terminate();
    EngineContext2::terminate();
}

void run() {
    auto window = detail::PlatformManager::instance()->mainWindow();

    auto shader = Shader::create(ASSETFILE("simple.hlsl"));

    auto descriptorLayout = shader->descriptorLayout();

    auto shaderPass = shader->techniques()[0]->passes()[0];

    auto vertexLayout = makeObject<VertexLayout>();
    vertexLayout->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

    Vector4 v[] = {
        Vector4(0, 0.5, 0, 1),
        Vector4(-0.5, -0.25, 0, 1),
        Vector4(0.5, -0.25, 0, 1),
    };
    auto vertexBuffer = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

    int backbufferWidth;
    int backbufferHeight;
    window->getFramebufferSize(&backbufferWidth, &backbufferHeight);
    auto swapChain = makeObject<SwapChain>(window, SizeI(backbufferWidth, backbufferHeight));

    while (Platform::processEvents()) {
        auto ctx = swapChain->beginFrame2();

        auto descriptor = ctx->allocateShaderDescriptor(shaderPass);
        descriptor->setVector(descriptorLayout->findUniformMemberIndex(U"_Color"), Vector4(1, 0, 0, 1));

        auto renderPass = swapChain->currentRenderPass();
        ctx->beginRenderPass(renderPass);
        ctx->setVertexLayout(vertexLayout);
        ctx->setVertexBuffer(0, vertexBuffer);
        ctx->setShaderPass(shaderPass);
        ctx->setShaderDescriptor(descriptor);
        ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
        ctx->drawPrimitive(0, 1);
        ctx->endRenderPass();

        swapChain->endFrame();
    }
}

int main() {
    init();
    run();
    cleanup();
}
