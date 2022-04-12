#include <LuminoCore.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoGraphics/RHI/Shader.hpp>
#include <LuminoGraphics/RHI/ShaderDescriptor.hpp>
#include <LuminoGraphics/RHI/VertexLayout.hpp>
#include <LuminoGraphics/RHI/VertexBuffer.hpp>
#include <LuminoGraphics/RHI/SwapChain.hpp>
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/RHIModule.hpp>
#include <LuminoGraphics/Rendering/Vertex.hpp>
#include "../../src/Rendering/RenderingManager.hpp"
#include "../../../Graphics/src/GraphicsManager.hpp"


#include "../../src/Rendering/RenderingPipeline.hpp"
#include "../../src/Rendering/RenderElement.hpp"
#include "../../src/Rendering/CommandListServer.hpp"
#include "../../src/Rendering/RenderingProfiler.hpp"
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawCommand.hpp>
#include <LuminoEngine/Rendering/Kanata/KPipelineState.hpp>
#include <LuminoEngine/Rendering/Kanata/KUnlitRenderPass.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/KPrimitiveMeshRenderer.hpp>
#include <LuminoEngine/Rendering/Kanata/KRenderView.hpp>
using namespace ln;

Ref<PlatformWindow> g_window;
Ref<SwapChain> g_swapChain;

class MyRenderingContext : public RenderingContext {
public:
    MyRenderingContext() {}

    void resetForBeginRendering() {
        RenderingContext::resetForBeginRendering();
    }
};

class MyRenderView : public RenderView {
public:
    MyRenderView() {
        m_renderingContext = makeRef<MyRenderingContext>();
        m_sceneRenderingPipeline = makeRef<detail::FlatRenderingPipeline>();
        m_sceneRenderingPipeline->init();
    }

    void render(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget) override {
        {
            detail::RenderingManager::instance()->profiler()->reset();

            detail::RenderViewInfo renderViewInfo;
            // renderViewInfo.cameraInfo.makeUnproject(Size(640, 480));
            renderViewInfo.cameraInfo.makePerspective(Vector3(10, 10, 10), Vector3::normalize(-1, -1, -1), 0.3, Size(640, 480), 0.1, 1000.0);

            detail::SceneInfo sceneInfo;
            
            makeViewProjections(renderViewInfo.cameraInfo, 1.0); // TODO: dpiscale
            m_renderingContext->resetForBeginRendering();
        }

        // draw
        {
            m_renderingContext->drawBox(Box(1), Color::Red);
        }

        ClearInfo clearInfo;
        clearInfo.color = backgroundColor();
        clearInfo.depth = 1.0f;
        clearInfo.stencil = 0x00;
        if (clearMode() == SceneClearMode::ColorAndDepth) {
            clearInfo.flags = ClearFlags::All;
        }
        else {
            clearInfo.flags = ClearFlags::Depth | ClearFlags::Stencil;
        }
        
        m_sceneRenderingPipeline->render(
            graphicsContext,
            m_renderingContext,
            renderTarget,
            clearInfo,
            this,
            m_renderingContext->commandList()->elementList(),
            m_renderingContext->commandListServer());
    }

    Ref<MyRenderingContext> m_renderingContext;
    Ref<detail::FlatRenderingPipeline> m_sceneRenderingPipeline;
};

Ref<MyRenderView> g_renderView;

void init() {
    RuntimeModule::initialize();
    PlatformModule::initialize({ { U"Example", 640, 480 } });
    RHIModule::initialize({ GraphicsAPI::Vulkan, U"", true });
    RuntimeModule::mountAssetDirectory(ASSETS_DIR);

    detail::RenderingManager::Settings renderingManagerSettings;
    renderingManagerSettings.graphicsManager = detail::GraphicsManager::instance();
    renderingManagerSettings.fontManager = nullptr;
    detail::RenderingManager::initialize(renderingManagerSettings);
}

void initApp() {
    g_window = Platform::mainWindow();
    g_swapChain = makeObject<SwapChain>(g_window);
    g_renderView = makeObject<MyRenderView>();
}

void cleanupApp() {
    g_renderView = nullptr;
    g_swapChain = nullptr;
    g_window = nullptr;
}

void cleanup() {
    RHIModule::terminate();
    PlatformModule::terminate();
    RuntimeModule::terminate();
}

void render(GraphicsCommandList* commandList) {
    g_renderView->render(commandList, g_swapChain->currentBackbuffer());
}

void mainLoop() {
    Platform::processEvents();

    auto commandList = g_swapChain->beginFrame2();
    render(commandList);
    g_swapChain->endFrame();

    Thread::sleep(10);
}

int main() {
    init();
    initApp();
    while (!Platform::shouldQuit()) {
        mainLoop();
        Thread::sleep(16);
    }
    cleanupApp();
    cleanup();
    return 0;
}

#if 0

Ref<detail::RenderingManager> g_renderingManager;
Ref<PlatformWindow> g_window;
Ref<SwapChain> g_swapChain;

URef<kanata::BatchCollector> g_batchList;
URef<kanata::DrawCommandList> g_drawCommandList;
URef<kanata::UnlitRenderPass> g_renderPass;
URef<kanata::BoxMeshBatchProxy> g_boxMeshBatchProxy;
Ref<Shader> g_shader;
Ref<VertexBuffer> g_vertexBuffer;

void init() {
    RuntimeModule::initialize();
    PlatformModule::initialize({ { U"Example", 640, 480 } });
    RHIModule::initialize({ GraphicsAPI::Vulkan, U"", true });
    RuntimeModule::mountAssetDirectory(ASSETS_DIR);

    detail::RenderingManager::Settings renderingManagerSettings;
    renderingManagerSettings.graphicsManager = detail::GraphicsManager::instance();
    renderingManagerSettings.fontManager = nullptr;
    g_renderingManager = makeRef<detail::RenderingManager>();
    g_renderingManager->init(renderingManagerSettings);
}

void initApp() {
    g_window = Platform::mainWindow();
    g_swapChain = makeObject<SwapChain>(g_window);

    g_batchList = makeURef<kanata::BatchCollector>(g_renderingManager);
    g_drawCommandList = makeURef<kanata::DrawCommandList>(g_renderingManager);
    g_renderPass = makeURef<kanata::UnlitRenderPass>(g_renderingManager);
    g_boxMeshBatchProxy = makeURef<kanata::BoxMeshBatchProxy>();

    g_shader = Shader::load(U"simple");
    Vertex v[] = { // CCW
        Vertex(Vector3(0, 0.5, 0), Vector3(0, 0, 1), Vector2(0, 0), Color::Red),
        Vertex(Vector3(-0.5, -0.25, 0), Vector3(0, 0, 1), Vector2(1, 0), Color::Red),
        Vertex(Vector3(0.5, -0.25, 0), Vector3(0, 0, 1), Vector2(0, 1), Color::Red),
    };
    g_vertexBuffer = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);
}

void cleanupApp() {
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

    
    std::cout << "==========" << std::endl;
    ElapsedTimer timer;


    detail::RenderViewInfo renderViewInfo;
    //renderViewInfo.cameraInfo.makeUnproject(Size(640, 480));
    renderViewInfo.cameraInfo.makePerspective(Vector3(10, 10, 10), Vector3::normalize(-1, -1, -1), 0.3, Size(640, 480), 0.1, 1000.0);

    detail::SceneInfo sceneInfo;

    auto descriptorLayout = g_shader->descriptorLayout();
    auto shaderPass = g_shader->techniques()[0]->passes()[0];

    auto ctx = g_swapChain->beginFrame2();

    // Build batch
    {
        g_batchList->clear();

        // 手動で頑張るパターン
        auto* batch = g_batchList->newBatch<kanata::Batch>();
        kanata::BatchElement batchElement;
        batchElement.vertexBuffers = {};
        batchElement.vertexBuffers[0] = g_vertexBuffer;
        batchElement.indexBuffer = nullptr;
        batchElement.firstIndex = 0;
        batchElement.firstVertex = 0;
        batchElement.primitiveCount = 1;
        batchElement.instanceCount = 0;
        batch->elemets.push(batchElement);
        batch->vertexLayout = g_renderingManager->standardVertexDeclaration();
        batch->primitiveTopology = PrimitiveTopology::TriangleList;

        // Proxy を使い、Static な Mesh を描画するパターン
        g_boxMeshBatchProxy->getBatch(g_batchList);

        // 
        auto& r = g_batchList->primitiveRenderer();
        r->begin();
        r->drawBox(Box(0.5), Color::Red, Matrix::makeTranslation(2, 0, 0));
        r->drawBox(Box(2), Color::Red, Matrix::makeTranslation(-2, 0, 0));
        r->end();
    }


    auto buildBatchTime = timer.elapsedMilliseconds();


    g_drawCommandList->clear();
    g_renderPass->buildDrawCommands(g_batchList, ctx, renderViewInfo, sceneInfo, g_drawCommandList);

    auto renderPass = g_swapChain->currentRenderPass();
    ctx->beginRenderPass(renderPass);
    ctx->clear(ClearFlags::All, Color::Azure);

    g_drawCommandList->submitMeshDrawCommands(ctx);
    //
    //
    //
    //ctx->setVertexLayout(g_vertexLayout);
    //ctx->setVertexBuffer(0, g_vertexBuffer);
    //ctx->setShaderPass(shaderPass);
    //ctx->setShaderDescriptor(descriptor);
    //ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    //ctx->drawPrimitive(0, 1);

    ctx->endRenderPass();
    g_swapChain->endFrame();

    std::cout << "BuildBatch : " << buildBatchTime << " [ms]" << std::endl;
    std::cout << "Total      : " << timer.elapsedMilliseconds() << " [ms]" << std::endl;

    Thread::sleep(10);
}

int main() {
    init();
    initApp();
    while (!Platform::shouldQuit()) {
        mainLoop();
        Thread::sleep(16);
    }
    cleanupApp();
    cleanup();
    return 0;
}
#endif