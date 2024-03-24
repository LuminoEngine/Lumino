#include <LuminoCore.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Platform/PlatformModule.hpp>
#include <LuminoEngine/Platform/Platform.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/detail/GraphicsManager.hpp>
#include <LuminoEngine/GraphicsRHI/ShaderCompiler/detail/UnifiedShader.hpp>
#include <LuminoEngine/LuminoGraphics.hpp>
using namespace ln;

#define ASSETFILE(x) ln::Path(ASSETS_DIR, U##x)

void init() {
    RuntimeModule::initialize();
    PlatformModule::initialize({ { U"Example", 640, 480 }, WindowSystem::GLFWWithoutOpenGL });
    GraphicsModule::initialize({ GraphicsAPI::Vulkan });

    RuntimeModule::mountAssetDirectory(ASSETS_DIR);
}

void cleanup() {
    GraphicsModule::terminate();
    PlatformModule::terminate();
    RuntimeModule::terminate();
}

void run() {
    auto window = Platform::mainWindow();

    //auto shader = Shader::create(ASSETFILE("simple.hlsl"));
    auto shader = Shader::load(U"simple");
    auto descriptorLayout = shader->descriptorLayout();
    auto shaderPass = shader->techniques()[0]->passes()[0];

    auto shader2 = Shader::load(U"Sprite");
    auto descriptorLayout2 = shader2->descriptorLayout();
    auto shaderVariants2 = Array<std::string>{ "LN_USE_INSTANCING" };
    auto shaderPass2 = shader2->findTechniqueByVariantKey(kokage::VariantSet::calcHash(shaderVariants2), true)->passes()[0];

    auto vertexLayout = makeObject_deprecated<VertexLayout>();
    vertexLayout->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

    // CCW
    Vector4 v[] = {
        Vector4(0, 0.5, 0, 1),
        Vector4(-0.5, -0.25, 0, 1),
        Vector4(0.5, -0.25, 0, 1),
    };
    auto vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

    auto swapChain = makeObject_deprecated<GraphicsContext>(window);

    //auto spriteRenderer = makeURef<detail::SpriteRenderer>();
    //spriteRenderer->init();

    while (Platform::processEvents()) {
        auto commandList = swapChain->currentCommandList2();
        commandList->beginCommandRecoding();

        auto descriptor = commandList->allocateShaderDescriptor_deprecated(shaderPass);
        descriptor->setVector(descriptorLayout->findUniformMemberIndex(U"_Color"), Vector4(1, 0, 0, 1));

        auto renderPass = swapChain->currentRenderPass();
        commandList->beginRenderPass(renderPass);
        commandList->setVertexLayout(vertexLayout);
        commandList->setVertexBuffer(0, vertexBuffer);
        commandList->setShaderPass(shaderPass);
        commandList->setShaderDescriptor_deprecated(descriptor);
        commandList->setPrimitiveTopology(PrimitiveTopology::TriangleList);
        commandList->drawPrimitive(0, 1);

        commandList->setShaderDescriptor_deprecated(nullptr);
        commandList->setShaderPass(shaderPass2);
        //spriteRenderer->begin();
        //spriteRenderer->end();
        //spriteRenderer->render(commandList);
        commandList->endRenderPass();
        commandList->endCommandRecoding();

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
        swapChain->present();
        //std::cout << t.elapsedMilliseconds() << "[ms]" << std::endl;
        //::Sleep(16);
    }
}

int main() {
    init();
    run();
    cleanup();
}
