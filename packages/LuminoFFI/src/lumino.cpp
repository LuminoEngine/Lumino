#include <stdio.h>
#include <LuminoEngine.hpp>

#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/FPSController.hpp>
#include <LuminoEngine/Graphics/Bitmap/BitmapRenderingContext.hpp>
#include <LuminoEngine/Graphics/GPU/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/VertexLayout.hpp>
#include <LuminoEngine/Graphics/GPU/RenderPass.hpp>
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Graphics/GPU/DepthBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/RHIIntegrations.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Graphics/Font/detail/FontManager.hpp>
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/SurfaceContext.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/DebugPrint.hpp>
#include <lumino.h>


#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawEvent.hpp>
#include <LuminoEngine/Rendering/Kanata/KPipelineState.hpp>
#include <LuminoEngine/Rendering/Kanata/KUnlitRenderPass.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxyCollector.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KPrimitiveMeshRenderer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchRenderer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/SpriteTextRenderer.hpp>
#include <LuminoEngine/Graphics/Font/Font.hpp>

namespace ln {

class FFI {
public:
    static LNResult processException(Exception* e);

private:
};

LNResult FFI::processException(Exception* e) {
    return LN_ERROR_UNKNOWN;
}

/**
 * Provides operations and utilities for the entire Runtime module.
 */
class Runtime {
public:
    static LNHandle wrapObject(Object* obj, bool fromCreate);
    static Object* getObject(LNHandle handle);
    //static void setManagedObjectId(LNHandle handle, int64_t id);
    //static int64_t getManagedObjectId(LNHandle handle);
    //static const Char* getUTF16StringPtr(const String& str);
    //static const char* getAStringPtr(const String& str);
    //static void setAStringEncoding(TextEncoding* value);
    //static TextEncoding* getAStringEncoding();
};

LNHandle Runtime::wrapObject(Object* obj, bool fromCreate) {
    auto* manager = ln::EngineInstance::instance()->runtimeManager().get();
    return manager->makeObjectWrap(obj, fromCreate);
}

Object* Runtime::getObject(LNHandle handle) {
    auto* manager = ln::EngineInstance::instance()->runtimeManager().get();
    return manager->getObjectEntry(handle)->object;
}
} // namespace ln



using namespace ln;



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define LN_FFI_TRY_BEGIN try {

#define LN_FFI_TRY_END_RETURN             \
    }                                     \
    catch (Exception & e) {               \
        return FFI::processException(&e); \
    }                                     \
    return LN_OK;

#define LN_FFI_TRY_END             \
    }                              \
    catch (Exception & e) {        \
        FFI::processException(&e); \
    }             

#define LN_HANDLE_TO_OBJECT(type, h) static_cast<type*>((h) ? ::Runtime::getObject(h) : nullptr)
#define LN_RELEASE_OBJECT(h) LNObject_Release(reinterpret_cast<LNHandle>(h))

#define TO_FFI_ERROR(result) LN_ERROR_UNKNOWN // TODO:

#define TRY_FFI_RESULT(expr)             \
    {                                    \
        auto result = (expr);            \
        if (!result) {                   \
            return TO_FFI_ERROR(result); \
        }                                \
    }

// #define LN_DEFINE_HANDLE(object) typedef struct object##_T* object
// NOTE: ↑こういうタイプセーフなハンドル定義は行わない。
//   - 継承されたクラスを使いづらくなるため。
//     例えば LNTexture2D を LNTextre として LNMaterial にセットしたいときなど。
//     キャストして使ってもよいが、そうすると LNHandle のまま使うのと大差なくなるかも。

class FFIRenderPass : public ln::Object {
public:
    SurfaceContext* owner;
    Ref<kanata::UnlitRenderPass> sceneRenderPass;
    RenderPass* renderPass;
};

//==============================================================================

//LNStructHandle LNMatrix_New() {
//    Matrix* m = new Matrix();
//    return reinterpret_cast<LNStructHandle>(m);
//}
//
//void LNMatrix_Delete(LNStructHandle matrix) {
//    delete reinterpret_cast<Matrix*>(matrix);
//}

void LNMatrix_SetIdentity(LNMatrix* outResult) {
    LN_FFI_TRY_BEGIN;
    Matrix* m = reinterpret_cast<Matrix*>(outResult);
    *m = Matrix::Identity;
    LN_FFI_TRY_END;
}

//==============================================================================
LNStructHandle LNRenderPassDescriptor_Get() {
    static LNRenderPassDescriptor s;
    memset(&s, 0, sizeof(s));
    return reinterpret_cast<LNStructHandle>(&s);
}

void LNRenderPassDescriptor_SetRenderTarget(
    LNStructHandle handle,
    int32_t index,
    LNHandle renderTarget,
    float clearR,
    float clearG,
    float clearB,
    float clearA,
    LNBool clearEnable) {
    //std::cout << "handle: " << handle << std::endl;
    //std::cout << "index: " << index << std::endl;
    //std::cout << "renderTarget: " << renderTarget << std::endl;
    //std::cout << "clearR: " << clearR << std::endl;
    //std::cout << "clearG: " << clearG << std::endl;
    //std::cout << "clearB: " << clearB << std::endl;
    //std::cout << "clearA: " << clearA << std::endl;
    //std::cout << "clearEnable: " << clearEnable << std::endl;
    LNRenderPassDescriptor* d = reinterpret_cast<LNRenderPassDescriptor*>(handle);
    d->renderTargets[index].renderTarget = renderTarget;
    d->renderTargets[index].clearColor[0] = clearR;
    d->renderTargets[index].clearColor[1] = clearG;
    d->renderTargets[index].clearColor[2] = clearB;
    d->renderTargets[index].clearColor[3] = clearA;
    d->renderTargets[index].clearEnable = clearEnable;
}

void LNRenderPassDescriptor_SetDepthBuffer(
    LNStructHandle handle,
    LNHandle depthBuffer,
    float clearDepth,
    int32_t clearStencil,
    LNBool clearDepthEnable,
    LNBool clearStencilEnable) {
    LNRenderPassDescriptor* d = reinterpret_cast<LNRenderPassDescriptor*>(handle);
    d->depthBuffer.depthBuffer = depthBuffer;
    d->depthBuffer.clearDepth = clearDepth;
    d->depthBuffer.clearStencil = clearStencil;
    d->depthBuffer.clearDepthEnable = clearDepthEnable;
    d->depthBuffer.clearStencilEnable = clearStencilEnable;
}

//==============================================================================
// LNConfig
//   NOTE: 設定は個別モジュールではなく、全体で取りまとめる Config クラスにすることとした。
//     一度分けて運用してみたところ、次のような問題を感じたため。
//     - 適用範囲がわかりやすくなるが探しづらくなる
//     - C_API で構造体をネストするような仕組みにすると言語バインディングが作り辛くなる
//     - Scene などをコアから除外したことで、ほとんどの場合はコアを一括で初期化することになるため分ける意味があまりない。
//==============================================================================
static EngineOptions s_engineOptions;

LNResult LNConfig_SetGraphicsBackend(LNGraphicsBackend value) {
    LN_FFI_TRY_BEGIN;
    s_engineOptions.graphics.graphicsAPI = value;
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
//
//==============================================================================

LNResult LNInstance_Initialize() {
    LN_FFI_TRY_BEGIN;
    const auto result = ln::Engine::initialize(s_engineOptions);
    if (!result) {
        return LN_ERROR_UNKNOWN;
    }
    LN_FFI_TRY_END_RETURN;
}

void LNInstance_Terminate() {
    LN_FFI_TRY_BEGIN;
    ln::Engine::terminate();
    LN_FFI_TRY_END;
}
//==============================================================================
// LNInstance
//==============================================================================

LNResult LNInstance_ProcessEvents() {
    LN_FFI_TRY_BEGIN;
    ln::detail::PlatformManager* manager = ln::EngineInstance::instance()->platformManager();
    manager->processSystemEventQueue();
    LN_FFI_TRY_END_RETURN;
}

LNResult LNInstance_ShouldQuit(LNBool* outQuit) {
    LN_FFI_TRY_BEGIN;
    ln::detail::PlatformManager* manager = ln::EngineInstance::instance()->platformManager();
    *outQuit = manager->shouldQuit();
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
//
//==============================================================================
LNResult LNGraphicsContext_BeginFrame(
    LNHandle graphicsContext,
    int32_t width,
    int32_t height,
    LNHandle* outColorBuffer,
    LNHandle* outDepthBuffer,
    LNHandle* outCommandList) {
    LN_FFI_TRY_BEGIN;
    SurfaceContext* surfaceContext = LN_HANDLE_TO_OBJECT(SurfaceContext, graphicsContext);
    surfaceContext->beginFrame();
    GraphicsContext* context = surfaceContext->context();
    std::cout << "LNGraphicsContext_BeginFrame currentBackbuffer:" << context->currentBackbuffer() << std::endl;
    std::cout << "LNGraphicsContext_BeginFrame currentDepthBuffer:" << context->currentDepthBuffer() << std::endl;
    *outColorBuffer = ln::Runtime::wrapObject(context->currentBackbuffer(), false);
    *outDepthBuffer = ln::Runtime::wrapObject(context->currentDepthBuffer(), false);
    *outCommandList = ::Runtime::wrapObject(surfaceContext, false);
    surfaceContext->commandList()->reset();
    TRY_FFI_RESULT(surfaceContext->commandList()->beginCommandRecoding());
    LN_FFI_TRY_END_RETURN;
}

LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext_) {
    LN_FFI_TRY_BEGIN;
    //SurfaceContext* renderingContext2 = LN_HANDLE_TO_OBJECT(SurfaceContext, commandList_);
    SurfaceContext* context = LN_HANDLE_TO_OBJECT(SurfaceContext, graphicsContext_);
    CommandList* renderingContext = context->renderingContext();

    GraphicsCommandList* commandList = context->commandList();

    // Render Debug
    {
        detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
        TRY_FFI_RESULT(renderingManager->debugPrint()->render(context, renderingContext));
    }

    TRY_FFI_RESULT(commandList->endCommandRecoding());

    // この後 Present までの間で、描画結果などを VRAM から RAM に転送することがある。ユニットテストとか。
    // 別案として CaptureRequest だけ投げておいてコールバックを呼んでもらう方法も考えたが、
    // バックバッファのキャプチャ以外でも使うかもしれないので、コールバックはやめておく。
    context->context()->submitCurrentCommandList();

    context->endFrame();
    LN_FFI_TRY_END_RETURN;
}

LNResult LNGraphicsContext_CreateFromWebGPUCanvas(const char* selector, LNHandle* outGraphicsContext) {
    LN_FFI_TRY_BEGIN;
    WebGPUGraphicsContext::Settings s;
    s.selectorOrNull = selector;
    s.debugMode = true; // TODO:
    Ref<WebGPUGraphicsContext> context = WebGPUGraphicsContext::create(s);
    Ref<SurfaceContext> surfaceContext = SurfaceContext::createFromExternal(
        EngineInstance::instance()->renderingManager(),
        context);
    *outGraphicsContext = ::Runtime::wrapObject(surfaceContext, true);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNGraphicsContext_CreateFromCurrentGL(int32_t width, int32_t height, LNHandle* outGraphicsContext) {
    LN_FFI_TRY_BEGIN;
    OpenGLGraphicsContext::Settings s;
    s.window = nullptr;
    s.width = width;
    s.height = height;
    Ref<OpenGLGraphicsContext> context = OpenGLGraphicsContext::create(s);
    detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
    Ref<SurfaceContext> surfaceContext = SurfaceContext::createFromExternal(renderingManager, context);
    *outGraphicsContext = ::Runtime::wrapObject(surfaceContext, true);
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
// LNCommandList
//   NOTE: 名前について
//     - LNGraphicsCommandList: 冗長なので LNCommandList に変更した。
//       元々 Audio など他のモジュールでも CommandList というものを公開するかも知れないと考えていたが、
//       FFI のコンセプト変更により、GraphicsCommandList Graphics 以外で CommandList 的なものを公開することはないと考えられるため。
//==============================================================================

LNResult LNCommandList_BeginRenderPass(
    LNHandle renderingCommandList_,
    LNRenderPassDescriptor descriptor_,
    LNHandle renderingViewPoint_,
    LNHandle* outRenderPass_) {
    LN_FFI_TRY_BEGIN;
    SurfaceContext* renderingContext = LN_HANDLE_TO_OBJECT(SurfaceContext, renderingCommandList_);
    RenderViewPoint* renderingViewPoint = LN_HANDLE_TO_OBJECT(RenderViewPoint, renderingViewPoint_);
    renderingContext->renderingContext()->clearCommandsAndState(renderingViewPoint);

    
    Ref<FFIRenderPass> renderPass = makeObject_deprecated<FFIRenderPass>();
    renderPass->owner = renderingContext;

    ln::ClearFlags clearFlags = ln::ClearFlags::None;
    if (descriptor_.renderTargets[0].clearEnable) {
        clearFlags = clearFlags | ln::ClearFlags::Color;
	}
    if (descriptor_.depthBuffer.clearDepthEnable) {
		clearFlags = clearFlags | ln::ClearFlags::Depth;
	}
	if (descriptor_.depthBuffer.clearStencilEnable) {
		clearFlags = clearFlags | ln::ClearFlags::Stencil;
	}

    renderPass->renderPass = RenderPass::get(
        LN_HANDLE_TO_OBJECT(RenderTargetTexture, descriptor_.renderTargets[0].renderTarget), 
        LN_HANDLE_TO_OBJECT(DepthBuffer, descriptor_.depthBuffer.depthBuffer),
        static_cast<ln::ClearFlags>(clearFlags),
        Color(
            descriptor_.renderTargets[0].clearColor[0],
            descriptor_.renderTargets[0].clearColor[1],
            descriptor_.renderTargets[0].clearColor[2],
            descriptor_.renderTargets[0].clearColor[3]),
        descriptor_.depthBuffer.clearDepth,
        descriptor_.depthBuffer.clearStencil
    );

    detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
    renderPass->sceneRenderPass = makeRef<kanata::UnlitRenderPass>(renderingManager);

    *outRenderPass_ = ::Runtime::wrapObject(renderPass, true);

    
    // 背景クリアテスト
    renderingContext->commandList()->beginRenderPass(renderPass->renderPass);
    //renderingContext->commandList->clear(ClearFlags::All, Color::Aqua);
    renderingContext->commandList()->endRenderPass();

    LN_FFI_TRY_END_RETURN;
}

LNResult LNCommandList_EndRenderPass(LNHandle renderingCommandList_, LNHandle renderPass_) {
    LN_FFI_TRY_BEGIN;
    FFIRenderPass* renderPass = LN_HANDLE_TO_OBJECT(FFIRenderPass, renderPass_);
    GraphicsContext* context = renderPass->owner->context();
    CommandList* renderingContext = renderPass->owner->renderingContext();
    kanata::DrawEventList* drawEventList = renderPass->owner->drawEventList();

    if (1) {

        //RenderPass* renderPass = context->currentRenderPass();
        GraphicsCommandList* commandList = context->currentCommandList2();

        const RenderViewPoint* renderingViewPoint = renderingContext->viewPoint();

        //renderPass->setClearFlags(ClearFlags::Color);
        //renderPass->setClearColor(Color::Red);
        renderingContext->setTransfrom(Matrix::Identity);

        // commandList->beginCommandRecoding();
        // commandList->beginRenderPass(renderPass);
        // commandList->clear(ClearFlags::Color, Color::Red);
        // commandList->endRenderPass();
        // commandList->endCommandRecoding();

#if 1
        {
            using namespace ln;
            // URef<kanata::BatchCollector> g_batchList;
            //URef<kanata::BoxMeshBatchProxy> g_boxMeshBatchProxy;

            detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
            // g_batchList = makeURef<kanata::BatchCollector>(renderingManager);
            kanata::BatchCollector* batchList = renderingContext->batchCollector();
            //g_drawCommandList =
            //g_boxMeshBatchProxy = makeURef<kanata::BoxMeshBatchProxy>();

            detail::RenderViewInfo renderViewInfo;
            // renderViewInfo.cameraInfo.makePerspective(Vector3(10, 10, 10), Vector3::normalize(-1, -1, -1), 0.3, Size(renderPass->width(), renderPass->height()), 0.1, 1000.0);
            detail::SceneInfo sceneInfo;

            // RenderViewPoint viewPoint;
            // viewPoint.resetPerspective(Vector3(10, 10, 10), Vector3::normalize(-1, -1, -1), 0.3, Size(renderPass->width(), renderPass->height()), 0.1, 1000.0);
            renderingViewPoint->makeCameraInfo(&renderViewInfo.cameraInfo);

            kanata::BatchProxyState batchState;
            batchState.reset();
            batchState.m_depthTestEnabled = false;
            batchState.m_cullingMode = CullMode::None;

            batchList->clear(renderingViewPoint);

            // Build batch test.
            Ref<VertexBuffer> g_vertexBuffer;
            Ref<Material> material;
            if (0) {
                Vertex v[] = {
                    // Vertex(Vector3(0, 5.5, 0), Vector3(0, 0, 1), Vector2(0, 0), Color::Red),
                    // Vertex(Vector3(-5.5, 0, 0), Vector3(0, 0, 1), Vector2(1, 0), Color::Red),
                    // Vertex(Vector3(5.5, 0, 0), Vector3(0, 0, 1), Vector2(0, 1), Color::Red),

                    Vertex(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector2(0, 0), Color::Red),
                    Vertex(Vector3(0, 10, 0), Vector3(0, 0, 1), Vector2(0, 1), Color::Green),
                    Vertex(Vector3(10, 0, 0), Vector3(0, 0, 1), Vector2(1, 0), Color::Blue),
                };
                g_vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

                Ref<Material> material = Material::create();

                // 手動で頑張るパターン
                batchList->batchProxyState = &batchState;
                auto* batch = batchList->newBatch<kanata::Batch>(1, material);
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

                auto& r = EngineInstance::instance()->renderingManager()->spriteRenderer();
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
            }




            // Render commands
            {
                auto& batchProxyCollector = renderingContext->batchProxyCollector();
                batchProxyCollector->resolveSingleFrameBatchProxies(batchList);

                drawEventList->clear();
                //ln::ElapsedTimer t1;
                renderPass->sceneRenderPass->buildDrawEvents(
                    nullptr,
                    batchList,
                    commandList,
                    renderPass->renderPass,
                    renderViewInfo,
                    sceneInfo,
                    drawEventList);
                //std::cout << t1.elapsedMilliseconds() << "[ms] buildDrawCommands" << std::endl;
                ln::ElapsedTimer t2;
                drawEventList->submitDrawEvents(commandList);
            }

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
            Three.js のように SceneGraph が一級市民なフレームワークでは、 RenderingContext が隠れるので良いだろう。
            でも Lumino は SceneGraph が Engine から独立しているので、RenderingContext は公開される。

            この Proxy の収取を担当しているのは BatchProxyCollector.
            このクラスは RenderingContext が既に持っている。

            */
        }
#endif
    }

    // TODO: まだキャッシュとかできていなのでこれが必要
    LNObject_Release(renderPass_);
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
// LNDebug
//==============================================================================
LNResult LNDebug_Print(LNHandle graphicsContext, const char* str) {
    LN_FFI_TRY_BEGIN;
    SurfaceContext* renderingContext = LN_HANDLE_TO_OBJECT(SurfaceContext, graphicsContext);
    GraphicsCommandList* commandList = renderingContext->commandList();
    detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
    renderingManager->debugPrint()->print(str);
    LN_FFI_TRY_END_RETURN;
}


LNResult LNDebug_GetGraphicsProfilerng(LNHandle graphicsContext, LNGraphicsProfilerng* outProfilerng) {
    LN_FFI_TRY_BEGIN;
    SurfaceContext* surfaceContext = LN_HANDLE_TO_OBJECT(SurfaceContext, graphicsContext);
    GraphicsCommandList* commandList = surfaceContext->commandList();
    outProfilerng->drawCallCount = commandList->m_drawCall;
    outProfilerng->lastFrameTime = surfaceContext->lastFrameTime();

    PlatformWindow* window = surfaceContext->ownerWindowOrNull();
    if (window) {
        outProfilerng->actualFPS = window->fpsController()->totalFps();
    }
    else {
        outProfilerng->actualFPS = 0.0f;
    }
    LN_FFI_TRY_END_RETURN;
}


//==============================================================================
// LNRenderPass
//==============================================================================



//==============================================================================
// LNViewPoint
//   NOTE: 名前について
//     多くの 3Dライブラリでは、 Camera は次のような機能を持っている。
//     - ビュー行列・プロジェクション行列の生成
//     - SceneNode のひとつ。
//     - 必要に応じて、描画先の RenderTarget を持つ。
//     - 動的 CubeMap など、より高度な描画のエントリポイントを提供する。
//     - ポストプロセスのアタッチ先となる。
//     Lumino のコアモジュールとしては、行列生成以外は提供しないこととしたい。
//     ポストプロセスのカスタマイズなど、C_API として提供するのが面倒なものもあるが、
//     そういった高度な描画は Lumino を利用するフレームワークに任せたい。 Scene と同じ。
//     そのためあまり大きな機能を連想させないように、ViewPoint という名前にした。
//   NOTE: 低レイヤー志向なら行列の生成もユーザープログラムに任せて良いのでは？
//     各種化リングやZソートは Lumino 内部で行いたい。それには行列が必要。
//==============================================================================
LNResult LNViewPoint_Create(LNHandle* outRenderingViewPoint) {
    LN_FFI_TRY_BEGIN;
    Ref<RenderViewPoint> viewPoint = makeRef<RenderViewPoint>();
    *outRenderingViewPoint = ::Runtime::wrapObject(viewPoint, true);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNViewPoint_SetupPerspectiveOrthoLH(
    LNHandle graphicsViewPoint,
    float x,
    float y,
    float z,
    float lookAtX,
    float lookAtY,
    float lookAtZ,
    float width,
    float height,
    float nearZ,
    float farZ) {
    LN_FFI_TRY_BEGIN;
    RenderViewPoint* viewPoint = LN_HANDLE_TO_OBJECT(RenderViewPoint, graphicsViewPoint);
    viewPoint->resetPerspectiveOrthoLH(
        Vector3(x, y, z), Vector3(lookAtX, lookAtY, lookAtZ), Size(width, height), nearZ, farZ);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNViewPoint_SetupPerspective2DLH(
    LNHandle graphicsViewPoint,
    float x,
    float y,
    float z,
    float width,
    float height,
    float nearZ,
    float farZ) {
    LN_FFI_TRY_BEGIN;
    RenderViewPoint* viewPoint = LN_HANDLE_TO_OBJECT(RenderViewPoint, graphicsViewPoint);
    viewPoint->resetPerspective2DLH(Vector3(x,y, z), Size(width, height), nearZ, farZ);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNUnlitSceneRenderingPass_Create(LNHandle* outUnlitSceneRenderingPass) {
    LN_FFI_TRY_BEGIN;
    detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
    Ref<kanata::UnlitRenderPass> renderingPass = makeRef<kanata::UnlitRenderPass>(renderingManager);
    *outUnlitSceneRenderingPass = ::Runtime::wrapObject(renderingPass, true);
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
// LNTexture2D
//==============================================================================

LNResult LNTexture2D_Create(int32_t width, int32_t height, LNHandle* outTexture2D) {
    LN_FFI_TRY_BEGIN;
    Ref<Texture2D> texture = Texture2D::create(width, height);
    *outTexture2D = ::Runtime::wrapObject(texture, true);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNTexture2D_CreateFromImageFileData(const uint8_t* data, int32_t length, LNHandle* outTexture2D) {
    LN_FFI_TRY_BEGIN;
    Result<Ref<Texture2D>> texture = Texture2D::createFromImageFileData(data, length);
    if (!texture) {
        return TO_FFI_ERROR(texture.error());
    }
    *outTexture2D = ::Runtime::wrapObject(texture.value(), true);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNTexture2D_CreateFromImageFile(const char* filePathUTF8, LNHandle* outTexture2D) {
    LN_FFI_TRY_BEGIN;
    auto data = ln::FileSystem::readAllBytes(ln::String::fromUtf8(filePathUTF8));
    if (!data) {
        return TO_FFI_ERROR(data.error());
    }
    return LNTexture2D_CreateFromImageFileData(data->data(), data->size(), outTexture2D);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNTexture2D_GetContext(LNHandle texture2D_, LNHandle* outTextureRenderingContext_) {
    LN_FFI_TRY_BEGIN;
    Texture2D* texture2D = LN_HANDLE_TO_OBJECT(Texture2D, texture2D_);
    *outTextureRenderingContext_ = ::Runtime::wrapObject(texture2D->getContext(), false);
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
// LNShader
//==============================================================================
LNResult LNShader_CreateFromSourceCode(const void* code, int32_t length, LNHandle* outShader) {
    LN_FFI_TRY_BEGIN;
    LN_NOTIMPLEMENTED();
    //Ref<Shader> shader = Shader::create(data, length);
    //*outShader = ::Runtime::wrapObject(shader, true);
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
// LNMaterial
//==============================================================================
LNResult LNMaterial_Create(LNHandle* outMaterial) {
    LN_FFI_TRY_BEGIN;
    Ref<Material> material = Material::create();
    material->setShader(EngineInstance::instance()->renderingManager()->builtinShader(detail::BuiltinShader::Sprite));
    material->setBlendMode(BlendMode::Alpha);
    *outMaterial = ::Runtime::wrapObject(material, true);
	LN_FFI_TRY_END_RETURN;
}

LNResult LNMaterial_CreateFromSourceFile(const char* filePathUTF8, LNHandle* outMaterial) {
    LN_FFI_TRY_BEGIN;
    Ref<Shader> shader = Shader::createFromSourceFile(std::filesystem::u8path(filePathUTF8));
    Ref<Material> material = Material::create();
    material->setShader(shader);
    material->setBlendMode(BlendMode::Alpha);
    *outMaterial = ::Runtime::wrapObject(material, true);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNMaterial_SetMainTexture(LNHandle material_, LNHandle texture_) {
    LN_FFI_TRY_BEGIN;
    Material* material = LN_HANDLE_TO_OBJECT(Material, material_);
    Texture* texture = LN_HANDLE_TO_OBJECT(Texture, texture_);
    material->setMainTexture(texture);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNMaterial_SetShader(LNHandle material_, LNHandle shader_) {
    LN_FFI_TRY_BEGIN;
    Material* material = LN_HANDLE_TO_OBJECT(Material, material_);
    Shader* shader = LN_HANDLE_TO_OBJECT(Shader, shader_);
    material->setShader(shader);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNMaterial_FindParameterIndex(LNHandle material, const char* name, int32_t* outIndex) {
    LN_FFI_TRY_BEGIN;
    Material* m = LN_HANDLE_TO_OBJECT(Material, material);
    *outIndex = m->findParameterIndex(name);
    LN_FFI_TRY_END_RETURN;

}
LNResult LNMaterial_SetFloat(LNHandle material, int32_t parameterIndex, float value) {
    LN_FFI_TRY_BEGIN;
    Material* m = LN_HANDLE_TO_OBJECT(Material, material);
    m->setFloat(parameterIndex, value);
    LN_FFI_TRY_END_RETURN;
}   

LNResult LNMaterial_CanCombined(LNHandle material_, LNHandle texture_, LNBool* outResult_) {
    LN_FFI_TRY_BEGIN;
    *outResult_ = LN_FALSE; // TODO:
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
// LNTextureRenderingContext
//==============================================================================
LNResult LNTextureRenderingContext_FillText(LNHandle textureRenderingContext) {
    LN_FFI_TRY_BEGIN;
	BitmapRenderingContext* context = LN_HANDLE_TO_OBJECT(BitmapRenderingContext, textureRenderingContext);

    detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
        context->drawText(U"Hello, Lumino!",
            Rect(0, 0, 200, 100), renderingManager->fontManager()->defaultFont(), Color::White);

	LN_FFI_TRY_END_RETURN;
}

LNResult LNTextureRenderingContext_StrokeText(LNHandle textureRenderingContext) {
    LN_FFI_TRY_BEGIN;
    BitmapRenderingContext* context = LN_HANDLE_TO_OBJECT(BitmapRenderingContext, textureRenderingContext);

    detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
    context->drawText(
        U"Hello, Lumino!",
        Rect(0, 0, 200, 100),
        renderingManager->fontManager()->defaultFont(),
        Color::Black,
        TextAlignment::Forward,
        TextDrawMode::Stroke,
        1.0);

    LN_FFI_TRY_END_RETURN;
}

//==============================================================================

LNResult LNBatchRenderer_Get(LNHandle* outSpriteRenderer) {
    LN_FFI_TRY_BEGIN;
	detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
    BatchRenderer* spriteRenderer = renderingManager->spriteRenderer();
        *outSpriteRenderer = ::Runtime::wrapObject(spriteRenderer, false);
	LN_FFI_TRY_END_RETURN;
}

LNResult LNBatchRenderer_BeginBatch(
    LNHandle spriteRenderer_,
    LNHandle graphicsCommandList_,
    LNHandle material_,
    const LNMatrix* transform_) {
    // NOTE: なぜ BeginBatch, EndBatch を用意しているのか？
    //      Zソートを行う以上、現実的には BeginBatch～EndBatch の間までに、ひとつの Sprite しか描画するべきではない。
    //      それなのになぜ？
    //      ※注意点(覚書): 複数の BatchProxy をひとつのドローコールにマージするかどうかは Encoder の仕事なので、ここで考えることではない
    //
    //      BeginBatch, EndBatch を提供することで可能となるのは、複数の Sprite や Mesh をひとつの BatchProxy にできること。
    //      提供しない場合、自動的に Batch をマージすることは一切できない。これはZソートができなくなる方が問題が大きいから。
    //      → マージできるようなフラグを与えても良いが、それなら BeginBatch, EndBatch でも同じだしこちらの方が（冗長ではあるけどその分）わかりやすい。
    //      これが役に立つのは、カリング、Zソートなどをせずに、プログラマの意図でまとめて大量のメッシュを描きたいとき。
    //      例えば、
    //      - 木や小石などの不透明な背景オブジェクト（主に3D）
    //      - ユーザープログラム側でソート済みの大量のパーティクル
    //      直近で使う機能ではないが、かといって無いのは今後困ることがあるかもしれない。
    //
    // NOTE: [2024/11/21] なぜ SpriteRenderer を BatchRenderer としたのか？
    //      従来の PrimitiveMeshRenderer や FrameRectRenderer もそうだけど、これらの役割は要するに BatchProxy を作ること。
    //      現時点ではまだ BatchMaterial が Material と 1:1 でテクスチャを扱うようになっているからダメだけど、
    //      それができれば SpriteTextRenderer, VectorTextRenderer も同じようにできる。
    // 
    //      今の PrimitiveMeshRenderer がそんな感じだけど、
    //      同じクラスの BatchProxy を使っていて、メッシュの生成は MeshGenerater に任せている。
    // 
    //      ただ、 TextRenderer の仕組みは MeshGenerater ではカバーできない。
    //      BatchRenderer に全部統一するなら、 SpriteEncoder や TextSpriteEncoder に仕事を振る人が必要。
    //      BatchEncoderDispatcher とかにしようか。
    // 
    // NOTE: とはいえ、ここまでやる必要があるのか？
    //      汎用性の麺でもメリットはある。例えば☆を描くのに、次のようにできるかもしれない。
    //      ```
    //      encoderRegistry->register(new StarBatchEncoder());
    //      batchRenderer->drawInstruction(label: "Star", data: newFrameData<StarInstruction>(pos: ..., size: ...));
    //      ```
    //      従来だと Encoder(Feature) と Renderer のペアが必要だったが、不要になる。
    // 
    // NOTE: Effekseer など他のレンダリングライブラリを組み込むうえで問題はあるか？
    //      Effekseer の場合は歪みを使う場合それ自体が RenderPass を複数使うように動くことがある（かも。未調査だけど、普通はそうしないとできないはず）
    //      その場合は RenderPass の外側になるので、 CommandList に queueCustomRendering() みたなの作っておけばよいと思う。
    //
    // NOTE: 旧 RenderingContext(CommandList) との違いは？
    //      RenderingContext 自体が Command のマージを行うかどうか。
    //      RenderingContext はステートを確認しマージしていたため、個々の Command の詳細を理解しなければならず、複雑化していた。
    //      また、何か機能を追加するたびにここに変更を入れなければならず、状態遷移の条件が増えることになるためメンテコストも大きかった。
    //      それに対して新しい BatchRenderer は Command(BatchProxy) を追加するだけにする。
    //      Q. でもそうすると BeginBatch, EndBatch は必要なのか？
    //      → 目的は Z ソートの抑制となる。
    // 
    // NOTE: Zソート抑制に傾倒して、通常使用時のオーバーヘッドが増えたりしないか？
    //      以前の SpriteRenderer は SpriteData の配列を Encoder に送っていたが、通常使用時は [0] しか使わない。
    //      なのでそんなに以前とはかわらないはず。
    //
    // NOTE: 名称について
    //	    BeginBatch,EndBatch だとやっぱりちょっと混乱するかもしれない。
    //      厳密には Batch ではなくて、Node とか Object とかの方が正しいか？
    //

    LN_FFI_TRY_BEGIN;
    //std::cout << "spriteRenderer: " << spriteRenderer_ << std::endl;
    //std::cout << "graphicsCommandList: " << graphicsCommandList_ << std::endl;
    //std::cout << "material: " << material_ << std::endl;
    //std::cout << "transform: " << transform_ << std::endl;
    //std::cout << "  transform m11: " << transform_->m11 << std::endl;
    //std::cout << "  transform m12: " << transform_->m12 << std::endl;
    //std::cout << "  transform m41: " << transform_->m41 << std::endl;
    //std::cout << "  transform m42: " << transform_->m42 << std::endl;
    //std::cout << "  transform m43: " << transform_->m43 << std::endl;
    //std::cout << "  transform m44: " << transform_->m44 << std::endl;
    BatchRenderer* spriteRenderer = LN_HANDLE_TO_OBJECT(BatchRenderer, spriteRenderer_);
    SurfaceContext* commandList = LN_HANDLE_TO_OBJECT(SurfaceContext, graphicsCommandList_);
    Material* material = LN_HANDLE_TO_OBJECT(Material, material_);

    if (transform_) {
        const Matrix* transform = reinterpret_cast<const Matrix*>(transform_);
        commandList->renderingContext()->setTransfrom(*transform);
    }
    else {
        commandList->renderingContext()->setTransfrom(Matrix::Identity);
    }
    spriteRenderer->begin(commandList->renderingContext(), material);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNBatchRenderer_EndBatch(LNHandle spriteRenderer_) {
    LN_FFI_TRY_BEGIN;
	BatchRenderer* spriteRenderer = LN_HANDLE_TO_OBJECT(BatchRenderer, spriteRenderer_);
	spriteRenderer->end();
	LN_FFI_TRY_END_RETURN;
}

LNResult LNBatchRenderer_DrawSprite_deprecated(
    LNHandle spriteRenderer,
    const LNMatrix* localTransformOrNull,
    float width,
    float height,
    float anchorRatioX,
    float anchorRatioY,
    float uvRectX,
    float uvRectY,
    float uvRectW,
    float uvRectH,
    float r,
    float g,
    float b,
    float a,
    LNSpriteBaseDirection baseDirection,
    LNBillboardType billboardType) {
        //std::cout << "anchorRatioX: " << anchorRatioX << std::endl;
        //std::cout << "anchorRatioY: " << anchorRatioY << std::endl;
        //std::cout << "uvRectX: " << uvRectX << std::endl;
        //std::cout << "uvRectY: " << uvRectY << std::endl;
        //std::cout << "uvRectW: " << uvRectW << std::endl;
        //std::cout << "uvRectH: " << uvRectH << std::endl;

    LN_FFI_TRY_BEGIN;
    const Matrix* localTransform = reinterpret_cast<const Matrix*>(localTransformOrNull);
    BatchRenderer* renderer = LN_HANDLE_TO_OBJECT(BatchRenderer, spriteRenderer);
    renderer->drawSprite(
        (localTransformOrNull) ? *reinterpret_cast<const Matrix*>(localTransformOrNull) : Matrix::Identity,
        Size(width, height),
        Vector2(anchorRatioX, anchorRatioY),
        Rect(uvRectX, uvRectY, uvRectW, uvRectH),
        Color(r, g, b, a),
        static_cast<SpriteBaseDirection>(baseDirection),
        static_cast<BillboardType>(billboardType),
        SpriteFlipFlags::None);
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
// LNSpriteTextRenderer
//==============================================================================
LNResult LNSpriteTextRenderer_Get(LNHandle* outSpriteTextRenderer) {
    LN_FFI_TRY_BEGIN;
    detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
    SpriteTextRenderer* spriteTextRenderer = renderingManager->spriteTextRenderer();
    *outSpriteTextRenderer = ::Runtime::wrapObject(spriteTextRenderer, false);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNSpriteTextRenderer_BeginBatch(
    LNHandle spriteTextRenderer_,
    LNHandle graphicsCommandList_,
    LNHandle material_,
    const LNMatrix* transform_) {
    LN_FFI_TRY_BEGIN;
    SpriteTextRenderer* spriteTextRenderer = LN_HANDLE_TO_OBJECT(SpriteTextRenderer, spriteTextRenderer_);
    CommandList* commandList = LN_HANDLE_TO_OBJECT(CommandList, graphicsCommandList_);
    Material* material = LN_HANDLE_TO_OBJECT(Material, material_);
    const Matrix* transform = reinterpret_cast<const Matrix*>(transform_);
    commandList->setTransfrom(*transform);
    spriteTextRenderer->begin(commandList, material);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNSpriteTextRenderer_EndBatch(LNHandle spriteTextRenderer_) {
    LN_FFI_TRY_BEGIN;
    SpriteTextRenderer* spriteTextRenderer = LN_HANDLE_TO_OBJECT(SpriteTextRenderer, spriteTextRenderer_);
    spriteTextRenderer->end();
    LN_FFI_TRY_END_RETURN;
}

#if 0
LNResult LNSpriteTextRenderer_DrawFillText(LNHandle spriteTextRenderer_, const LNMatrix* localTransformOrNull_, const char* text_) {
    LN_FFI_TRY_BEGIN;
	SpriteTextRenderer* renderer = LN_HANDLE_TO_OBJECT(SpriteTextRenderer, spriteTextRenderer_);

    Ref<detail::FontRequester> font = makeRef<detail::FontRequester>();
    String text = String::fromUtf8(text_);
    detail::RenderingManager* renderingManager = EngineInstance::instance()->renderingManager();
    renderer->drawFillText(
        (localTransformOrNull_) ? *reinterpret_cast<const Matrix*>(localTransformOrNull_) : Matrix::Identity,
        Vector2::Zero,
        SpriteBaseDirection::Basic2D,
        nullptr,
		text,
        renderingManager->fontManager()->defaultFont(),
        Color::Green,
        Rect(0, 0, 300, 300),
        TextAlignment::Forward,
        font.get());
	LN_FFI_TRY_END_RETURN;
}
#endif

//==============================================================================
// LNWindow
//==============================================================================
LNResult LNWindow_Create(int32_t width, int32_t height, const LNChar* title, LNHandle* outWindow) {
    LN_FFI_TRY_BEGIN;
    EngineInstance* instance = EngineInstance::instance();
    detail::PlatformManager* platformManager = instance->platformManager();
    WindowCreationSettings options;
    options.clientWidth = width;
    options.clientHeight = height;
    options.title = String::fromUtf8(title);
    Result_deprecated<Ref<PlatformWindow>> window = platformManager->createWindow(options);
    if (!window) return TO_FFI_ERROR(window);
    *outWindow = ::Runtime::wrapObject(window.unwrap(), true);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNWindow_GetFramebufferSize(LNHandle window, int32_t* outWidth, int32_t* outHeight) {
    // NOTE: 名前について
    //   - ViewportSize: 画面内のサブセクションを指定する機能としての Viewport　と衝突するので使わない。
    //   - BackbufferSize vs FramebufferSize: Framebuffer の方が、
    //     低レイヤーグラフィックスにおけるサイズである (DPI とは関係ない) ことを強調できると考えたため、 FramebufferSize とする。
    //     OpenGL と Vulkan の用語ではあるけど、 DX12 関連の記事でもたまに見かける。
    LN_FFI_TRY_BEGIN;
    PlatformWindow* platformWindow = LN_HANDLE_TO_OBJECT(PlatformWindow, window);
    platformWindow->getFramebufferSize(outWidth, outHeight);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNWindow_GetGraphicsContext(LNHandle window, LNHandle* outGraphicsContext) {
    // NOTE: Window から Context を得るのは Get. (Create ではなく)
    //   これは、Window に Present を担当させるため。
    //   Create は、主に外部の Context を使うときに使う。
    LN_FFI_TRY_BEGIN;
    PlatformWindow* platformWindow = LN_HANDLE_TO_OBJECT(PlatformWindow, window);
    *outGraphicsContext = ::Runtime::wrapObject(platformWindow->surfaceContext(), false);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNWindow_Present(LNHandle window) {
    LN_FFI_TRY_BEGIN;
    PlatformWindow* platformWindow = LN_HANDLE_TO_OBJECT(PlatformWindow, window);
    TRY_FFI_RESULT(platformWindow->present());
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
// LNObject

LNResult LNObject_Release(LNHandle obj) {
    LN_FFI_TRY_BEGIN;
    auto* instance = ln::EngineInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED; 
    auto* manager = instance->runtimeManager().get();
    if (!manager) return LN_RUNTIME_UNINITIALIZED;
    manager->releaseObjectExplicitly(obj);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNObject_Retain(LNHandle obj) {
    LN_FFI_TRY_BEGIN;
    auto* instance = ln::EngineInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;
    auto* manager = instance->runtimeManager().get();
    if (!manager) return LN_RUNTIME_UNINITIALIZED;
    manager->retainObjectExplicitly(obj);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNObject_GetReferenceCount(LNHandle obj, int32_t* outReturn) {
    if (!outReturn) return LN_ERROR_INVALID_ARGUMENT;

    if (auto t = LN_HANDLE_TO_OBJECT(Object, obj)) {
        *outReturn = RefObjectHelper::getReferenceCount(t);
        return LN_OK;
    }
    else {
        return LN_ERROR_INVALID_ARGUMENT;
    }
}



#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
