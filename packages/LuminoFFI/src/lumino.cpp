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
#include <lumino.h>


#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawCommand.hpp>
#include <LuminoEngine/Rendering/Kanata/KPipelineState.hpp>
#include <LuminoEngine/Rendering/Kanata/KUnlitRenderPass.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KPrimitiveMeshRenderer.hpp>

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

LUMINO_API LNResult LNGraphicsContext_CreateFromOpenGL(int32_t width, int32_t height, LNHandle* outReturn) {
    LN_FFI_TRY_BEGIN;
    ln::OpenGLGraphicsContext::Settings s;
    s.window = nullptr;
    s.width = width;
    s.height = height;
    *outReturn = ::ln::FFI::wrapObject(ln::OpenGLGraphicsContext::create(s), true);
    LN_FFI_TRY_END_RETURN;
}

//LUMINO_API LNResult LNGraphicsContext_Release(LNHandle handle) {
//    LN_SAFE_RELEASE(handle)
//
//}

extern LUMINO_API LNResult LNRenderingContext_Create(LNHandle graphicsContext, LNHandle* outReturn) {
    LN_FFI_TRY_BEGIN;
    printf("LNRenderingContext_Create1 %d\n", graphicsContext);
    ln::GraphicsContext* context = LNI_HANDLE_TO_OBJECT(ln::GraphicsContext, graphicsContext);

    ln::RenderPass* renderPass = context->currentRenderPass();
    ln::GraphicsCommandList* commandList = context->currentCommandList2();

    printf("LNRenderingContext_Create %p %p %p\n", context, renderPass, commandList);
    ;

    renderPass->setClearFlags(ln::ClearFlags::Color);
    renderPass->setClearColor(ln::Color::Red);

    //commandList->beginCommandRecoding();
    //commandList->beginRenderPass(renderPass);
    //commandList->clear(ln::ClearFlags::Color, ln::Color::Red);
    //commandList->endRenderPass();
    //commandList->endCommandRecoding();

#if 1
    {
        using namespace ln;
        URef<kanata::BatchCollector> g_batchList;
        URef<kanata::DrawCommandList> g_drawCommandList;
        URef<kanata::UnlitRenderPass> g_renderPass;
        URef<kanata::BoxMeshBatchProxy> g_boxMeshBatchProxy;
        Ref<VertexBuffer> g_vertexBuffer;

        auto* renderingManager = ln::detail::RenderingManager::instance();
        g_batchList = makeURef<kanata::BatchCollector>(renderingManager);
        g_drawCommandList = makeURef<kanata::DrawCommandList>(renderingManager);
        g_renderPass = makeURef<kanata::UnlitRenderPass>(renderingManager);
        g_boxMeshBatchProxy = makeURef<kanata::BoxMeshBatchProxy>();

        Vertex v[] = {
            Vertex(Vector3(0, 5.5, 0), Vector3(0, 0, 1), Vector2(0, 0), Color::Red),
            Vertex(Vector3(-5.5, 0, 0), Vector3(0, 0, 1), Vector2(1, 0), Color::Red),
            Vertex(Vector3(5.5, 0, 0), Vector3(0, 0, 1), Vector2(0, 1), Color::Red),

        };
        g_vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

        Ref<Material> material = Material::create();
        //Ref<Texture2D> texture = Texture2D::load(U"C:/Proj/LN/Lumino/assets/Distributable/assets/icon256.png");
        //material->setMainTexture(texture);

        //auto* target = TestEnv::mainWindowSwapChain()->currentBackbuffer();
        //auto* renderPass = TestEnv::renderPass();

        detail::RenderViewInfo renderViewInfo;
        //renderViewInfo.cameraInfo.makePerspective(Vector3(10, 10, 10), Vector3::normalize(-1, -1, -1), 0.3, Size(renderPass->width(), renderPass->height()), 0.1, 1000.0);
        detail::SceneInfo sceneInfo;

        printf("size: %d %d\n", renderPass->width(), renderPass->height());

        RenderViewPoint viewPoint;
        viewPoint.resetPerspective(Vector3(10, 10, 10), Vector3::normalize(-1, -1, -1), 0.3, Size(renderPass->width(), renderPass->height()), 0.1, 1000.0);
        viewPoint.makeCameraInfo(&renderViewInfo.cameraInfo);

        kanata::BatchProxyState batchState;
        batchState.reset();
        batchState.m_depthTestEnabled = false;
        batchState.m_cullingMode = CullMode::None;

        // Build batch
        {
            g_batchList->clear(&viewPoint);

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
            //g_boxMeshBatchProxy->getBatch(g_batchList);

            //
            //auto& r = g_batchList->primitiveRenderer();
            //r->begin();
            //r->drawBox(Box(0.5), Color::Red, Matrix::makeTranslation(2, 0, 0));
            //r->drawBox(Box(2), Color::Red, Matrix::makeTranslation(-2, 0, 0));
            //r->end();
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
    }
#endif

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
