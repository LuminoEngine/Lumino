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
#include <LuminoGraphics/Rendering/Vertex.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>
#include <LuminoGraphics/detail/GraphicsManager.hpp>


#include <LuminoGraphics/Rendering/RenderingPipeline/FlatRenderingPipeline.hpp>
#include "../../src/Rendering/RenderElement.hpp"
#include "../../src/Rendering/CommandListServer.hpp"
#include "../../src/Rendering/RenderingProfiler.hpp"
#include <LuminoGraphics/Rendering/RenderView.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/CommandList.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/Kanata/KBatch.hpp>
#include <LuminoGraphics/Rendering/Kanata/KBatchList.hpp>
#include <LuminoGraphics/Rendering/Kanata/KDrawCommand.hpp>
#include <LuminoGraphics/Rendering/Kanata/KPipelineState.hpp>
#include <LuminoGraphics/Rendering/Kanata/KUnlitRenderPass.hpp>
#include <LuminoGraphics/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoGraphics/Rendering/Kanata/RenderFeature/KPrimitiveMeshRenderer.hpp>
#include <LuminoGraphics/Rendering/Kanata/KRenderView.hpp>
#include <LuminoGraphics/Rendering/FeatureRenderer/PrimitiveMeshRenderer.hpp>
using namespace ln;

Ref<PlatformWindow> g_window;
Ref<SwapChain> g_swapChain;

class MyRenderView : public RenderView {
public:
    MyRenderView() {
        m_sceneRenderingPipeline = makeObject_deprecated<FlatRenderingPipeline>();
    }

    void render(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget) override {
        renderPipeline(graphicsContext, renderTarget);
    }

    void onUpdateViewPoint(RenderViewPoint* viewPoint, RenderTargetTexture* renderTarget) override {
        viewPoint->resetPerspective(Vector3(10, 10, 10), Vector3::normalize(-1, -1, -1), 0.3, Size(640, 480), 0.1, 1000.0);
    }

    void onRender(GraphicsCommandList* graphicsContext, RenderingContext* renderingContext, RenderTargetTexture* renderTarget) override {
        {
            detail::RenderingManager::instance()->profiler()->reset();

            
        }

        // draw
        {
            PrimitiveMeshRenderer::drawBox(renderingContext, Material::defaultMaterial(), Box(1), Color::Red);
        }
        


        //m_sceneRenderingPipeline->render(
        //    graphicsContext,
        //    m_renderingContext,
        //    renderTarget,
        //    this,
        //    nullptr);
    }

    Ref<FlatRenderingPipeline> m_sceneRenderingPipeline;
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
    g_swapChain = makeObject_deprecated<SwapChain>(g_window);
    g_renderView = makeObject_deprecated<MyRenderView>();
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

    auto commandList = g_swapChain->currentCommandList2();
    commandList->beginCommandRecoding();
    render(commandList);
    commandList->endCommandRecoding();
    g_swapChain->present();

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
    g_swapChain = makeObject_deprecated<SwapChain>(g_window);

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
    g_vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);
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