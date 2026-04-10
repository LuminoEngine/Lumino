#include "pch.hpp"
#include <unordered_map>
#include <LuminoBase.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/Runtime/ObjectRegistry.hpp>
#include <LuminoCore/Graphics/Texture2D.hpp>
#include <LuminoCore/Graphics/TextureLoader.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/Graphics/Mesh.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>
#include <LuminoCore/Graphics/Camera.hpp>
#include <LuminoCore/Graphics/Transform.hpp>
#include <LuminoCore/Graphics/Batch.hpp>
#include <LuminoCore/Platform/Window.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoC/lumino.h>
#include <cstring>

//------------------------------------------------------------------------------
// Internal wrapper classes for value types that need handle management
//------------------------------------------------------------------------------

namespace {

/** Camera は値型なので Object でラップしてハンドル管理に載せる。 */
class CameraObject : public ln::Object {
public:
    ln::Camera camera;
};

/** DrawCommandBuffer を Object でラップしてハンドル管理に載せる。 */
class DrawCommandBufferObject : public ln::Object {
public:
    ln::DrawCommandBuffer buffer;
};

/** BatchProcessor を Object でラップしてハンドル管理に載せる。 */
class BatchProcessorObject : public ln::Object {
public:
    std::unique_ptr<ln::BatchProcessor> processor;
};

/** Convert LNTransform pointer to ln::Transform (identity if null). */
ln::Transform toLnTransform(const LNTransform* transform) {
    ln::Transform xform;
    if (transform) {
        xform.position = {transform->posX, transform->posY, transform->posZ};
        xform.rotation = ln::Quaternion{transform->rotX, transform->rotY, transform->rotZ, transform->rotW};
        xform.scale    = {transform->scaleX, transform->scaleY, transform->scaleZ};
    }
    return xform;
}

/**
 * Object が未登録の場合、登録して LNHandle を返します。
 * 登録されている場合は、既存の LNHandle を返します。
 * 
 * この関数は Object の所有権をクライアントに渡す時に使います。
 * クライアントは LNObject_Release を呼び出してオブジェクトを解放する責任があります。
 */
LNHandle wrapObjectFromCreate(ln::Object* object) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_NULL_HANDLE;
    LNHandle handle = instance->objectRegistry()->registerObject(object);
    object->addRef(); // 登録後に参照カウントを増やす
    return handle;
}

/**
 * Object が未登録の場合、登録して LNHandle を返します。
 * 登録されている場合は、既存の LNHandle を返します。
 * 
 * この関数は Object の所有権をクライアントに渡さない時に使います。
 * 例えば GraphicsContext が内部で所有している Renderer 等のオブジェクトをクライアントに渡す場合などです。
 */
LNHandle wrapObjectFromGet(ln::Object* object) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_NULL_HANDLE;
    return instance->objectRegistry()->registerObject(object);
}

template<typename T, typename H>
T* resolveObject(H handle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return nullptr;
    return instance->objectRegistry()->resolve<T>(handle);
}

} // anonymous namespace

//------------------------------------------------------------------------------
// LNInstance
//------------------------------------------------------------------------------

LNResult LNInstance_Initialize(const LNInstanceInitializeSettings* settings) {
    ln::CoreInstance::Settings s = {};
    if (settings) {
        s.enableValidation = settings->enableValidation;
        switch (settings->preferredBackend) {
            case LN_GRAPHICS_BACKEND_VULKAN:
                s.preferredBackend = ln::rhi::Backend::Vulkan;
                break;
            case LN_GRAPHICS_BACKEND_WEBGPU:
                s.preferredBackend = ln::rhi::Backend::WebGPU;
                break;
            case LN_GRAPHICS_BACKEND_DEFAULT:
            default:
#ifdef __EMSCRIPTEN__
                s.preferredBackend = ln::rhi::Backend::WebGPU;
#else
                s.preferredBackend = ln::rhi::Backend::Vulkan;
#endif
                break;
        }
    }
    auto result = ln::CoreInstance::initialize(s);
    if (!result) return LN_ERROR_UNKNOWN;
    return LN_OK;
}

void LNInstance_Terminate() {
    ln::CoreInstance::terminate();
}

LNResult LNObject_Release(LNHandle handle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;
    if (handle == LN_NULL_HANDLE) return LN_ERROR_INVALID_HANDLE;

    if (!instance->objectRegistry()->release(handle)) {
        return LN_ERROR_INVALID_HANDLE;
    }
    return LN_OK;
}

LNResult LNTexture2D_Create(
    uint32_t width,
    uint32_t height,
    uint32_t format,
    LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto texture = ln::Ref<ln::Texture>::adopt(LN_NEW ln::Texture(
        width,
        height,
        static_cast<ln::rhi::TextureFormat>(format)));

    *outHandle = wrapObjectFromCreate(texture.get());
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNWindow
//------------------------------------------------------------------------------

LNResult LNWindow_Create(
    const char* title,
    uint32_t width,
    uint32_t height,
    LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    ln::platform::WindowDesc winDesc;
    winDesc.title = title ? title : "Lumino";
    winDesc.width = width;
    winDesc.height = height;

    ln::GraphicsContextDesc gfxDesc;
    auto windowResult = ln::platform::PlatformWindow::create(instance->graphicsModule(),
        winDesc,
        gfxDesc);
    if (!windowResult) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromCreate(windowResult->get());
    return LN_OK;
}

LNResult LNWindow_ProcessEvents(LNHandle handle, LNBool* outQuit) {
    if (!outQuit) return LN_ERROR_INVALID_ARGUMENT;
    *outQuit = LN_FALSE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<ln::platform::PlatformWindow>(handle);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    *outQuit = obj->processEvents() ? LN_FALSE : LN_TRUE;
    return LN_OK;
}

LNResult LNWindow_GetGraphicsContext(LNHandle window, LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<ln::platform::PlatformWindow>(window);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    ln::GraphicsContext* ctx = obj->graphicsContext();
    if (!ctx) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromGet(ctx);
    return LN_OK;
}

/** LNLoadOp → rhi::LoadOp 変換 */
static ln::rhi::LoadOp toRhiLoadOp(LNLoadOp op) {
    switch (op) {
        case LN_LOAD_OP_LOAD:      return ln::rhi::LoadOp::Load;
        case LN_LOAD_OP_DONT_CARE: return ln::rhi::LoadOp::DontCare;
        case LN_LOAD_OP_CLEAR:
        default:                   return ln::rhi::LoadOp::Clear;
    }
}

//------------------------------------------------------------------------------
// LNHandle
//------------------------------------------------------------------------------

LNResult LNGraphicsContext_BeginFrame(
    LNHandle graphicsContext,
    LNHandle* outRenderer,
    LNHandle* outColorBuffer,
    LNHandle* outDepthBuffer) {
    if (!outRenderer || !outColorBuffer || !outDepthBuffer) return LN_ERROR_INVALID_ARGUMENT;
    *outRenderer = LN_NULL_HANDLE;
    *outColorBuffer = LN_NULL_HANDLE;
    *outDepthBuffer = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto frameResult = ctx->beginFrame();
    if (!frameResult) return LN_ERROR_UNKNOWN;

    ctx->m_currentCmd = ctx->currentCommandBuffer();
    if (!ctx->m_currentCmd) return LN_ERROR_UNKNOWN;

    // Renderer ハンドルをキャッシュから取得、または初回登録する
    auto* renderer = ctx->renderer();
    renderer->beginFrame();

    *outRenderer = wrapObjectFromGet(renderer);

    // バックバッファ・デプスバッファのハンドルを返す
    const ln::FramebufferInfo* fb = ctx->currentFramebuffer();
    *outColorBuffer = wrapObjectFromGet(fb->colorTexture.get());
    *outDepthBuffer = wrapObjectFromGet(fb->depthTexture.get());
    return LN_OK;
}

LNResult LNGraphicsContext_BeginRenderPass(
    LNHandle graphicsContext, const LNRenderPassDesc* desc) {
    if (!desc) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx || !ctx->m_currentCmd) return LN_ERROR_INVALID_HANDLE;

    const ln::FramebufferInfo* fb = ctx->currentFramebuffer();

    // カラーアタッチメント
    ln::rhi::RenderPassDesc rpDesc;
    if (desc->colorAttachmentCount == 0) {
        // count が 0 の場合、バックバッファをフォールバック
        ln::rhi::ColorAttachment colorAttach;
        colorAttach.view = fb->colorTexture->rhiTextureView();
        colorAttach.loadOp = toRhiLoadOp(desc->colorAttachments[0].loadOp);
        colorAttach.storeOp = ln::rhi::StoreOp::Store;
        colorAttach.clearColor = {
            desc->colorAttachments[0].clearColor[0],
            desc->colorAttachments[0].clearColor[1],
            desc->colorAttachments[0].clearColor[2],
            desc->colorAttachments[0].clearColor[3]};
        rpDesc.colorAttachments.push_back(colorAttach);
    } else {
        for (uint32_t i = 0; i < desc->colorAttachmentCount; ++i) {
            ln::rhi::ColorAttachment colorAttach;
            if (desc->colorAttachments[i].renderTarget != LN_NULL_HANDLE) {
                auto* tex = instance->objectRegistry()->resolve<ln::Texture>(desc->colorAttachments[i].renderTarget);
                if (!tex) return LN_ERROR_INVALID_HANDLE;
                colorAttach.view = tex->rhiTextureView();
            } else {
                colorAttach.view = fb->colorTexture->rhiTextureView();
            }
            colorAttach.loadOp = toRhiLoadOp(desc->colorAttachments[i].loadOp);
            colorAttach.storeOp = ln::rhi::StoreOp::Store;
            colorAttach.clearColor = {
                desc->colorAttachments[i].clearColor[0],
                desc->colorAttachments[i].clearColor[1],
                desc->colorAttachments[i].clearColor[2],
                desc->colorAttachments[i].clearColor[3]};
            rpDesc.colorAttachments.push_back(colorAttach);
        }
    }

    // デプス・ステンシルアタッチメント
    ln::rhi::DepthStencilAttachment depthAttach;
    if (desc->depthStencil.depthBuffer != LN_NULL_HANDLE) {
        auto* tex = instance->objectRegistry()->resolve<ln::Texture>(desc->depthStencil.depthBuffer);
        if (!tex) return LN_ERROR_INVALID_HANDLE;
        depthAttach.view = tex->rhiTextureView();
    } else {
        depthAttach.view = fb->depthTexture->rhiTextureView();
    }
    depthAttach.depthLoadOp = toRhiLoadOp(desc->depthStencil.depthLoadOp);
    depthAttach.depthStoreOp = ln::rhi::StoreOp::Store;
    depthAttach.clearDepth = desc->depthStencil.clearDepth;
    depthAttach.stencilLoadOp = toRhiLoadOp(desc->depthStencil.stencilLoadOp);
    depthAttach.stencilStoreOp = ln::rhi::StoreOp::Store;
    depthAttach.clearStencil = desc->depthStencil.clearStencil;

    rpDesc.depthStencilAttachment = &depthAttach;

    ctx->m_currentPass = ctx->m_currentCmd->beginRenderPass(rpDesc);
    if (!ctx->m_currentPass) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNGraphicsContext_EndRenderPass(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx || !ctx->m_currentPass) return LN_ERROR_INVALID_HANDLE;

    ctx->m_currentPass->end();
    ctx->m_currentPass = nullptr;

    return LN_OK;
}

//------------------------------------------------------------------------------
// LNRenderPassDesc / LNHandle 関数
//------------------------------------------------------------------------------

void LNRenderPassDesc_Init(LNRenderPassDesc* desc) {
    if (!desc) return;
    memset(desc, 0, sizeof(*desc));
    desc->depthStencil.clearDepth = 1.0f;
    // loadOp フィールドは LN_LOAD_OP_CLEAR == 0 なのでゼロ初期化で OK
}

LNResult LNGraphicsContext_CaptureBackbuffer(
    LNHandle graphicsContext,
    const uint8_t** outData,
    int32_t* outWidth,
    int32_t* outHeight) {
    if (!outData || !outWidth || !outHeight) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto result = ctx->captureBackbuffer();
    if (!result) return LN_ERROR_UNKNOWN;

    const auto& pixels = ctx->captureBuffer();
    *outData = pixels.data();
    *outWidth = static_cast<int32_t>(ctx->width());
    *outHeight = static_cast<int32_t>(ctx->height());
    return LN_OK;
}

LUMINO_API LNResult LNGraphicsContext_WaitIdle(LNHandle graphicsContext) {
    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;
    ctx->waitIdle();
    return LN_OK;

}

LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;
    if (graphicsContext == LN_NULL_HANDLE) return LN_ERROR_INVALID_HANDLE;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx || !ctx->m_currentCmd) return LN_ERROR_INVALID_HANDLE;

    // 1. Render DebugPrint overlay (records commands into the open command buffer).
    auto* dp = ctx->debugPrint();
    if (dp) {
        (void)dp->render(ctx); // ignore error; best-effort overlay
    }

    // 2. Submit all recorded GPU commands.
    // Transition the swapchain image to PRESENT_SRC_KHR before submitting.
    ctx->renderer()->endFrame();
    ctx->m_currentCmd = nullptr;

    // 3. Present + update FPS stats.
    ctx->endFrame();
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNDebug
//------------------------------------------------------------------------------

LNResult LNDebug_Print(LNHandle graphicsContext, const char* str) {
    if (!str) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    ctx->debugPrintText(str);
    return LN_OK;
}

LNResult LNDebug_GetGraphicsProfiler(LNHandle graphicsContext, LNGraphicsProfilering* outProfiler) {
    if (!outProfiler) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    outProfiler->drawCallCount   = static_cast<int32_t>(ctx->renderer()->drawCallCount());
    outProfiler->fps             = ctx->fps();
    outProfiler->lastFrameTimeMs = ctx->lastFrameTimeMs();
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNTexture2D (file loading)
//------------------------------------------------------------------------------

LNResult LNTexture2D_LoadFromFile(
    LNHandle graphicsContext,
    const char* filePath,
    LNHandle* outHandle) {
    if (!outHandle || !filePath) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto texResult = ln::TextureLoader::loadFromFile(ctx->device(), filePath);
    if (!texResult) return LN_ERROR_UNKNOWN;

    auto rhiTexture = std::move(*texResult);
    // TODO: extract actual width/height from rhi::Texture if accessor is available
    auto texture = ln::Ref<ln::Texture>::adopt(
        LN_NEW ln::Texture(std::move(rhiTexture), 0, 0));
    *outHandle = wrapObjectFromCreate(texture.get());
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNMaterial
//------------------------------------------------------------------------------

LNResult LNMaterial_CreateFromBuiltinShader(LNHandle graphicsContext, LNBuiltinShader shader, LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    ln::Result<ln::Ref<ln::Material>> matResult;
    switch (shader) {
    case LN_BUILTIN_SHADER_UNLIT:
        matResult = ln::MaterialFactory::createUnlit(ctx);
        break;
    case LN_BUILTIN_SHADER_BASIC_LIT:
        matResult = ln::MaterialFactory::createBasicLit(ctx);
        break;
    case LN_BUILTIN_SHADER_STENCIL_MASK:
        matResult = ln::MaterialFactory::createStencilMask(ctx);
        break;
    default:
        return LN_ERROR_INVALID_ARGUMENT;
    }
    if (!matResult) return LN_ERROR_UNKNOWN;
    *outHandle = wrapObjectFromCreate(matResult->get());
    return LN_OK;
}

//LNResult LNMaterial_CreateFromShaderSourceFile(
//    LNHandle graphicsContext,
//    const char* shaderFilePath,
//    const char* searchPathOrNull,
//    LNHandle* outHandle) {
//#ifndef LUMINO_USE_SLANG
//    return LN_ERROR_NOT_SUPPORTED;
//#else
//    if (!shaderFilePath || !outHandle) return LN_ERROR_INVALID_ARGUMENT;
//    *outHandle = LN_NULL_HANDLE;
//
//    auto* instance = ln::CoreInstance::instance();
//    if (!instance) return LN_RUNTIME_UNINITIALIZED;
//
//    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
//    if (!ctx) return LN_ERROR_INVALID_HANDLE;
//
//    auto compilerResult = ln::shader::ShaderCompiler2::create();
//    if (!compilerResult) return LN_ERROR_UNKNOWN;
//    auto& compiler = *compilerResult;
//
//    if (searchPath) {
//        compiler->addSearchPath(searchPath);
//    }
//
//    auto buildResult = compiler->build(shaderFilePath);
//    if (!buildResult) return LN_ERROR_UNKNOWN;
//
//    auto memResult = ln::shader::UnifiedShaderSerializer2::saveToMemory(compiler->shader());
//    if (!memResult) return LN_ERROR_UNKNOWN;
//    auto& bytes = *memResult;
//
//    auto matResult = ln::MaterialFactory::createFromCompiledShader(ctx, bytes.data(), bytes.size());
//    if (!matResult) return LN_ERROR_UNKNOWN;
//    *outHandle = wrapObjectFromCreate(matResult->get());
//    return LN_OK;
//#endif
//}

LNResult LNMaterial_CreateUnlit(LNHandle graphicsContext, LNHandle* outHandle) {
    return LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, outHandle);
}

LNResult LNMaterial_CreateFromCompiledShader(LNHandle graphicsContext, const void* data, uint32_t size, LNHandle* outHandle) {
    if (!data || !outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto matResult = ln::MaterialFactory::createFromCompiledShader(ctx, data, static_cast<size_t>(size));
    if (!matResult) return LN_ERROR_UNKNOWN;
    *outHandle = wrapObjectFromCreate(matResult->get());
    return LN_OK;
}

LNResult LNMaterial_SetColor(LNHandle material, float r, float g, float b, float a) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mat = instance->objectRegistry()->resolve<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    mat->setColor(ln::Color{r, g, b, a});
    return LN_OK;
}

LNResult LNMaterial_SetMainTexture(LNHandle material, LNHandle texture) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mat = instance->objectRegistry()->resolve<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    auto* tex = instance->objectRegistry()->resolve<ln::Texture>(texture);
    if (!tex) return LN_ERROR_INVALID_HANDLE;

    mat->setTexture(tex->rhiTexture());
    return LN_OK;
}

extern LUMINO_API LNResult LNMaterial_SetFloat4(LNHandle material, const char* name, const float* values) {
    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;
    mat->setFloat4(name, values);
    return LN_OK;

}
//------------------------------------------------------------------------------
// LNMesh
//------------------------------------------------------------------------------

LNResult LNMesh_Create(
    LNHandle graphicsContext,
    const LNVertex* vertices,
    uint32_t vertexCount,
    const uint32_t* indices,
    uint32_t indexCount,
    const LNSubMesh* submeshes,
    uint32_t submeshCount,
    LNHandle* outHandle) {
    if (!outHandle || !vertices || !indices || !submeshes) return LN_ERROR_INVALID_ARGUMENT;
    if (vertexCount == 0 || indexCount == 0 || submeshCount == 0) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    // Convert LNVertex[] → std::vector<ln::Vertex>
    std::vector<ln::Vertex> verts(vertexCount);
    for (uint32_t i = 0; i < vertexCount; i++) {
        const auto& sv = vertices[i];
        auto& dv = verts[i];
        dv.position = {sv.posX, sv.posY, sv.posZ};
        dv.normal   = {sv.normX, sv.normY, sv.normZ};
        dv.uv       = {sv.u, sv.v};
        dv.color    = {sv.colorR, sv.colorG, sv.colorB, sv.colorA};
        dv.tangent  = {sv.tanX, sv.tanY, sv.tanZ, sv.tanW};
    }

    // Convert indices
    std::vector<ln::u32> idx(indices, indices + indexCount);

    // Convert LNSubMesh[] → std::vector<ln::SubMesh>
    std::vector<ln::SubMesh> subs(submeshCount);
    for (uint32_t i = 0; i < submeshCount; i++) {
        subs[i].indexOffset   = submeshes[i].indexOffset;
        subs[i].indexCount    = submeshes[i].indexCount;
        subs[i].materialIndex = submeshes[i].materialIndex;
    }

    auto meshResult = ln::Mesh::create(ctx->device(), verts, idx, subs);
    if (!meshResult) return LN_ERROR_UNKNOWN;
    *outHandle = wrapObjectFromCreate(meshResult->get());
    return LN_OK;
}

LNResult LNMesh_CreateDynamic(
    LNHandle graphicsContext,
    uint32_t maxVertexCount,
    uint32_t maxIndexCount,
    LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    if (maxVertexCount == 0 || maxIndexCount == 0) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto meshResult = ln::Mesh::createDynamic(ctx->device(), maxVertexCount, maxIndexCount);
    if (!meshResult) return LN_ERROR_UNKNOWN;
    *outHandle = wrapObjectFromCreate(meshResult->get());
    return LN_OK;
}

LNResult LNMesh_UpdateVertices(
    LNHandle meshHandle,
    uint32_t firstVertex,
    const LNVertex* vertices,
    uint32_t count) {
    if (!vertices || count == 0) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mesh = instance->objectRegistry()->resolve<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    // Convert LNVertex[] → ln::Vertex[]
    std::vector<ln::Vertex> verts(count);
    for (uint32_t i = 0; i < count; i++) {
        const auto& sv = vertices[i];
        auto& dv = verts[i];
        dv.position = {sv.posX, sv.posY, sv.posZ};
        dv.normal   = {sv.normX, sv.normY, sv.normZ};
        dv.uv       = {sv.u, sv.v};
        dv.color    = {sv.colorR, sv.colorG, sv.colorB, sv.colorA};
        dv.tangent  = {sv.tanX, sv.tanY, sv.tanZ, sv.tanW};
    }

    auto result = mesh->updateVertices(firstVertex, verts.data(), count);
    if (!result) return LN_ERROR_UNKNOWN;
    return LN_OK;
}

LNResult LNMesh_UpdateIndices(
    LNHandle meshHandle,
    uint32_t firstIndex,
    const uint32_t* indices,
    uint32_t count) {
    if (!indices || count == 0) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mesh = instance->objectRegistry()->resolve<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto result = mesh->updateIndices(firstIndex, indices, count);
    if (!result) return LN_ERROR_UNKNOWN;
    return LN_OK;
}

LNResult LNMesh_SetSubMeshes(
    LNHandle meshHandle,
    const LNSubMesh* submeshes,
    uint32_t submeshCount) {
    if (!submeshes || submeshCount == 0) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mesh = instance->objectRegistry()->resolve<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    std::vector<ln::SubMesh> subs(submeshCount);
    for (uint32_t i = 0; i < submeshCount; i++) {
        subs[i].indexOffset   = submeshes[i].indexOffset;
        subs[i].indexCount    = submeshes[i].indexCount;
        subs[i].materialIndex = submeshes[i].materialIndex;
    }
    mesh->setSubmeshes(subs);
    return LN_OK;
}

LNResult LNMesh_SetMaterial(LNHandle meshHandle, uint32_t materialIndex, LNHandle materialHandle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mesh = instance->objectRegistry()->resolve<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto* mat = instance->objectRegistry()->resolve<ln::Material>(materialHandle);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    auto& materials = mesh->materials();
    if (materialIndex >= materials.size()) return LN_ERROR_INVALID_ARGUMENT;

    // addRef because materials() stores Ref<Material>
    mat->addRef();
    materials[materialIndex] = ln::Ref<ln::Material>::adopt(mat);

    return LN_OK;
}

//------------------------------------------------------------------------------
// LNCamera
//------------------------------------------------------------------------------

LNResult LNCamera_Create(LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto camObj = ln::Ref<CameraObject>::adopt(LN_NEW CameraObject());
    *outHandle = wrapObjectFromCreate(camObj.get());
    return LN_OK;
}

LNResult LNCamera_SetPerspective(
    LNHandle camera, float fovY, float aspect, float nearClip, float farClip) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* camObj = instance->objectRegistry()->resolve<CameraObject>(camera);
    if (!camObj) return LN_ERROR_INVALID_HANDLE;

    camObj->camera.setPerspective(fovY, aspect, nearClip, farClip);
    return LN_OK;
}

LNResult LNCamera_SetOrthographic(
    LNHandle camera, float width, float height, float nearClip, float farClip) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* camObj = instance->objectRegistry()->resolve<CameraObject>(camera);
    if (!camObj) return LN_ERROR_INVALID_HANDLE;

    camObj->camera.setOrthographic(width, height, nearClip, farClip);
    return LN_OK;
}

LNResult LNCamera_SetLookAt(
    LNHandle camera,
    float eyeX, float eyeY, float eyeZ,
    float targetX, float targetY, float targetZ,
    float upX, float upY, float upZ) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* camObj = instance->objectRegistry()->resolve<CameraObject>(camera);
    if (!camObj) return LN_ERROR_INVALID_HANDLE;

    camObj->camera.setLookAt(
        ln::Vector3{eyeX, eyeY, eyeZ},
        ln::Vector3{targetX, targetY, targetZ},
        ln::Vector3{upX, upY, upZ});
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNRenderer
//------------------------------------------------------------------------------

LNResult LNRenderer_BeginRenderPass(
    LNHandle renderer, LNHandle graphicsContext,
    LNHandle camera,
    float r, float g, float b, float a) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    const ln::FramebufferInfo* fb = ctx->currentFramebuffer();

    if (camera != LN_NULL_HANDLE) {
        auto* camObj = instance->objectRegistry()->resolve<CameraObject>(camera);
        if (!camObj) return LN_ERROR_INVALID_HANDLE;
        ren->beginRenderPass(
            fb->colorTexture->rhiTextureView(),
            fb->depthTexture->rhiTextureView(),
            camObj->camera,
            ln::Color{r, g, b, a});
    } else {
        ren->beginRenderPass(
            fb->colorTexture->rhiTextureView(),
            fb->depthTexture->rhiTextureView(),
            ln::Color{r, g, b, a});
    }
    return LN_OK;
}

LNResult LNRenderer_EndRenderPass(LNHandle renderer) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* r = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!r) return LN_ERROR_INVALID_HANDLE;

    r->endRenderPass();
    return LN_OK;
}

LNResult LNRenderer_DrawMesh(
    LNHandle renderer, LNHandle meshHandle, const LNTransform* transform,
    int32_t zIndex) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = instance->objectRegistry()->resolve<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    ren->drawMesh(mesh, toLnTransform(transform), zIndex);
    return LN_OK;
}

LNResult LNRenderer_DrawMeshImmediate(
    LNHandle renderer, LNHandle meshHandle, const LNTransform* transform) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = instance->objectRegistry()->resolve<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto result = ren->drawMeshImmediate(mesh, toLnTransform(transform));
    if (!result) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNRenderer_DrawSprite(
    LNHandle renderer, LNHandle material, int32_t zIndex,
    float posX, float posY, float posZ,
    float sizeW, float sizeH,
    float uvX, float uvY, float uvW, float uvH,
    float colorR, float colorG, float colorB, float colorA,
    float rotation) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mat = instance->objectRegistry()->resolve<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    ren->drawSprite(
        mat, zIndex,
        ln::Vector3{posX, posY, posZ},
        ln::Vector2{sizeW, sizeH},
        ln::Vector2{uvX, uvY},
        ln::Vector2{uvW, uvH},
        ln::Color{colorR, colorG, colorB, colorA},
        rotation);
    return LN_OK;
}

//------------------------------------------------------------------------------

LNResult LNRenderer_PushStencilMask(
    LNHandle renderer, LNHandle meshHandle,
    const LNTransform* transform, LNHandle materialHandle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = instance->objectRegistry()->resolve<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto* mat = instance->objectRegistry()->resolve<ln::Material>(materialHandle);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    ln::Transform xform;
    if (transform) {
        xform.position = {transform->posX, transform->posY, transform->posZ};
        xform.rotation = ln::Quaternion{transform->rotX, transform->rotY, transform->rotZ, transform->rotW};
        xform.scale    = {transform->scaleX, transform->scaleY, transform->scaleZ};
    }

    auto result = ren->pushStencilMask(mesh, xform, mat);
    if (!result) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNRenderer_PopStencilMask(LNHandle renderer) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto result = ren->popStencilMask();
    if (!result) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

//------------------------------------------------------------------------------
// LNTexture2D_CreateRenderTarget
//------------------------------------------------------------------------------

LNResult LNTexture2D_CreateRenderTarget(
    LNHandle graphicsContext,
    uint32_t width,
    uint32_t height,
    LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    if (width == 0 || height == 0) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto rtResult = ln::Texture::createRenderTarget(ctx->device(), width, height);
    if (!rtResult) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromCreate(rtResult->get());
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNRenderer_BeginRenderPassToTexture
//------------------------------------------------------------------------------

LNResult LNRenderer_BeginRenderPassToTexture(
    LNHandle renderer,
    LNHandle renderTargetTexture,
    LNHandle camera,
    float r, float g, float b, float a) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* tex = instance->objectRegistry()->resolve<ln::Texture>(renderTargetTexture);
    if (!tex || !tex->isRenderTarget()) return LN_ERROR_INVALID_HANDLE;

    auto* colorView = tex->rhiTextureView();
    auto* depthView = tex->depthView();
    if (!colorView) return LN_ERROR_INVALID_HANDLE;

    if (camera != LN_NULL_HANDLE) {
        auto* camObj = instance->objectRegistry()->resolve<CameraObject>(camera);
        if (!camObj) return LN_ERROR_INVALID_HANDLE;
        ren->beginRenderPass(
            colorView, depthView,
            camObj->camera,
            ln::Color{r, g, b, a});
    } else {
        ren->beginRenderPass(
            colorView, depthView,
            ln::Color{r, g, b, a});
    }
    
    return LN_OK;
}


//------------------------------------------------------------------------------
// LNDrawCommandBuffer
//------------------------------------------------------------------------------

LNResult LNDrawCommandBuffer_Create(LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto obj = ln::Ref<DrawCommandBufferObject>::adopt(LN_NEW DrawCommandBufferObject());
    *outHandle = wrapObjectFromCreate(obj.get());
    return LN_OK;
}

LNResult LNDrawCommandBuffer_Clear(LNHandle buffer) {
    auto* obj = resolveObject<DrawCommandBufferObject>(buffer);
    if (!obj) return LN_ERROR_INVALID_HANDLE;
    obj->buffer.clear();
    return LN_OK;
}

LNResult LNDrawCommandBuffer_DrawSprite(
    LNHandle buffer, LNHandle material, int32_t zIndex,
    float posX, float posY, float posZ,
    float sizeW, float sizeH,
    float uvX, float uvY, float uvW, float uvH,
    float colorR, float colorG, float colorB, float colorA,
    float rotation) {

    auto* obj = resolveObject<DrawCommandBufferObject>(buffer);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    obj->buffer.drawSprite(
        mat, zIndex,
        ln::Vector3{posX, posY, posZ},
        ln::Vector2{sizeW, sizeH},
        ln::Vector2{uvX, uvY},
        ln::Vector2{uvW, uvH},
        ln::Color{colorR, colorG, colorB, colorA},
        rotation);
    return LN_OK;
}

LNResult LNDrawCommandBuffer_DrawSprites(
    LNHandle buffer, LNHandle material,
    const LNSpriteCommand* sprites, uint32_t count) {

    if (!sprites && count > 0) return LN_ERROR_INVALID_ARGUMENT;

    auto* obj = resolveObject<DrawCommandBufferObject>(buffer);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    for (uint32_t i = 0; i < count; ++i) {
        const auto& s = sprites[i];
        obj->buffer.drawSprite(
            mat, s.zIndex,
            ln::Vector3{s.posX, s.posY, s.posZ},
            ln::Vector2{s.sizeW, s.sizeH},
            ln::Vector2{s.uvX, s.uvY},
            ln::Vector2{s.uvW, s.uvH},
            ln::Color{s.colorR, s.colorG, s.colorB, s.colorA},
            s.rotation);
    }
    return LN_OK;
}

LNResult LNDrawCommandBuffer_DrawSubMesh(
    LNHandle buffer, LNHandle meshHandle, uint32_t submeshIndex,
    LNHandle materialHandle, const LNTransform* transform, int32_t zIndex) {

    auto* obj = resolveObject<DrawCommandBufferObject>(buffer);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = resolveObject<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto* mat = resolveObject<ln::Material>(materialHandle);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    obj->buffer.drawSubMesh(mesh, submeshIndex, mat, toLnTransform(transform), zIndex);
    return LN_OK;
}

LNResult LNDrawCommandBuffer_DrawMesh(
    LNHandle buffer, LNHandle meshHandle,
    const LNTransform* transform, int32_t zIndex) {

    auto* obj = resolveObject<DrawCommandBufferObject>(buffer);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = resolveObject<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    obj->buffer.drawMesh(mesh, toLnTransform(transform), zIndex);
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNBatchProcessor
//------------------------------------------------------------------------------

LNResult LNBatchProcessor_Create(LNHandle graphicsContext, LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto result = ln::BatchProcessor::create(ctx);
    if (!result) return LN_ERROR_UNKNOWN;

    auto obj = ln::Ref<BatchProcessorObject>::adopt(LN_NEW BatchProcessorObject());
    obj->processor = std::move(*result);
    *outHandle = wrapObjectFromCreate(obj.get());
    return LN_OK;
}

LNResult LNBatchProcessor_Flush(
    LNHandle batchProcessor, LNHandle renderer, LNHandle commandBuffer) {

    auto* bpObj = resolveObject<BatchProcessorObject>(batchProcessor);
    if (!bpObj) return LN_ERROR_INVALID_HANDLE;

    auto* ren = resolveObject<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* cmdObj = resolveObject<DrawCommandBufferObject>(commandBuffer);
    if (!cmdObj) return LN_ERROR_INVALID_HANDLE;

    auto result = bpObj->processor->flush(ren, &cmdObj->buffer);
    if (!result) return LN_ERROR_UNKNOWN;

    return LN_OK;
}
