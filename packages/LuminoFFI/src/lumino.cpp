#include <stdio.h>
#include <LuminoEngine/RuntimeModule.hpp>
#include <LuminoEngine/RHIModule.hpp>
#include <LuminoEngine/GPU/VertexBuffer.hpp>
#include <LuminoEngine/GPU/VertexLayout.hpp>
#include <LuminoEngine/GPU/RenderPass.hpp>
#include <LuminoEngine/GPU/OpenGLGraphicsContext.hpp>
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Font/detail/FontManager.hpp>
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <lumino.h>


#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawCommand.hpp>
#include <LuminoEngine/Rendering/Kanata/KPipelineState.hpp>
#include <LuminoEngine/Rendering/Kanata/KUnlitRenderPass.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxyCollector.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KPrimitiveMeshRenderer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/SpriteRenderer.hpp>

namespace ln {

class FFI {
public:
    static LNResult processException(Exception* e);
    static LNHandle wrapObject(Object* obj, bool fromCreate);
    static Object* getObject(LNHandle handle);
    //static void setManagedObjectId(LNHandle handle, int64_t id);
    //static int64_t getManagedObjectId(LNHandle handle);
    //static const Char* getUTF16StringPtr(const String& str);
    //static const char* getAStringPtr(const String& str);
    //static void setAStringEncoding(TextEncoding* value);
    //static TextEncoding* getAStringEncoding();

private:
};

LNResult FFI::processException(Exception* e) {
    return LN_ERROR_UNKNOWN;
}

LNHandle FFI::wrapObject(Object* obj, bool fromCreate) {
    return detail::RuntimeManager::instance()->makeObjectWrap(obj, fromCreate);
}

Object* FFI::getObject(LNHandle handle) {
    return detail::RuntimeManager::instance()->getObjectEntry(handle)->object;
}

} // namespace ln







#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define LN_FFI_TRY_BEGIN try {

#define LN_FFI_TRY_END_RETURN               \
    }                                         \
    catch (ln::Exception & e) {               \
        return ln::FFI::processException(&e); \
    }                                         \
    return LN_OK;

#define LNI_HANDLE_TO_OBJECT(type, h) static_cast<type*>((h) ? ::ln::FFI::getObject(h) : nullptr)

LUMINO_API LNResult LNRuntime_Initialize() {
    ln::RuntimeModule::initialize();
    ln::detail::RuntimeManager::initialize(ln::detail::RuntimeManager::Settings());
    ln::GraphicsModule::initialize({ ln::GraphicsAPI::OpenGL });

    {
        ln::detail::FontManager::Settings settings;
        settings.assetManager = ln::detail::AssetManager::instance();
        ln::detail::FontManager::initialize(settings);
    }

    {
        ln::detail::RenderingManager::Settings settings;
        settings.graphicsManager = ln::detail::GraphicsManager::instance();
        settings.fontManager = ln::detail::FontManager::instance();
        printf("RenderingManager 111 %p %p\n", settings.graphicsManager, settings.fontManager);
        ln::detail::RenderingManager::initialize(settings);
    }
    return LN_OK;
}

LUMINO_API void LNRuntime_Terminate() {
    ln::detail::RenderingManager::terminate();
    ln::detail::FontManager::terminate();
    ln::GraphicsModule::terminate();
    ln::detail::RuntimeManager::terminate();
    ln::RuntimeModule::terminate();
}

LUMINO_API LNResult LNGraphicsContext_CreateFromCurrentOpenGLContext(int32_t width, int32_t height, LNHandle* outReturn) {
    LN_FFI_TRY_BEGIN;
    ln::OpenGLGraphicsContext::Settings s;
    s.window = nullptr;
    s.width = width;
    s.height = height;
    *outReturn = ::ln::FFI::wrapObject(ln::OpenGLGraphicsContext::create(s), true);
    LN_FFI_TRY_END_RETURN;
}

LUMINO_API LNResult LNGraphicsContext_BeginFrame(LNHandle graphicsContext, int32_t width, int32_t height) {
    LN_FFI_TRY_BEGIN;
    ln::GraphicsContext* context = LNI_HANDLE_TO_OBJECT(ln::GraphicsContext, graphicsContext);
    ln::GraphicsCommandList* commandList = context->currentCommandList2();
    commandList->beginCommandRecoding();
    LN_FFI_TRY_END_RETURN;
}

LUMINO_API LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext) {
    LN_FFI_TRY_BEGIN;
    ln::GraphicsContext* context = LNI_HANDLE_TO_OBJECT(ln::GraphicsContext, graphicsContext);
    ln::GraphicsCommandList* commandList = context->currentCommandList2();
    commandList->endCommandRecoding();
    LN_FFI_TRY_END_RETURN;
}

LUMINO_API LNResult LNGraphicsContext_Present(LNHandle graphicsContext) {
    LN_FFI_TRY_BEGIN;
    LN_NOTIMPLEMENTED();
    LN_FFI_TRY_END_RETURN;
}

//LUMINO_API LNResult LNGraphicsContext_Release(LNHandle handle) {
//    LN_SAFE_RELEASE(handle)
//
//}

extern LUMINO_API LNResult LNRenderingContext_Create(LNHandle graphicsContext, LNHandle* outReturn) {
    LN_FFI_TRY_BEGIN;
    ln::GraphicsContext* context = LNI_HANDLE_TO_OBJECT(ln::GraphicsContext, graphicsContext);
    ln::Ref<ln::CommandList> renderingContext = ln::makeObject_deprecated<ln::CommandList>();
    *outReturn = ::ln::FFI::wrapObject(renderingContext, true);
    LN_FFI_TRY_END_RETURN;
}

extern LUMINO_API LNResult LNRenderingContext_Reset(LNHandle renderingContext_, LNHandle renderingViewPoint_, LNHandle graphicsContext_) {
    LN_FFI_TRY_BEGIN;
    ln::CommandList* renderingContext = LNI_HANDLE_TO_OBJECT(ln::CommandList, renderingContext_);
    ln::RenderViewPoint* renderingViewPoint = LNI_HANDLE_TO_OBJECT(ln::RenderViewPoint, renderingViewPoint_);

    renderingContext->clearCommandsAndState(renderingViewPoint);

    if (1) {

        ln::GraphicsContext* context = LNI_HANDLE_TO_OBJECT(ln::GraphicsContext, graphicsContext_);

        ln::RenderPass* renderPass = context->currentRenderPass();
        ln::GraphicsCommandList* commandList = context->currentCommandList2();

        ;

        renderPass->setClearFlags(ln::ClearFlags::Color);
        renderPass->setClearColor(ln::Color::Red);

        // commandList->beginCommandRecoding();
        // commandList->beginRenderPass(renderPass);
        // commandList->clear(ln::ClearFlags::Color, ln::Color::Red);
        // commandList->endRenderPass();
        // commandList->endCommandRecoding();

#if 1
        {
            using namespace ln;
            //URef<kanata::BatchCollector> g_batchList;
            URef<kanata::DrawCommandList> g_drawCommandList;
            Ref<kanata::UnlitRenderPass> g_renderPass;
            URef<kanata::BoxMeshBatchProxy> g_boxMeshBatchProxy;
            Ref<VertexBuffer> g_vertexBuffer;

            auto* renderingManager = ln::detail::RenderingManager::instance();
            //g_batchList = makeURef<kanata::BatchCollector>(renderingManager);
            kanata::BatchCollector* g_batchList = renderingContext->batchCollector();
            g_drawCommandList = makeURef<kanata::DrawCommandList>(renderingManager);
            g_renderPass = makeRef<kanata::UnlitRenderPass>(renderingManager);
            g_boxMeshBatchProxy = makeURef<kanata::BoxMeshBatchProxy>();

            Vertex v[] = {
                //Vertex(Vector3(0, 5.5, 0), Vector3(0, 0, 1), Vector2(0, 0), Color::Red),
                //Vertex(Vector3(-5.5, 0, 0), Vector3(0, 0, 1), Vector2(1, 0), Color::Red),
                //Vertex(Vector3(5.5, 0, 0), Vector3(0, 0, 1), Vector2(0, 1), Color::Red),

                Vertex(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector2(0, 0), Color::Red),
                Vertex(Vector3(0, 10, 0), Vector3(0, 0, 1), Vector2(0, 1), Color::Green),
                Vertex(Vector3(10, 0, 0), Vector3(0, 0, 1), Vector2(1, 0), Color::Blue),
            };
            g_vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

            Ref<Material> material = Material::create();
             //Ref<Texture2D> texture = Texture2D::load(U"C:/Proj/LN/Lumino/assets/Distributable/assets/icon256.png");
             //material->setMainTexture(texture);

            // auto* target = TestEnv::mainWindowSwapChain()->currentBackbuffer();
            // auto* renderPass = TestEnv::renderPass();

            detail::RenderViewInfo renderViewInfo;
            // renderViewInfo.cameraInfo.makePerspective(Vector3(10, 10, 10), Vector3::normalize(-1, -1, -1), 0.3, Size(renderPass->width(), renderPass->height()), 0.1, 1000.0);
            detail::SceneInfo sceneInfo;

            printf("size: %d %d\n", renderPass->width(), renderPass->height());

            //RenderViewPoint viewPoint;
            //viewPoint.resetPerspective(Vector3(10, 10, 10), Vector3::normalize(-1, -1, -1), 0.3, Size(renderPass->width(), renderPass->height()), 0.1, 1000.0);
            renderingViewPoint->makeCameraInfo(&renderViewInfo.cameraInfo);

            kanata::BatchProxyState batchState;
            batchState.reset();
            batchState.m_depthTestEnabled = false;
            batchState.m_cullingMode = CullMode::None;

            // Build batch
            {
                g_batchList->clear(renderingViewPoint);

                // 手動で頑張るパターン
                g_batchList->batchProxyState = &batchState;
                auto* batch = g_batchList->newBatch<kanata::Batch>(1, material);
                batch->elemets2[0].vertexBuffers = {};
                batch->elemets2[0].vertexBuffers[0] = g_vertexBuffer;
                batch->elemets2[0].indexBuffer = nullptr;
                batch->elemets2[0].firstIndex = 0;
                batch->elemets2[0].firstVertex = 0;
                batch->elemets2[0].primitiveCount = 1;
                batch->elemets2[0].instanceCount = 0;
                batch->vertexLayout = renderingManager->standardVertexDeclaration();
                batch->primitiveTopology = PrimitiveTopology::TriangleList;

                // Proxy を使い、Static な Mesh を描画するパターン
                // g_boxMeshBatchProxy->getBatch(g_batchList);

                //
                // auto& r = g_batchList->primitiveRenderer();
                // r->begin();
                // r->drawBox(Box(0.5), Color::Red, Matrix::makeTranslation(2, 0, 0));
                // r->drawBox(Box(2), Color::Red, Matrix::makeTranslation(-2, 0, 0));
                // r->end();
                
                auto& r = ln::detail::RenderingManager::instance()->spriteRenderer();
                r->begin(renderingContext, material);
                r->drawSprite(
                    Matrix::makeTranslation(0, 0, 0),
                    Size(100, 200),
                    Vector2(0, 0),
                    Rect(0, 0, 1, 1),
                    Color::Red,
                    SpriteBaseDirection::Basic2D,
                    BillboardType::None,
                    SpriteFlipFlags::None);
                r->end();
                
                
             auto& batchProxyCollector = renderingContext->batchProxyCollector();
                auto& batchCollector = renderingContext->batchCollector();

                batchProxyCollector->resolveSingleFrameBatchProxies(batchCollector);
            }

            commandList->beginCommandRecoding();
            commandList->beginRenderPass(renderPass);
            commandList->clear(ClearFlags::All, Color::Aqua);
            commandList->endRenderPass();

            // Render commands
            {
                g_drawCommandList->clear();
                g_renderPass->buildDrawCommands(
                    nullptr,
                    g_batchList,
                    commandList,
                    renderPass,
                    renderViewInfo,
                    sceneInfo,
                    g_drawCommandList);
                g_drawCommandList->submitMeshDrawCommands(commandList);
            }

            commandList->endCommandRecoding();
            /*
            NOTE: カリングとZソートについて
            カリングは出来るだけ早い段階で行った方が処理負荷は少なくなる。
            Three.js は Object(SceneNode) に対して行っている。PIXI.js も Sprite が boundBox 持っているようだ。

            Lumino としては BatchProxy に対して行うのがよいかもしれない。
            まず SceneGraph は Engine から独立させたいので、SceneNode は対象にならない。
            Tilemap を例に考えてみるとよいか。
            Tilemap は見える範囲の tile を動的に描画するが、この動的な生成処理は BatchProxy の担当。
            Batch は、そのカリングを通った Tile の集合となる。

            Siv3D のような動的で手続き的なドローイングをする場合、ひとつひとつの図形が Proxy となる。
            まず Proxy としておき、 Arc を描くときもすぐには頂点を生成せず、boundBox だけ Proxy が持つ。
            ※Proxy 化しなくても、RenderingContext::drawArc() のような関数の中ですぐに弾いてもいいかも。

            Tilemap や BillboardSprite(Particle) のように視点に依存して描画結果が変わるものを想定すると、
            getBatch() には始点情報を渡す必要がある。（既に BatchCollector::viewPoint() で渡している）

            Zソートはやるなら Proxy のリストに対して行う。
            なので流れとしては、

            1. RenderingContext::drawSprite() などで Proxy を生成する。
            2. Batch 化の前に、カリング & Zソートを実施する。

            ## 先に始点情報を作るか？
            RenderingContext::begin()/end() を設けて、この内側でしか draw 出来ないようにする必要がある。
            ただ、 draw 時点でカリングできるので効率は良い。
            一方でシャドウマップを作る場合などでは、全く同じ RenderingContext::begin()->draw()->end() を
            呼び出してもらう必要がある。特に RenderingContext を公開する場合。
            Three.js のように SceneGrap が一級市民なフレームワークでは、 RenderingContext が隠れるので良いだろう。
            でも Lumino は SceneGraph が Engine から独立しているので、RenderingContext は公開される。

            この Proxy の収取を担当しているのは BatchProxyCollector.
            このクラスは RenderingContext が既に持っている。

            */
        }
#endif
    }
    LN_FFI_TRY_END_RETURN;
}

extern LUMINO_API LNResult LNSceneRenderingViewPoint_Create(LNHandle* outRenderingViewPoint) {
    LN_FFI_TRY_BEGIN;
    ln::Ref<ln::RenderViewPoint> viewPoint = ln::makeRef<ln::RenderViewPoint>();
    *outRenderingViewPoint = ::ln::FFI::wrapObject(viewPoint, true);
    LN_FFI_TRY_END_RETURN;
}

extern LUMINO_API LNResult LNSceneRenderingViewPoint_SetupPerspective2D(LNHandle renderingViewPoint, float x, float y, float z, float width, float height, float nearZ, float farZ) {
    LN_FFI_TRY_BEGIN;
    ln::RenderViewPoint* viewPoint = LNI_HANDLE_TO_OBJECT(ln::RenderViewPoint, renderingViewPoint);
    viewPoint->resetPerspective2D(ln::Vector3(x,y, z), ln::Size(width, height), nearZ, farZ);
    LN_FFI_TRY_END_RETURN;
}

extern LUMINO_API LNResult LNUnlitSceneRenderingPass_Create(LNHandle* outUnlitSceneRenderingPass) {
    LN_FFI_TRY_BEGIN;
    ln::detail::RenderingManager* renderingManager = ln::detail::RenderingManager::instance();
    ln::Ref<ln::kanata::UnlitRenderPass> renderingPass = ln::makeRef<ln::kanata::UnlitRenderPass>(renderingManager);
    *outUnlitSceneRenderingPass = ::ln::FFI::wrapObject(renderingPass, true);
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
// LNObject

LUMINO_API LNResult LNObject_Release(LNHandle obj) {
    if (auto m = ln::detail::RuntimeManager::instance()) {
        m->releaseObjectExplicitly(obj);
        return LN_OK;
    }
    else {
        return LN_RUNTIME_UNINITIALIZED;
    }
}

LUMINO_API LNResult LNObject_Retain(LNHandle obj) {
    if (auto m = ln::detail::RuntimeManager::instance()) {
        m->retainObjectExplicitly(obj);
        return LN_OK;
    }
    else {
        return LN_RUNTIME_UNINITIALIZED;
    }
}

LUMINO_API LNResult LNObject_GetReferenceCount(LNHandle obj, int32_t* outReturn) {
    if (!outReturn) return LN_ERROR_INVALID_ARGUMENT;

    if (auto t = LNI_HANDLE_TO_OBJECT(ln::Object, obj)) {
        *outReturn = ln::RefObjectHelper::getReferenceCount(t);
        return LN_OK;
    }
    else {
        return LN_ERROR_INVALID_ARGUMENT;
    }
}



#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
