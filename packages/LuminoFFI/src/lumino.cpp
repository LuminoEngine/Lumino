#include <stdio.h>
#include <LuminoEngine.hpp>


#include <LuminoEngine/RuntimeModule.hpp>
#include <LuminoEngine/RHIModule.hpp>
#include <LuminoEngine/Bitmap/BitmapRenderingContext.hpp>
#include <LuminoEngine/GPU/VertexBuffer.hpp>
#include <LuminoEngine/GPU/VertexLayout.hpp>
#include <LuminoEngine/GPU/RenderPass.hpp>
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/GPU/DepthBuffer.hpp>
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
#include <LuminoEngine/Rendering/FeatureRenderer/SpriteTextRenderer.hpp>
#include <LuminoEngine/Font/Font.hpp>

namespace ln {

class FFI {
public:
    static LNResult processException(Exception* e);

private:
};

LNResult FFI::processException(Exception* e) {
    return LN_ERROR_UNKNOWN;
}

} // namespace ln



using namespace ln;



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define LN_FFI_TRY_BEGIN try {

#define LN_FFI_TRY_END_RETURN               \
    }                                         \
    catch (Exception & e) {               \
        return FFI::processException(&e); \
    }                                         \
    return LN_OK;

#define LN_FFI_TRY_END                                                                                        \
    }                                                                                                                  \
    catch (Exception & e) {                                                                                            \
        FFI::processException(&e);                                                                              \
    }             

#define LN_HANDLE_TO_OBJECT(type, h) static_cast<type*>((h) ? ::Runtime::getObject(h) : nullptr)
//#define LN_HANDLE_TO_OBJECT2(type, h) static_cast<type*>((h) ? ::FFI::getObject(reinterpret_cast<LNHandle>(h)) : nullptr)
//#define LN_WRAP_OBJECT(type, obj, fromCreate) reinterpret_cast<type>(::FFI::wrapObject(obj, false))
#define LN_RELEASE_OBJECT(h) LNObject_Release(reinterpret_cast<LNHandle>(h))

// #define LN_DEFINE_HANDLE(object) typedef struct object##_T* object
// NOTE: ↑こういうタイプセーフなハンドル定義は行わない。
//   - 継承されたクラスを使いづらくなるため。
//     例えば LNTexture2D を LNTextre として LNMaterial にセットしたいときなど。
//     キャストして使ってもよいが、そうすると LNHandle のまま使うのと大差なくなるかも。

    
class FFIRenderingCommandList : public ln::Object {
public:
    GraphicsContext* context;
    GraphicsCommandList* commandList;
    Ref<CommandList> renderingContext;
    URef<kanata::DrawCommandList> drawCommandList;
};

class FFIRenderPass : public ln::Object {
public:
    FFIRenderingCommandList* owner;
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
//
//==============================================================================
LNResult LNRuntime_Initialize() {
    LN_FFI_TRY_BEGIN;
    const auto result = ln::Engine::initialize();
    if (!result) {
        return LN_ERROR_UNKNOWN;
    }
    LN_FFI_TRY_END_RETURN;
}

void LNRuntime_Terminate() {
    LN_FFI_TRY_BEGIN;
    ln::Engine::terminate();
    LN_FFI_TRY_END;
}

LNResult LNGraphicsContext_GetCurrentColorBuffer(LNHandle graphicsContext, LNHandle* outRenderTargetTexture) {
	LN_FFI_TRY_BEGIN;
	GraphicsContext* context = LN_HANDLE_TO_OBJECT(GraphicsContext, graphicsContext);
    *outRenderTargetTexture = ln::Runtime::wrapObject(context->currentBackbuffer(), false);
	LN_FFI_TRY_END_RETURN;
}

LNResult LNGraphicsContext_GetCurrentDepthBuffer(LNHandle graphicsContext, LNHandle* outDepthBuffer) {
    LN_FFI_TRY_BEGIN;
	GraphicsContext* context = LN_HANDLE_TO_OBJECT(GraphicsContext, graphicsContext);
    *outDepthBuffer = LN_NULL_HANDLE;
	LN_FFI_TRY_END_RETURN;
}        

LNResult LNGraphicsContext_BeginFrame(LNHandle graphicsContext, int32_t width, int32_t height) {
    LN_FFI_TRY_BEGIN;
    GraphicsContext* context = LN_HANDLE_TO_OBJECT(GraphicsContext, graphicsContext);
    GraphicsCommandList* commandList = context->currentCommandList2();
    commandList->beginCommandRecoding();
    LN_FFI_TRY_END_RETURN;
}

LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext) {
    LN_FFI_TRY_BEGIN;
    GraphicsContext* context = LN_HANDLE_TO_OBJECT(GraphicsContext, graphicsContext);
    GraphicsCommandList* commandList = context->currentCommandList2();
    commandList->endCommandRecoding();
    LN_FFI_TRY_END_RETURN;
}

LNResult LNGraphicsContext_Present(LNHandle graphicsContext) {
    LN_FFI_TRY_BEGIN;
    LN_NOTIMPLEMENTED();
    LN_FFI_TRY_END_RETURN;
}

//LNResult LNGraphicsContext_Release(LNHandle handle) {
//    LN_SAFE_RELEASE(handle)
//
//}

//==============================================================================

LNResult LNGLGraphicsContext_CreateFromCurrentGL(int32_t width, int32_t height, LNHandle* outGraphicsContext) {
    LN_FFI_TRY_BEGIN;
    OpenGLGraphicsContext::Settings s;
    s.window = nullptr;
    s.width = width;
    s.height = height;
    *outGraphicsContext = ::Runtime::wrapObject(OpenGLGraphicsContext::create(s), true);
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
//
//==============================================================================
extern LNResult LNGraphicsCommandList_Create(LNHandle graphicsContext, LNHandle* outGraphicsCommandList) {
    LN_FFI_TRY_BEGIN;
    GraphicsContext* context = LN_HANDLE_TO_OBJECT(GraphicsContext, graphicsContext);
    Ref<FFIRenderingCommandList> commandList = makeObject_deprecated<FFIRenderingCommandList>();
    auto* renderingManager = detail::RenderingManager::instance();
    commandList->context = context;
    commandList->commandList = context->currentCommandList2();
    commandList->renderingContext = makeObject_deprecated<CommandList>();
    commandList->drawCommandList = makeURef<kanata::DrawCommandList>(renderingManager);
    *outGraphicsCommandList = ::Runtime::wrapObject(commandList, true);
    LN_FFI_TRY_END_RETURN;
}

extern LNResult LNGraphicsCommandList_Reset(LNHandle renderingCommandList_) {
    LN_FFI_TRY_BEGIN;
    FFIRenderingCommandList* commandList = LN_HANDLE_TO_OBJECT(FFIRenderingCommandList, renderingCommandList_);
    commandList->commandList->beginCommandRecoding();
    LN_FFI_TRY_END_RETURN;
}

extern LNResult LNGraphicsCommandList_BeginRenderPass(
    LNHandle renderingCommandList_,
    LNRenderPassDescriptor descriptor_,
    LNHandle renderingViewPoint_,
    LNHandle* outRenderPass_) {
    LN_FFI_TRY_BEGIN;
    FFIRenderingCommandList* renderingContext = LN_HANDLE_TO_OBJECT(FFIRenderingCommandList, renderingCommandList_);
    RenderViewPoint* renderingViewPoint = LN_HANDLE_TO_OBJECT(RenderViewPoint, renderingViewPoint_);
    renderingContext->renderingContext->clearCommandsAndState(renderingViewPoint);

    
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

    std::cout << "cccc: " << descriptor_.renderTargets[0].renderTarget << std::endl;
    renderPass-> renderPass = RenderPass::get(
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

    auto* renderingManager = detail::RenderingManager::instance();
    renderPass->sceneRenderPass = makeRef<kanata::UnlitRenderPass>(renderingManager);

    *outRenderPass_ = ::Runtime::wrapObject(renderPass, true);

    
    // 背景クリアテスト
    renderingContext->commandList->beginRenderPass(renderPass->renderPass);
    //renderingContext->commandList->clear(ClearFlags::All, Color::Aqua);
    renderingContext->commandList->endRenderPass();

    LN_FFI_TRY_END_RETURN;
}

LNResult LNGraphicsContext_SubmitCommandList(
    LNHandle graphicsContext_,
    LNHandle renderingCommandList_) {
    LN_FFI_TRY_BEGIN;
    FFIRenderingCommandList* renderingContext2 = LN_HANDLE_TO_OBJECT(FFIRenderingCommandList, renderingCommandList_);
    CommandList* renderingContext = renderingContext2->renderingContext;
    GraphicsContext* context = LN_HANDLE_TO_OBJECT(GraphicsContext, graphicsContext_);
    
    
    GraphicsCommandList* commandList = context->currentCommandList2();
    commandList->endCommandRecoding();

    LN_FFI_TRY_END_RETURN;
}
//==============================================================================
// LNRenderPass
//==============================================================================

LNResult LNRenderPass_End(LNHandle renderPass_) {
    LN_FFI_TRY_BEGIN;
    FFIRenderPass* renderPass = LN_HANDLE_TO_OBJECT(FFIRenderPass, renderPass_);
    GraphicsContext* context = renderPass->owner->context;
    CommandList* renderingContext = renderPass->owner->renderingContext;
    kanata::DrawCommandList* g_drawCommandList = renderPass->owner->drawCommandList;
    
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

            auto* renderingManager = detail::RenderingManager::instance();
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

                auto& r = detail::RenderingManager::instance()->spriteRenderer();
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

            auto& batchProxyCollector = renderingContext->batchProxyCollector();
            batchProxyCollector->resolveSingleFrameBatchProxies(batchList);

            // Render commands
            {
                g_drawCommandList->clear();
                renderPass->sceneRenderPass->buildDrawCommands(
                    nullptr,
                    batchList,
                    commandList,
                    renderPass->renderPass,
                    renderViewInfo,
                    sceneInfo,
                    g_drawCommandList);
                g_drawCommandList->submitMeshDrawCommands(commandList);
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


LNResult LNGraphicsViewPoint_Create(LNHandle* outRenderingViewPoint) {
    LN_FFI_TRY_BEGIN;
    Ref<RenderViewPoint> viewPoint = makeRef<RenderViewPoint>();
    *outRenderingViewPoint = ::Runtime::wrapObject(viewPoint, true);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNGraphicsViewPoint_SetupPerspectiveOrthoLH(
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

LNResult LNGraphicsViewPoint_SetupPerspective2D(
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
    viewPoint->resetPerspective2D(Vector3(x,y, z), Size(width, height), nearZ, farZ);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNUnlitSceneRenderingPass_Create(LNHandle* outUnlitSceneRenderingPass) {
    LN_FFI_TRY_BEGIN;
    detail::RenderingManager* renderingManager = detail::RenderingManager::instance();
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
    //std::cout << "data: " << reinterpret_cast<int64_t>(data) << std::endl;
    //std::cout << "length: " << length << std::endl;
    //std::cout << "data[0]: " << static_cast<int>(data[0]) << std::endl;
    //std::cout << "data[1]: " << static_cast<int>(data[1]) << std::endl;
    Ref<Texture2D> texture = Texture2D::createFromImageFileData(data, length);
    *outTexture2D = ::Runtime::wrapObject(texture, true);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNTexture2D_GetContext(LNHandle texture2D_, LNHandle* outTextureRenderingContext_) {
    LN_FFI_TRY_BEGIN;
    Texture2D* texture2D = LN_HANDLE_TO_OBJECT(Texture2D, texture2D_);
    *outTextureRenderingContext_ = ::Runtime::wrapObject(texture2D->getContext(), false);
    LN_FFI_TRY_END_RETURN;
}

//==============================================================================
// LNMaterial
//==============================================================================
LNResult LNMaterial_Create(LNHandle* outMaterial) {
    LN_FFI_TRY_BEGIN;
    Ref<Material> material = Material::create();
    material->setShader(detail::RenderingManager::instance()->builtinShader(detail::BuiltinShader::Sprite));
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

    detail::RenderingManager* renderingManager = detail::RenderingManager::instance();
        context->drawText(U"Hello, Lumino!",
            Rect(0, 0, 200, 100), renderingManager->fontManager()->defaultFont(), Color::White);

	LN_FFI_TRY_END_RETURN;
}

LNResult LNTextureRenderingContext_StrokeText(LNHandle textureRenderingContext) {
    LN_FFI_TRY_BEGIN;
    BitmapRenderingContext* context = LN_HANDLE_TO_OBJECT(BitmapRenderingContext, textureRenderingContext);

    detail::RenderingManager* renderingManager = detail::RenderingManager::instance();
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

LNResult LNSpriteRenderer_Get(LNHandle* outSpriteRenderer) {
    LN_FFI_TRY_BEGIN;
	detail::RenderingManager* renderingManager = detail::RenderingManager::instance();
    SpriteRenderer* spriteRenderer = renderingManager->spriteRenderer();
        *outSpriteRenderer = ::Runtime::wrapObject(spriteRenderer, false);
	LN_FFI_TRY_END_RETURN;
}

LNResult LNSpriteRenderer_BeginBatch(
    LNHandle spriteRenderer_,
    LNHandle graphicsCommandList_,
    LNHandle material_,
    const LNMatrix* transform_) {
    LN_FFI_TRY_BEGIN;
    //std::cout << "spriteRenderer: " << spriteRenderer_ << std::endl;
    //std::cout << "graphicsCommandList: " << graphicsCommandList_ << std::endl;
    //std::cout << "material: " << material_ << std::endl;
    //std::cout << "transform: " << transform_ << std::endl;
    //std::cout << "transform m11: " << transform_->m11 << std::endl;
    //std::cout << "transform m12: " << transform_->m12 << std::endl;
    SpriteRenderer* spriteRenderer = LN_HANDLE_TO_OBJECT(SpriteRenderer, spriteRenderer_);
    FFIRenderingCommandList* commandList = LN_HANDLE_TO_OBJECT(FFIRenderingCommandList, graphicsCommandList_);
    Material* material = LN_HANDLE_TO_OBJECT(Material, material_);
    const Matrix* transform = reinterpret_cast<const Matrix*>(transform_);
    commandList->renderingContext->setTransfrom(*transform);
    spriteRenderer->begin(commandList->renderingContext, material);
    LN_FFI_TRY_END_RETURN;
}

LNResult LNSpriteRenderer_EndBatch(LNHandle spriteRenderer_) {
    LN_FFI_TRY_BEGIN;
	SpriteRenderer* spriteRenderer = LN_HANDLE_TO_OBJECT(SpriteRenderer, spriteRenderer_);
	spriteRenderer->end();
	LN_FFI_TRY_END_RETURN;
}

LNResult LNSpriteRenderer_DrawSprite(
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
    SpriteRenderer* renderer = LN_HANDLE_TO_OBJECT(SpriteRenderer, spriteRenderer);
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
    detail::RenderingManager* renderingManager = detail::RenderingManager::instance();
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
    detail::RenderingManager* renderingManager = detail::RenderingManager::instance();
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
// LNObject

LNResult LNObject_Release(LNHandle obj) {
    if (auto m = detail::RuntimeManager::instance()) {
        m->releaseObjectExplicitly(obj);
        return LN_OK;
    }
    else {
        return LN_RUNTIME_UNINITIALIZED;
    }
}

LNResult LNObject_Retain(LNHandle obj) {
    if (auto m = detail::RuntimeManager::instance()) {
        m->retainObjectExplicitly(obj);
        return LN_OK;
    }
    else {
        return LN_RUNTIME_UNINITIALIZED;
    }
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
