#include "Common.hpp"
#include <LuminoEngine/GPU/RenderPass.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawCommand.hpp>
#include <LuminoEngine/Rendering/Kanata/KPipelineState.hpp>
#include <LuminoEngine/Rendering/Kanata/KUnlitRenderPass.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KPrimitiveMeshRenderer.hpp>
#include <LuminoEngine/Engine/detail/EngineDomain.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

#if 0
//==============================================================================
//
class Test_Rendering_LowLevels : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//
TEST_F(Test_Rendering_LowLevels, Basic) {

    URef<kanata::BatchCollector> g_batchList;
    URef<kanata::DrawCommandList> g_drawCommandList;
    URef<kanata::UnlitRenderPass> g_renderPass;
    URef<kanata::BoxMeshBatchProxy> g_boxMeshBatchProxy;
    Ref<VertexBuffer> g_vertexBuffer;

    auto* renderingManager = detail::EngineDomain::renderingManager();
    g_batchList = makeURef<kanata::BatchCollector>(renderingManager);
    g_drawCommandList = makeURef<kanata::DrawCommandList>(renderingManager);
    g_renderPass = makeURef<kanata::UnlitRenderPass>(renderingManager);
    g_boxMeshBatchProxy = makeURef<kanata::BoxMeshBatchProxy>();

    Vertex v[] = {
        // CCW
        Vertex(Vector3(0, 0.5, 0), Vector3(0, 0, 1), Vector2(0, 0), Color::Red),
        Vertex(Vector3(-0.5, -0.25, 0), Vector3(0, 0, 1), Vector2(1, 0), Color::Red),
        Vertex(Vector3(0.5, -0.25, 0), Vector3(0, 0, 1), Vector2(0, 1), Color::Red),
    };
    g_vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

    
    
    auto* ctx = TestEnv::beginFrame();
    auto* target = TestEnv::mainWindowSwapChain()->currentBackbuffer();
    auto* renderPass = TestEnv::renderPass();

    detail::RenderViewInfo renderViewInfo;
    renderViewInfo.cameraInfo.makePerspective(Vector3(10, 10, 10), Vector3::normalize(-1, -1, -1), 0.3, Size(renderPass->width(), renderPass->height()), 0.1, 1000.0);
    detail::SceneInfo sceneInfo;

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
        batch->vertexLayout = renderingManager->standardVertexDeclaration();
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

    ctx->beginRenderPass(renderPass);
    ctx->clear(ClearFlags::All, Color::White);

    // Render commands
    {
        g_drawCommandList->clear();
        g_renderPass->buildDrawCommands(g_batchList, ctx, renderViewInfo, sceneInfo, g_drawCommandList);
        g_drawCommandList->submitMeshDrawCommands(ctx);
    }

    ctx->endRenderPass();
    TestEnv::endFrame();
    ASSERT_RENDERTARGET(LN_ASSETFILE("Kanata/Expects/Test_Kanata.Basic.png"), target);
}
#endif
