#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <LuminoCore.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoGraphics/GPU/Shader.hpp>
#include <LuminoGraphics/GPU/ShaderDescriptor.hpp>
#include <LuminoGraphics/GPU/VertexLayout.hpp>
#include <LuminoGraphics/GPU/VertexBuffer.hpp>
#include <LuminoGraphics/GPU/SwapChain.hpp>
#include <LuminoGraphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/RHIModule.hpp>
using namespace ln;

Ref<PlatformWindow> g_window;
Ref<Shader> g_shader;
Ref<VertexLayout> g_vertexLayout;
Ref<VertexBuffer> g_vertexBuffer;
Ref<SwapChain> g_swapChain;

void init() {
    RuntimeModule::initialize();

    PlatformModule::initialize({ { U"Example", 640, 480 }, WindowSystem::GLFWWithOpenGL });

    RHIModule::initialize({ GraphicsAPI::OpenGL });

#ifdef __EMSCRIPTEN__
    RuntimeModule::mountAssetArchive(U"Assets.lna", StringView());
#else
    RuntimeModule::mountAssetDirectory(ASSETS_DIR);
#endif
}

void initApp() {
    g_window = Platform::mainWindow();

    g_swapChain = makeObject_deprecated<SwapChain>(g_window);

    g_shader = Shader::load(U"simple");

    g_vertexLayout = makeObject_deprecated<VertexLayout>();
    g_vertexLayout->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

    // CCW
    Vector4 v[] = {
        Vector4(0, 0.5, 0, 1),
        Vector4(-0.5, -0.25, 0, 1),
        Vector4(0.5, -0.25, 0, 1),
    };
    g_vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);
}

void cleanupApp() {
    g_vertexBuffer = nullptr;
    g_vertexLayout = nullptr;
    g_shader = nullptr;
    g_swapChain = nullptr;
    g_window = nullptr;
}

void cleanup() {
    RHIModule::terminate();
    PlatformModule::terminate();
    RuntimeModule::terminate();
}

void mainLoop() {
    Platform::processEvents();
    
    auto descriptorLayout = g_shader->descriptorLayout();
    auto shaderPass = g_shader->techniques()[0]->passes()[0];

    auto ctx = g_swapChain->currentCommandList2();
    ctx->beginCommandRecoding();
    auto descriptor = ctx->allocateShaderDescriptor_deprecated(shaderPass);
    descriptor->setVector(descriptorLayout->findUniformMemberIndex(U"_Color"), Vector4(1, 0, 0, 1));
    auto renderPass = g_swapChain->currentRenderPass();
    ctx->beginRenderPass(renderPass);
    ctx->clear(ClearFlags::All, Color::Azure);
    ctx->setVertexLayout(g_vertexLayout);
    ctx->setVertexBuffer(0, g_vertexBuffer);
    ctx->setShaderPass(shaderPass);
    ctx->setShaderDescriptor_deprecated(descriptor);
    ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    ctx->drawPrimitive(0, 1);
    ctx->endRenderPass();
    ctx->endCommandRecoding();

    g_swapChain->present();
}

int main() {
    init();
    initApp();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 60, true);
#else
    while (!Platform::shouldQuit()) {
        mainLoop();
        Thread::sleep(16);
    }
#endif
    cleanupApp();
    cleanup();
    return 0;
}
