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

#include <LuminoGraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoGraphicsRHI/ShaderCompiler/detail/ShaderManager.hpp>
#include "../../src/Vulkan/VulkanDeviceContext.hpp"
#include "../../src/ShaderCompiler/UnifiedShaderCompiler.hpp"
using namespace ln;

struct ConstantParams {
    Color _Color;
};

SizeI g_viewSize;
Ref<PlatformWindow> g_window;
Ref<detail::IGraphicsDevice> g_device;
Ref<detail::ISwapChain> g_swapChain;
std::vector<Ref<detail::ICommandList>> g_commandLists;
std::vector<Ref<detail::RHIResource>> g_depthBuffers;
std::vector<Ref<detail::IRenderPass>> g_renderPasses;
std::vector<Ref<detail::RHIResource>> g_uniformBuffers;
std::vector<Ref<detail::IDescriptorPool>> g_descriptorPools;
Ref<detail::IVertexDeclaration> g_vertexDeclaration;
Ref<detail::RHIResource> g_vertexBuffer;
Ref<detail::IShaderPass> g_shaderPass;
// Ref<detail::IPipeline> g_renderPipeline;
int g_frameIndex;

void init() {
    RuntimeModule::initialize();
    PlatformModule::initialize({ { U"Example", 640, 480 }, WindowSystem::Native });
    RuntimeModule::mountAssetDirectory(ASSETS_DIR);
    detail::ShaderManager::initialize({});

    auto window = Platform::mainWindow();

    if (1) {
        detail::WebGPUDevice::Settings settings;
        settings.debugMode = true;
        auto device = makeRef<detail::WebGPUDevice>();
        device->init(settings);
        g_device = device;
    }
    else {
        detail::VulkanDevice::Settings settings;
        settings.mainWindow = window;
        settings.debugMode = true;
        bool dummy = false;
        auto device = makeRef<detail::VulkanDevice>();
        device->init(settings, &dummy);
        device->refreshCaps();
        g_device = device;
    }

    window->getSize(&g_viewSize);
    g_swapChain = g_device->createSwapChain(window, g_viewSize);

    ByteBuffer buffer = FileSystem::readAllBytes(Path(ASSETS_DIR, U"simple.hlsl")).unwrap();

    auto diag = makeObject<DiagnosticsManager>().unwrap();
    kokage::UnifiedShaderCompiler compiler(detail::ShaderManager::instance(), diag);
    List<Path> includeDirs;
    List<String> definitions;
    compiler.compile(reinterpret_cast<char*>(buffer.data()), buffer.size(), includeDirs, definitions);
    compiler.link();
    kokage::UnifiedShader* shader = compiler.unifiedShader();
    kokage::UnifiedShadePassId passId = shader->techniques()[0]->passes[0];
    kokage::UnifiedShaderPass* shaderPass = shader->pass(passId);
    // shaderPass->descriptorLayout;

    g_shaderPass = g_device->createShaderPassFromUnifiedShaderPass(shader, passId, "simple", diag);

    auto slotIndex = shaderPass->descriptorLayout.findUniformBufferRegisterIndex("ConstBuff");
    auto& item = shaderPass->descriptorLayout.bufferSlots()[slotIndex.i];

    assert(item.size == sizeof(ConstantParams));

    for (int i = 0; i < g_swapChain->getBackbufferCount(); i++) {
        Ref<detail::RHIResource> depthBuffer = g_device->createDepthBuffer(g_viewSize.width, g_viewSize.height);
        g_depthBuffers.push_back(depthBuffer);

        detail::DeviceFramebufferState info;
        info.renderTargets[0] = g_swapChain->getRenderTarget(i);
        info.depthBuffer = depthBuffer;
        Ref<detail::IRenderPass> renderPass = g_device->createRenderPass(info, ClearFlags::All, Color::parse(U"#98d98e"), 1.0, 0);
        g_renderPasses.push_back(renderPass);

        Ref<detail::ICommandList> commandList = g_device->createCommandList();
        g_commandLists.push_back(commandList);

        Ref<detail::RHIResource> uniformBuffer = g_device->createUniformBuffer(item.size);
        g_uniformBuffers.push_back(uniformBuffer);

        Ref<detail::IDescriptorPool> descriptorPool = g_device->createDescriptorPool(g_shaderPass);
        g_descriptorPools.push_back(descriptorPool);
    }

    // detail::DevicePipelineStateDesc pipelineStateDesc;
    //   pipelineStateDesc.shaderPass = g_shaderPass;
    //   pipelineStateDesc.renderPass = g_renderPasses[0];
    //   g_renderPipeline = g_device->createPipeline(pipelineStateDesc);

    VertexElement vertexAttributes[] = {
        { 0, VertexElementType::Float4, VertexElementUsage::Position, 0 },
    };
    g_vertexDeclaration = g_device->createVertexDeclaration(vertexAttributes, 1);

    // CCW
    Vector4 v[] = {
        Vector4(0, 0.5, 0.5, 1),
        Vector4(-0.5, -0.25, 0.5, 1),
        Vector4(0.5, -0.25, 0.5, 1),
    };
    g_vertexBuffer = g_device->createVertexBuffer(GraphicsResourceUsage::Static, sizeof(v), v);

    g_frameIndex = 0;
}

// void initApp() {
//     g_window = Platform::mainWindow();
//
//     g_swapChain = makeObject_deprecated<SwapChain>(g_window);
//
//     g_shader = Shader::load(U"simple");
//
//     g_vertexLayout = makeObject_deprecated<VertexLayout>();
//     g_vertexLayout->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);
//
//     // CCW
//     Vector4 v[] = {
//         Vector4(0, 0.5, 0, 1),
//         Vector4(-0.5, -0.25, 0, 1),
//         Vector4(0.5, -0.25, 0, 1),
//     };
//     g_vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);
// }
//
// void cleanupApp() {
//     g_vertexBuffer = nullptr;
//     g_vertexLayout = nullptr;
//     g_shader = nullptr;
//     g_swapChain = nullptr;
//     g_window = nullptr;
// }

void cleanup() {
    g_vertexBuffer->dispose();
    g_vertexBuffer = nullptr;

    g_vertexDeclaration->dispose();
    g_vertexDeclaration = nullptr;

    for (const Ref<detail::IDescriptorPool>& i : g_descriptorPools) {
        i->dispose();
    }
    g_descriptorPools.clear();

    for (const Ref<detail::RHIResource>& i : g_uniformBuffers) {
        i->dispose();
    }
    g_uniformBuffers.clear();

    for (const Ref<detail::ICommandList>& i : g_commandLists) {
        i->dispose();
    }
    g_commandLists.clear();

    for (const Ref<detail::IRenderPass>& i : g_renderPasses) {
        i->dispose();
    }
    g_renderPasses.clear();

    for (const Ref<detail::RHIResource>& i : g_depthBuffers) {
        i->dispose();
    }
    g_depthBuffers.clear();

    g_shaderPass->dispose();
    g_shaderPass = nullptr;

    g_swapChain->dispose();
    g_swapChain = nullptr;

    g_device->dispose();
    g_device = nullptr;

    detail::ShaderManager::terminate();
    PlatformModule::terminate();
    RuntimeModule::terminate();
}

void mainLoop() {
    Platform::processEvents();

    int imageIndex = 0;
    g_swapChain->acquireNextImage(&imageIndex);

    detail::ICommandList* commandList = g_commandLists[imageIndex];
    commandList->wait();

    ConstantParams params;
    params._Color = Color::Red;
    detail::RHIResource* uniformBuffer = g_uniformBuffers[imageIndex];
    void* data = uniformBuffer->map();
    memcpy(data, &params, sizeof(ConstantParams));
    uniformBuffer->unmap();

    detail::IDescriptorPool* descriptorPool = g_descriptorPools[imageIndex];
    descriptorPool->reset();
    detail::IDescriptor* descriptor = nullptr;
    descriptorPool->allocate(&descriptor);
    detail::ShaderDescriptorTableUpdateInfo info;
    info.uniforms[0] = {
        uniformBuffer,
        nullptr,
        0,
    };
    descriptor->setData(info);

    commandList->begin();
    commandList->beginRenderPass(g_renderPasses[imageIndex]);
    commandList->setViewportRect(RectI(0, 0, g_viewSize));
    commandList->setScissorRect(RectI(0, 0, g_viewSize));

    // RasterizerStateDesc rasterizerState;
    //   rasterizerState.cullMode = CullMode::None;
    //   commandList->setRasterizerState(rasterizerState);

    DepthStencilStateDesc depthStencilState;
    depthStencilState.depthTestFunc = ComparisonFunc::Always;
    commandList->setDepthStencilState(depthStencilState);

    commandList->setVertexDeclaration(g_vertexDeclaration);
    commandList->setVertexBuffer(0, g_vertexBuffer);
    commandList->setDescriptor(descriptor);
    commandList->setShaderPass(g_shaderPass);
    commandList->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    commandList->drawPrimitive(0, 1, 1);
    commandList->endRenderPass(g_renderPasses[imageIndex]);
    commandList->end();

    g_device->submitCommandBuffer(commandList, g_swapChain->getRenderTarget(imageIndex));
    g_swapChain->present();

    // auto descriptorLayout = g_shader->descriptorLayout();
    // auto shaderPass = g_shader->techniques()[0]->passes()[0];

    // auto ctx = g_swapChain->currentCommandList2();
    // ctx->beginCommandRecoding();
    // auto descriptor = ctx->allocateShaderDescriptor_deprecated(shaderPass);
    // descriptor->setVector(descriptorLayout->findUniformMemberIndex(U"_Color"), Vector4(1, 0, 0, 1));
    // auto renderPass = g_swapChain->currentRenderPass();
    // ctx->beginRenderPass(renderPass);
    // ctx->clear(ClearFlags::All, Color::Azure);
    // ctx->setVertexLayout(g_vertexLayout);
    // ctx->setVertexBuffer(0, g_vertexBuffer);
    // ctx->setShaderPass(shaderPass);
    // ctx->setShaderDescriptor_deprecated(descriptor);
    // ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    // ctx->drawPrimitive(0, 1);
    // ctx->endRenderPass();
    // ctx->endCommandRecoding();

    // g_swapChain->present();

    g_frameIndex = (g_frameIndex + 1) % g_swapChain->getBackbufferCount();
}

int main() {
    init();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 60, true);
#else
    while (!Platform::shouldQuit()) {
        mainLoop();
        Thread::sleep(16);
    }
#endif
    cleanup();
    return 0;
}
