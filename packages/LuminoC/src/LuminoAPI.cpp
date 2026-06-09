#include "pch.hpp"
#include <cstdio>
#include <cstring>
#include <unordered_map>
#include <LuminoBase.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/Runtime/ObjectRegistry.hpp>
#include <LuminoCore/Platform/Window.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/Texture2D.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <LuminoC/lumino.h>

#include <LuminoCore/Graphics/TextureLoader.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <stb_image.h>
#include <LuminoCore/Graphics/Mesh.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>
#include <LuminoCore/Graphics/Camera.hpp>
#include <LuminoCore/Graphics/Transform.hpp>
#include <LuminoCore/Graphics/Batch.hpp>

//------------------------------------------------------------------------------
// Internal helpers
//------------------------------------------------------------------------------

namespace {

#ifdef __EMSCRIPTEN__
//--------------------------------------
// Wasm: route LuminoBase logger to stdout so Emscripten forwards it to
// the browser console.log.
//--------------------------------------
void luminoLogToStdout(ln::LogLevel level, const char* file, int line,
                       const char* /*func*/, const char* message) {
    const char* levelStr = "?";
    switch (level) {
        case ln::LogLevel::Trace:   levelStr = "T"; break;
        case ln::LogLevel::Debug:   levelStr = "D"; break;
        case ln::LogLevel::Verbose: levelStr = "V"; break;
        case ln::LogLevel::Info:    levelStr = "I"; break;
        case ln::LogLevel::Warning: levelStr = "W"; break;
        case ln::LogLevel::Error:   levelStr = "E"; break;
        case ln::LogLevel::Fatal:   levelStr = "F"; break;
        default: break;
    }
    std::printf("[Lumino][%s] %s:%d %s\n", levelStr, file ? file : "?", line, message ? message : "");
}

bool s_loggerInstalled = false;

void ensureLoggerInstalled() {
    if (s_loggerInstalled) return;
    ln::Logger::setLevel(ln::LogLevel::Trace);
    ln::Logger::setCallback(&luminoLogToStdout);
    s_loggerInstalled = true;
}
#endif // __EMSCRIPTEN__

//--------------------------------------
// Value-type wrappers for handle management
//--------------------------------------

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

/** Convert LNMatrix pointer to ln::Matrix4x4 (identity if null). */
ln::Matrix4x4 toLnMatrix(const LNMatrix* matrix) {
    if (!matrix) return ln::Matrix4x4::identity();
    ln::Matrix4x4 m;
    std::memcpy(m.m, matrix->m, sizeof(float) * 16);
    return m;
}

//--------------------------------------
// Shared: Object handle helpers
//--------------------------------------

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
    object->addRef();
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

//--------------------------------------
// Shared: RHI helpers
//--------------------------------------

/** LNLoadOp → rhi::LoadOp 変換 */
ln::rhi::LoadOp toRhiLoadOp(LNLoadOp op) {
    switch (op) {
        case LN_LOAD_OP_LOAD:      return ln::rhi::LoadOp::Load;
        case LN_LOAD_OP_DONT_CARE: return ln::rhi::LoadOp::DontCare;
        case LN_LOAD_OP_CLEAR:
        default:                   return ln::rhi::LoadOp::Clear;
    }
}



} // anonymous namespace

//------------------------------------------------------------------------------
// Test
//------------------------------------------------------------------------------

int32_t LNHelloTest(int32_t value) {
    std::printf("Lumino HelloTest: %d\n", value);
    return value;
}

//------------------------------------------------------------------------------
// LNBuildInfo
//------------------------------------------------------------------------------

const char* LNBuildInfo_GetBuildTimestamp() {
    static const char kTimestamp[] = __DATE__ " " __TIME__;
    return kTimestamp;
}

//------------------------------------------------------------------------------
// LNInstance
//------------------------------------------------------------------------------

LNResult LNInstance_Initialize(const LNInstanceInitializeSettings* settings) {
#ifdef __EMSCRIPTEN__
    ensureLoggerInstalled();
    std::printf("LNInstance_Initialize: called\n");
#endif

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
    if (!result) {
#ifdef __EMSCRIPTEN__
        std::printf("LNInstance_Initialize: FAILED\n");
#endif
        return LN_ERROR_UNKNOWN;
    }

#ifdef __EMSCRIPTEN__
    std::printf("LNInstance_Initialize: success\n");
#endif
    return LN_OK;
}

void LNInstance_Terminate() {
#ifdef __EMSCRIPTEN__
    ensureLoggerInstalled();
    std::printf("LNInstance_Terminate: called\n");
#endif
    ln::CoreInstance::terminate();
#ifdef __EMSCRIPTEN__
    std::printf("LNInstance_Terminate: success\n");
#endif
}

//------------------------------------------------------------------------------
// LNObject
//------------------------------------------------------------------------------

LNResult LNObject_Release(LNHandle handle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;
    if (handle == LN_NULL_HANDLE) return LN_ERROR_INVALID_HANDLE;

    if (!instance->objectRegistry()->release(handle)) {
        return LN_ERROR_INVALID_HANDLE;
    }
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNTexture2D
//------------------------------------------------------------------------------

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
#ifdef __EMSCRIPTEN__
    // Wasm ビルドでは GLFW ベースの Window は作成できない。
    (void)title;
    (void)width;
    (void)height;
    if (outHandle) *outHandle = LN_NULL_HANDLE;
    return LN_ERROR_UNKNOWN;
#else
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
#endif
}

LNResult LNWindow_CreateFromCanvas(
    const char* canvasSelector,
    uint32_t width,
    uint32_t height,
    LNHandle* outHandle) {
#ifdef __EMSCRIPTEN__
    if (!canvasSelector || !outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    ln::platform::WindowDesc winDesc;
    winDesc.canvasSelector = canvasSelector;
    winDesc.width = width;
    winDesc.height = height;

    ln::GraphicsContextDesc gfxDesc;
    auto windowResult = ln::platform::PlatformWindow::create(
        instance->graphicsModule(), winDesc, gfxDesc);
    if (!windowResult) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromCreate(windowResult->get());
    return LN_OK;
#else
    // Desktop では canvas ベースの Window は作成できない。
    (void)canvasSelector;
    (void)width;
    (void)height;
    if (outHandle) *outHandle = LN_NULL_HANDLE;
    return LN_ERROR_UNKNOWN;
#endif
}

LNResult LNWindow_ProcessEvents(LNHandle handle, LNBool* outQuit) {
    if (!outQuit) return LN_ERROR_INVALID_ARGUMENT;
    *outQuit = LN_FALSE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = resolveObject<ln::platform::PlatformWindow>(handle);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    *outQuit = obj->processEvents() ? LN_FALSE : LN_TRUE;
    return LN_OK;
}

LNResult LNWindow_GetGraphicsContext(LNHandle window, LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = resolveObject<ln::platform::PlatformWindow>(window);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    ln::GraphicsContext* ctx = obj->graphicsContext();
    if (!ctx) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromGet(ctx);
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNGraphicsContext
//------------------------------------------------------------------------------

LNResult LNGraphicsContext_BeginFrame(
    LNHandle graphicsContext,
    uint32_t width,
    uint32_t height,
    LNHandle* outRenderer,
    LNHandle* outColorBuffer,
    LNHandle* outDepthBuffer) {
    if (!outRenderer || !outColorBuffer || !outDepthBuffer) return LN_ERROR_INVALID_ARGUMENT;
    *outRenderer = LN_NULL_HANDLE;
    *outColorBuffer = LN_NULL_HANDLE;
    *outDepthBuffer = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto frameResult = ctx->beginFrame(width, height);
    if (!frameResult) return LN_ERROR_UNKNOWN;

    ctx->m_currentCmd = ctx->currentCommandBuffer();
    if (!ctx->m_currentCmd) return LN_ERROR_UNKNOWN;

    auto* renderer = ctx->renderer();
    renderer->beginFrame();
    *outRenderer = wrapObjectFromGet(renderer);

    const ln::FramebufferInfo* fb = ctx->currentFramebuffer();
    *outColorBuffer = wrapObjectFromGet(fb->colorTexture.get());
    *outDepthBuffer = wrapObjectFromGet(fb->depthTexture.get());
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNRenderPassDesc
//------------------------------------------------------------------------------

void LNRenderPassDesc_Init(LNRenderPassDesc* desc) {
    if (!desc) return;
    std::memset(desc, 0, sizeof(*desc));
    desc->depthStencil.clearDepth = 1.0f;
    // loadOp フィールドは LN_LOAD_OP_CLEAR == 0 なのでゼロ初期化で OK
}

//------------------------------------------------------------------------------
// LNGraphicsContext (continued)
//------------------------------------------------------------------------------

#ifndef __EMSCRIPTEN__
LNResult LNGraphicsContext_RequestCaptureBackbuffer(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    ctx->requestCaptureBackbuffer();
    return LN_OK;
}

LNResult LNGraphicsContext_CaptureBackbuffer(
    LNHandle graphicsContext,
    const uint8_t** outData,
    int32_t* outWidth,
    int32_t* outHeight) {
    if (!outData || !outWidth || !outHeight) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
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
#endif // !__EMSCRIPTEN__

LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;
    if (graphicsContext == LN_NULL_HANDLE) return LN_ERROR_INVALID_HANDLE;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    if (!ctx->m_currentCmd) return LN_ERROR_INVALID_HANDLE;

    // 1. Render DebugPrint overlay (records commands into the open command buffer).
#if !defined(__EMSCRIPTEN__)
    auto* dp = ctx->debugPrint();
    if (dp) {
        (void)dp->render(ctx); // ignore error; best-effort overlay
    }
#endif

    // 2. Submit all recorded GPU commands.
    ctx->renderer()->endFrame();
    ctx->m_currentCmd = nullptr;

    // 3. Present + update FPS stats.
    ctx->endFrame();
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNDebug
//------------------------------------------------------------------------------

#ifndef __EMSCRIPTEN__
LNResult LNDebug_Print(LNHandle graphicsContext, const char* str) {
    if (!str) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    ctx->debugPrintText(str);
    return LN_OK;
}

LNResult LNDebug_GetGraphicsProfiler(LNHandle graphicsContext, LNGraphicsProfilering* outProfiler) {
    if (!outProfiler) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
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

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
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
#endif // !__EMSCRIPTEN__

LNResult LNTexture2D_LoadFromMemory(
    LNHandle graphicsContext,
    const void* data,
    uint32_t size,
    LNHandle* outHandle) {
    if (!outHandle || !data || size == 0) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto texResult = ln::TextureLoader::loadFromMemory(
        ctx->device(), static_cast<const uint8_t*>(data), static_cast<size_t>(size));
    if (!texResult) return LN_ERROR_UNKNOWN;

    auto rhiTexture = std::move(*texResult);
    auto texture = ln::Ref<ln::Texture>::adopt(
        LN_NEW ln::Texture(std::move(rhiTexture), 0, 0));
    *outHandle = wrapObjectFromCreate(texture.get());
    return LN_OK;
}

LNResult LNTexture2D_CreateFromPixels(
    LNHandle graphicsContext,
    uint32_t width,
    uint32_t height,
    uint32_t format,
    const void* pixelData,
    uint32_t dataSizeBytes,
    LNHandle* outHandle) {
    if (!outHandle || !pixelData || dataSizeBytes == 0) return LN_ERROR_INVALID_ARGUMENT;
    if (width == 0 || height == 0) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    ln::rhi::TextureDesc texDesc;
    texDesc.width = width;
    texDesc.height = height;
    texDesc.format = static_cast<ln::rhi::TextureFormat>(format);
    texDesc.usage = ln::rhi::TextureUsage::Sampled | ln::rhi::TextureUsage::CopyDst;
    texDesc.initialData = pixelData;

    auto texResult = ctx->device()->createTexture(texDesc);
    if (!texResult) return LN_ERROR_UNKNOWN;

    auto rhiTexture = std::move(*texResult);
    auto texture = ln::Ref<ln::Texture>::adopt(
        LN_NEW ln::Texture(std::move(rhiTexture),
            static_cast<int>(width), static_cast<int>(height)));
    *outHandle = wrapObjectFromCreate(texture.get());
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNImage
//------------------------------------------------------------------------------

LNResult LNImage_DecodeFromMemory(
    const void* data,
    uint32_t size,
    uint32_t* outWidth,
    uint32_t* outHeight,
    const void** outPixels,
    uint32_t* outPixelsSize) {
    if (!data || size == 0) return LN_ERROR_INVALID_ARGUMENT;
    if (!outWidth || !outHeight || !outPixels || !outPixelsSize)
        return LN_ERROR_INVALID_ARGUMENT;

    int w = 0, h = 0, channels = 0;
    // RGBA8 (4チャンネル) に強制変換してデコード
    uint8_t* pixels = stbi_load_from_memory(
        static_cast<const stbi_uc*>(data),
        static_cast<int>(size),
        &w, &h, &channels, 4);
    if (!pixels) return LN_ERROR_UNKNOWN;

    *outWidth = static_cast<uint32_t>(w);
    *outHeight = static_cast<uint32_t>(h);
    *outPixels = pixels;
    *outPixelsSize = static_cast<uint32_t>(w) * static_cast<uint32_t>(h) * 4;
    return LN_OK;
}

LNResult LNImage_FreePixels(const void* pixels) {
    if (!pixels) return LN_ERROR_INVALID_ARGUMENT;
    stbi_image_free(const_cast<void*>(pixels));
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

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
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

LNResult LNMaterial_CreateUnlit(LNHandle graphicsContext, LNHandle* outHandle) {
    return LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, outHandle);
}

LNResult LNMaterial_CreateFromCompiledShader(LNHandle graphicsContext, const void* data, uint32_t size, LNHandle* outHandle) {
    if (!data || !outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto matResult = ln::MaterialFactory::createFromCompiledShader(ctx, data, static_cast<size_t>(size));
    if (!matResult) return LN_ERROR_UNKNOWN;
    *outHandle = wrapObjectFromCreate(matResult->get());
    return LN_OK;
}

LNResult LNMaterial_CreateFromShaderSourceFile(LNHandle graphicsContext, const char* shaderFilePath, const char* searchPathOrNull, LNHandle* outHandle) {
    if (!shaderFilePath || !outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto matResult = ln::MaterialFactory::createFromShaderSourceFile(
        ctx,
        shaderFilePath,
        searchPathOrNull ? searchPathOrNull : "");
    if (!matResult) return LN_ERROR_UNKNOWN;
    *outHandle = wrapObjectFromCreate(matResult->get());
    return LN_OK;
}

LNResult LNMaterial_SetColor(LNHandle material, float r, float g, float b, float a) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    mat->setColor(ln::Color{r, g, b, a});
    return LN_OK;
}

LNResult LNMaterial_SetMainTexture(LNHandle material, LNHandle texture) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    auto* tex = resolveObject<ln::Texture>(texture);
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

LNResult LNMaterial_SetBlendMode(LNHandle material, LNBlendMode blendMode) {
    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    mat->setBlendMode(static_cast<ln::BlendMode>(blendMode));
    return LN_OK;
}

LNResult LNMaterial_SetCullMode(LNHandle material, LNCullMode cullMode) {
    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    mat->setCullMode(static_cast<ln::rhi::CullMode>(cullMode));
    return LN_OK;
}

LNResult LNMaterial_SetDepthTestEnabled(LNHandle material, LNBool enabled) {
    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    mat->setDepthTestEnabled(enabled != LN_FALSE);
    return LN_OK;
}

LNResult LNMaterial_SetDepthWriteEnabled(LNHandle material, LNBool enabled) {
    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    mat->setDepthWriteEnabled(enabled != LN_FALSE);
    return LN_OK;
}

LNResult LNMaterial_SetNamedTexture(LNHandle material, const char* name, LNHandle texture) {
    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;
    if (!name) return LN_ERROR_INVALID_ARGUMENT;

    auto* tex = resolveObject<ln::Texture>(texture);
    if (!tex) return LN_ERROR_INVALID_HANDLE;

    mat->setNamedTexture(name, tex->rhiTexture());
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

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
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
    std::vector<uint32_t> idx(indices, indices + indexCount);

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

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
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

    auto* mesh = resolveObject<ln::Mesh>(meshHandle);
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

    auto* mesh = resolveObject<ln::Mesh>(meshHandle);
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

    auto* mesh = resolveObject<ln::Mesh>(meshHandle);
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

    auto* mesh = resolveObject<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto* mat = resolveObject<ln::Material>(materialHandle);
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

    auto* camObj = resolveObject<CameraObject>(camera);
    if (!camObj) return LN_ERROR_INVALID_HANDLE;

    camObj->camera.setPerspective(fovY, aspect, nearClip, farClip);
    return LN_OK;
}

LNResult LNCamera_SetOrthographic(
    LNHandle camera, float width, float height, float nearClip, float farClip) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* camObj = resolveObject<CameraObject>(camera);
    if (!camObj) return LN_ERROR_INVALID_HANDLE;

    camObj->camera.setOrthographic(width, height, nearClip, farClip);
    return LN_OK;
}

LNResult LNCamera_SetOrthographic2D(
    LNHandle camera, float width, float height, float nearClip, float farClip,
    float pivotX, float pivotY) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* camObj = resolveObject<CameraObject>(camera);
    if (!camObj) return LN_ERROR_INVALID_HANDLE;

    camObj->camera.setOrthographic2D(width, height, nearClip, farClip, pivotX, pivotY);
    return LN_OK;
}

LNResult LNCamera_SetLookAt(
    LNHandle camera,
    float eyeX, float eyeY, float eyeZ,
    float targetX, float targetY, float targetZ,
    float upX, float upY, float upZ) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* camObj = resolveObject<CameraObject>(camera);
    if (!camObj) return LN_ERROR_INVALID_HANDLE;

    camObj->camera.setLookAt(
        ln::Vector3{eyeX, eyeY, eyeZ},
        ln::Vector3{targetX, targetY, targetZ},
        ln::Vector3{upX, upY, upZ});
    return LN_OK;
}

LNResult LNCamera_SetMatrices(
    LNHandle camera, const float* viewMatrix, const float* projMatrix) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* camObj = resolveObject<CameraObject>(camera);
    if (!camObj) return LN_ERROR_INVALID_HANDLE;

    if (!viewMatrix || !projMatrix) return LN_ERROR_INVALID_ARGUMENT;

    ln::Matrix4x4 view, proj;
    std::memcpy(view.m, viewMatrix, sizeof(float) * 16);
    std::memcpy(proj.m, projMatrix, sizeof(float) * 16);
    camObj->camera.setViewMatrix(view);
    camObj->camera.setProjectionMatrix(proj);
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNRenderer
//------------------------------------------------------------------------------

LNResult LNRenderer_BeginRenderPass(
    LNHandle renderer, LNHandle graphicsContext,
    const LNRenderPassDesc* desc, LNHandle camera) {
    if (!desc) return LN_ERROR_INVALID_ARGUMENT;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = resolveObject<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    const ln::FramebufferInfo* fb = ctx->currentFramebuffer();

    // カラーアタッチメント
    ln::rhi::RenderPassDesc rpDesc;
    ln::Texture* firstRTTexture = nullptr;
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
                auto* tex = resolveObject<ln::Texture>(desc->colorAttachments[i].renderTarget);
                if (!tex) return LN_ERROR_INVALID_HANDLE;
                colorAttach.view = tex->rhiTextureView();
                if (i == 0) firstRTTexture = tex;
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
        auto* tex = resolveObject<ln::Texture>(desc->depthStencil.depthBuffer);
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

    std::string shaderPassName =
        (desc->shaderPassName && desc->shaderPassName[0] != '\0')
            ? std::string(desc->shaderPassName)
            : std::string();

    if (camera != LN_NULL_HANDLE) {
        auto* camObj = resolveObject<CameraObject>(camera);
        if (!camObj) return LN_ERROR_INVALID_HANDLE;
        ren->beginRenderPass(rpDesc, camObj->camera, shaderPassName);
    } else {
        ren->beginRenderPass(rpDesc, shaderPassName);
    }
    return LN_OK;
}

LNResult LNRenderer_EndRenderPass(LNHandle renderer) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* r = resolveObject<ln::Renderer>(renderer);
    if (!r) return LN_ERROR_INVALID_HANDLE;
    r->endRenderPass();
    return LN_OK;
}

LNResult LNRenderer_DrawMesh(
    LNHandle renderer, LNHandle meshHandle, const LNTransform* transform,
    int32_t zIndex) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = resolveObject<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = resolveObject<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    ren->drawMesh(mesh, toLnTransform(transform), zIndex);
    return LN_OK;
}

LNResult LNRenderer_DrawMeshImmediate(
    LNHandle renderer, LNHandle meshHandle, const LNTransform* transform) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = resolveObject<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = resolveObject<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto result = ren->drawMeshImmediate(mesh, toLnTransform(transform));
    if (!result) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNRenderer_DrawMeshImmediateWithMaterial(
    LNHandle renderer, LNHandle meshHandle, const LNTransform* transform, LNHandle materialHandle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = resolveObject<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = resolveObject<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto* mat = resolveObject<ln::Material>(materialHandle);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    auto result = ren->drawMeshImmediate(mesh, toLnTransform(transform), mat);
    if (!result) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNRenderer_DrawScreenRect(LNHandle renderer, LNHandle materialHandle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = resolveObject<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mat = resolveObject<ln::Material>(materialHandle);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    auto result = ren->drawScreenRect(mat);
    if (!result) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNRenderer_DrawSprite(
    LNHandle renderer, LNHandle material, int32_t zIndex,
    const LNMatrix* transform,
    float offsetX, float offsetY,
    float sizeW, float sizeH,
    float pivotX, float pivotY,
    float uvX, float uvY, float uvW, float uvH,
    float colorR, float colorG, float colorB, float colorA) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = resolveObject<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mat = resolveObject<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    ren->drawSprite(
        mat, zIndex,
        toLnMatrix(transform),
        ln::Vector2{offsetX, offsetY},
        ln::Vector2{sizeW, sizeH},
        ln::Vector2{pivotX, pivotY},
        ln::Vector2{uvX, uvY},
        ln::Vector2{uvW, uvH},
        ln::Color{colorR, colorG, colorB, colorA});
    return LN_OK;
}

LNResult LNRenderer_PushStencilMask(
    LNHandle renderer, LNHandle meshHandle,
    const LNTransform* transform, LNHandle materialHandle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = resolveObject<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = resolveObject<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto* mat = resolveObject<ln::Material>(materialHandle);
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

    auto* ren = resolveObject<ln::Renderer>(renderer);
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

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto rtResult = ln::Texture::createRenderTarget(ctx->device(), width, height);
    if (!rtResult) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromCreate(rtResult->get());
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNTexture2D_CreateRenderTargetEx
//------------------------------------------------------------------------------

LNResult LNTexture2D_CreateRenderTargetEx(
    LNHandle graphicsContext,
    uint32_t width,
    uint32_t height,
    uint32_t format,
    LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    if (width == 0 || height == 0) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto rtResult = ln::Texture::createRenderTarget(
        ctx->device(), width, height, static_cast<ln::rhi::TextureFormat>(format));
    if (!rtResult) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromCreate(rtResult->get());
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNTexture2D_CreateDepthStencil
//------------------------------------------------------------------------------

LNResult LNTexture2D_CreateDepthStencil(
    LNHandle graphicsContext,
    uint32_t width,
    uint32_t height,
    LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    if (width == 0 || height == 0) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = resolveObject<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto dsResult = ln::Texture::createDepthStencil(ctx->device(), width, height);
    if (!dsResult) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromCreate(dsResult->get());
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

    // deprecated API: position + Z 回転を行列に畳み込む (world = T * Rz)。
    ln::Matrix4x4 xform =
        ln::Matrix4x4::translate(ln::Vector3{posX, posY, posZ}) *
        ln::Matrix4x4::rotateZ(rotation);
    obj->buffer.drawSprite(
        mat, zIndex,
        xform,
        ln::Vector2{0.0f, 0.0f}, // deprecated API: offset は使わない (位置は xform に含む)
        ln::Vector2{sizeW, sizeH},
        ln::Vector2{0.5f, 0.5f}, // deprecated API: 従来どおり中央原点
        ln::Vector2{uvX, uvY},
        ln::Vector2{uvW, uvH},
        ln::Color{colorR, colorG, colorB, colorA});
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
        // deprecated API: position + Z 回転を行列に畳み込む (world = T * Rz)。
        ln::Matrix4x4 xform =
            ln::Matrix4x4::translate(ln::Vector3{s.posX, s.posY, s.posZ}) *
            ln::Matrix4x4::rotateZ(s.rotation);
        obj->buffer.drawSprite(
            mat, s.zIndex,
            xform,
            ln::Vector2{0.0f, 0.0f}, // deprecated API: offset は使わない (位置は xform に含む)
            ln::Vector2{s.sizeW, s.sizeH},
            ln::Vector2{0.5f, 0.5f}, // deprecated API: 従来どおり中央原点
            ln::Vector2{s.uvX, s.uvY},
            ln::Vector2{s.uvW, s.uvH},
            ln::Color{s.colorR, s.colorG, s.colorB, s.colorA});
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
