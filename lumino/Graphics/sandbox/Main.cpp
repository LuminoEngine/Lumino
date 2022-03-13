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
    platformManagerrSettings.glfwWithOpenGLAPI = true;
    auto platformManager = detail::PlatformManager::initialize(platformManagerrSettings);

    detail::AssetManager::Settings assetManagerSettings;
    auto assetManager = detail::AssetManager::initialize(assetManagerSettings);

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

    // CCW
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

        // NOTE:
        // DX12で確認した現象。
        // ここで時間を測ると、次のように待ち時間 0[ms], 15[ms] が繰り返されることがある。
        // 
        // ```
        // 0[ms]
        // 15[ms]
        // 0[ms]
        // 15[ms]
        // ...
        // ```
        // 
        // この現象については次の回答が参考になる。
        // https://stackoverflow.com/questions/31499974/directx-11-optimization-with-waitable-object
        // つまりメインループがノーウェイトで回り続けている場合、DX12 が必要に応じてウェイトを入れることがある。
        // メインループで Sleep(16) のようにウェイトを入れると、Present の実行時間が 0[ms] となる。
        // 
        // このウェイトを無くすためには DXGI_PRESENT_DO_NOT_WAIT を使う。
        // ただしその場合ウェイトが必要な時は Present が DXGI_ERROR_WAS_STILL_DRAWING を返すようになる。
        // https://docs.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-present
        // https://docs.microsoft.com/en-us/windows/win32/direct3ddxgi/dxgi-present
        // 
        //ElapsedTimer t;
        swapChain->endFrame();
        //std::cout << t.elapsedMilliseconds() << "[ms]" << std::endl;
        //::Sleep(16);
    }
}

int main() {
    init();
    run();
    cleanup();
}
