#include <LuminoCore.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include <LuminoGraphics/Shader.hpp>
#include <LuminoGraphics/ShaderDescriptor.hpp>
#include <LuminoGraphics/VertexLayout.hpp>
#include <LuminoGraphics/VertexBuffer.hpp>
#include <LuminoGraphics/SwapChain.hpp>
#include <LuminoGraphics/GraphicsCommandBuffer.hpp>
#include "../src/GraphicsManager.hpp"
using namespace ln;

#define ASSETFILE(x) ln::Path(ASSETS_DIR, U##x)

void init() {
    RuntimeModule::initialize();
    PlatformModule::initialize({ { U"Example", 640, 480 }, WindowSystem::GLFWWithOpenGL });
    RHIModule::initialize({ GraphicsAPI::OpenGL });

    RuntimeModule::mountAssetDirectory(ASSETS_DIR);
}

void cleanup() {
    RHIModule::terminate();
    PlatformModule::terminate();
    RuntimeModule::terminate();
}

void run() {
    auto window = Platform::mainWindow();

    //auto shader = Shader::create(ASSETFILE("simple.hlsl"));
    auto shader = Shader::load(U"simple");

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

    auto swapChain = makeObject<SwapChain>(window);

    while (Platform::processEvents()) {
        auto commandList = swapChain->beginFrame2();

        auto descriptor = commandList->allocateShaderDescriptor(shaderPass);
        descriptor->setVector(descriptorLayout->findUniformMemberIndex(U"_Color"), Vector4(1, 0, 0, 1));

        auto renderPass = swapChain->currentRenderPass();
        commandList->beginRenderPass(renderPass);
        commandList->setVertexLayout(vertexLayout);
        commandList->setVertexBuffer(0, vertexBuffer);
        commandList->setShaderPass(shaderPass);
        commandList->setShaderDescriptor(descriptor);
        commandList->setPrimitiveTopology(PrimitiveTopology::TriangleList);
        commandList->drawPrimitive(0, 1);
        commandList->endRenderPass();

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
