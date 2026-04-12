// Phase 1 web wrapper.
// A minimal subset of the LuminoC C-API compiled only for the Emscripten build.
// At Phase 1 it exposes LNInstance_Initialize / LNInstance_Terminate which
// drive the real ln::CoreInstance, so we can verify that the init / terminate
// path (and its log output) flows all the way to the browser console.
//
// Phase 5 adds the rendering C functions (BeginFrame, EndFrame, BeginRenderPass,
// EndRenderPass, RenderPassDesc_Init) and a lightweight WebRenderer object that
// delegates to the existing rhi::CommandBuffer / rhi::RenderPass.
//
// The full LuminoAPI.cpp pulls in GraphicsModule / GraphicsContext / Texture /
// Mesh / ... and cannot be compiled for the web until Phase 2/4. This file is
// intentionally self-contained so we can link it into the wasm executable
// alongside a small slice of LuminoBase + LuminoCore.

#if defined(__EMSCRIPTEN__)

#include <cstdio>
#include <cstring>
#include <LuminoBase.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Runtime/ObjectRegistry.hpp>
#include <LuminoCore/Platform/Window.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/Texture2D.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <LuminoC/lumino.h>

namespace {

// Route LuminoBase's Logger callback to stdout so Emscripten forwards it to
// the browser's console.log (easier to spot than stderr on the web).
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

ln::rhi::Backend toRhiBackend(LNGraphicsBackend b) {
    switch (b) {
        case LN_GRAPHICS_BACKEND_VULKAN: return ln::rhi::Backend::Vulkan;
        case LN_GRAPHICS_BACKEND_WEBGPU: return ln::rhi::Backend::WebGPU;
        case LN_GRAPHICS_BACKEND_DEFAULT:
        default:                         return ln::rhi::Backend::WebGPU;
    }
}

} // anonymous namespace

extern "C" {

LUMINO_API LNResult LNInstance_Initialize(const LNInstanceInitializeSettings* settings) {
    ensureLoggerInstalled();
    std::printf("LNInstance_Initialize: called\n");

    ln::CoreInstance::Settings s = {};
    if (settings) {
        s.enableValidation = settings->enableValidation != LN_FALSE;
        s.preferredBackend = toRhiBackend(settings->preferredBackend);
    } else {
        s.preferredBackend = ln::rhi::Backend::WebGPU;
    }

    auto result = ln::CoreInstance::initialize(s);
    if (!result) {
        std::printf("LNInstance_Initialize: FAILED\n");
        return LN_ERROR_UNKNOWN;
    }
    std::printf("LNInstance_Initialize: success\n");
    return LN_OK;
}

LUMINO_API void LNInstance_Terminate() {
    ensureLoggerInstalled();
    std::printf("LNInstance_Terminate: called\n");
    ln::CoreInstance::terminate();
    std::printf("LNInstance_Terminate: success\n");
}

//------------------------------------------------------------------------------
// Handle helpers (mirrors LuminoAPI.cpp's anonymous namespace)
//------------------------------------------------------------------------------

static LNHandle wrapObjectFromCreate(ln::Object* object) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_NULL_HANDLE;
    LNHandle handle = instance->objectRegistry()->registerObject(object);
    object->addRef();
    return handle;
}

static LNHandle wrapObjectFromGet(ln::Object* object) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_NULL_HANDLE;
    return instance->objectRegistry()->registerObject(object);
}

//------------------------------------------------------------------------------
// LNObject
//------------------------------------------------------------------------------

LUMINO_API LNResult LNObject_Release(LNHandle handle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;
    if (handle == LN_NULL_HANDLE) return LN_ERROR_INVALID_HANDLE;
    if (!instance->objectRegistry()->release(handle)) {
        return LN_ERROR_INVALID_HANDLE;
    }
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNWindow
//------------------------------------------------------------------------------

LUMINO_API LNResult LNWindow_CreateFromCanvas(
    const char* canvasSelector,
    uint32_t width,
    uint32_t height,
    LNHandle* outHandle) {
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
        nullptr, winDesc, gfxDesc);
    if (!windowResult) return LN_ERROR_UNKNOWN;

    *outHandle = wrapObjectFromCreate(windowResult->get());
    return LN_OK;
}

LUMINO_API LNResult LNWindow_Create(
    const char* title,
    uint32_t width,
    uint32_t height,
    LNHandle* outHandle) {
    // Web ビルドでは GLFW ベースの Window は作成できない。
    (void)title;
    (void)width;
    (void)height;
    if (outHandle) *outHandle = LN_NULL_HANDLE;
    return LN_ERROR_UNKNOWN;
}

LUMINO_API LNResult LNWindow_GetGraphicsContext(LNHandle window, LNHandle* outHandle) {
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

LUMINO_API LNResult LNWindow_ProcessEvents(LNHandle handle, LNBool* outQuit) {
    if (!outQuit) return LN_ERROR_INVALID_ARGUMENT;
    *outQuit = LN_FALSE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<ln::platform::PlatformWindow>(handle);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    *outQuit = obj->processEvents() ? LN_FALSE : LN_TRUE;
    return LN_OK;
}

} // extern "C"

//------------------------------------------------------------------------------
// WebRenderer — lightweight render-pass wrapper for the web path
//------------------------------------------------------------------------------
// Desktop uses the full Renderer class (PipelineCache, UBO allocators, batching
// etc.) which is #ifdef'd out on Emscripten.  For the ClearScreen milestone we
// only need begin/end render pass, so this thin Object delegates directly to
// the rhi::CommandBuffer / rhi::RenderPass layer.
//------------------------------------------------------------------------------

namespace {

static ln::rhi::LoadOp toRhiLoadOp(LNLoadOp op) {
    switch (op) {
        case LN_LOAD_OP_LOAD:      return ln::rhi::LoadOp::Load;
        case LN_LOAD_OP_DONT_CARE: return ln::rhi::LoadOp::DontCare;
        case LN_LOAD_OP_CLEAR:
        default:                   return ln::rhi::LoadOp::Clear;
    }
}

class WebRenderer : public ln::Object {
public:
    void bind(ln::GraphicsContext* ctx) {
        m_ctx = ctx;
        m_currentPass = nullptr;
    }

    ln::GraphicsContext* ctx() const { return m_ctx; }

    LNResult beginRenderPass(ln::GraphicsContext* gfxCtx,
                             const LNRenderPassDesc* desc) {
        if (!desc) return LN_ERROR_INVALID_ARGUMENT;

        auto* cmd = gfxCtx->currentCommandBuffer();
        if (!cmd) return LN_ERROR_UNKNOWN;

        auto* instance = ln::CoreInstance::instance();
        if (!instance) return LN_RUNTIME_UNINITIALIZED;

        // Build rhi::RenderPassDesc from the C struct.
        ln::rhi::RenderPassDesc rhiDesc;

        const auto* fb = gfxCtx->currentFramebuffer();
        if (!fb) return LN_ERROR_UNKNOWN;

        uint32_t count = desc->colorAttachmentCount;
        // Default: use the backbuffer if count == 0.
        if (count == 0) count = 1;

        for (uint32_t i = 0; i < count; i++) {
            ln::rhi::ColorAttachment ca;

            if (i < desc->colorAttachmentCount) {
                const auto& src = desc->colorAttachments[i];
                if (src.renderTarget != LN_NULL_HANDLE) {
                    auto* tex = instance->objectRegistry()
                                    ->resolve<ln::Texture>(src.renderTarget);
                    ca.view = tex ? tex->rhiTextureView() : nullptr;
                } else {
                    ca.view = fb->colorTexture
                                  ? fb->colorTexture->rhiTextureView()
                                  : nullptr;
                }
                ca.clearColor = {src.clearColor[0], src.clearColor[1],
                                 src.clearColor[2], src.clearColor[3]};
                ca.loadOp = toRhiLoadOp(src.loadOp);
            } else {
                // Implicit backbuffer attachment when count was 0.
                ca.view = fb->colorTexture
                              ? fb->colorTexture->rhiTextureView()
                              : nullptr;
                ca.loadOp = ln::rhi::LoadOp::Clear;
                ca.clearColor = {0, 0, 0, 1};
            }
            rhiDesc.colorAttachments.push_back(ca);
        }

        // Depth/stencil
        ln::rhi::DepthStencilAttachment dsa;
        if (desc->depthStencil.depthBuffer != LN_NULL_HANDLE) {
            auto* tex = instance->objectRegistry()
                            ->resolve<ln::Texture>(desc->depthStencil.depthBuffer);
            dsa.view = tex ? tex->rhiTextureView() : nullptr;
        } else {
            dsa.view = fb->depthTexture
                           ? fb->depthTexture->rhiTextureView()
                           : nullptr;
        }
        dsa.clearDepth = desc->depthStencil.clearDepth;
        dsa.clearStencil = desc->depthStencil.clearStencil;
        dsa.depthLoadOp = toRhiLoadOp(desc->depthStencil.depthLoadOp);
        dsa.stencilLoadOp = toRhiLoadOp(desc->depthStencil.stencilLoadOp);
        rhiDesc.depthStencilAttachment = &dsa;

        m_currentPass = cmd->beginRenderPass(rhiDesc);
        if (!m_currentPass) return LN_ERROR_UNKNOWN;
        // Store on GraphicsContext for potential future use.
        gfxCtx->m_currentPass = m_currentPass;
        return LN_OK;
    }

    LNResult endRenderPass() {
        if (!m_currentPass) return LN_ERROR_UNKNOWN;
        m_currentPass->end();
        if (m_ctx) m_ctx->m_currentPass = nullptr;
        m_currentPass = nullptr;
        return LN_OK;
    }

private:
    ln::GraphicsContext* m_ctx = nullptr;
    ln::rhi::RenderPass* m_currentPass = nullptr;
};

} // anonymous namespace

extern "C" {

//------------------------------------------------------------------------------
// LNGraphicsContext — frame lifecycle
//------------------------------------------------------------------------------

LUMINO_API LNResult LNGraphicsContext_BeginFrame(
    LNHandle graphicsContext,
    LNHandle* outRenderer,
    LNHandle* outColorBuffer,
    LNHandle* outDepthBuffer) {
    if (!outRenderer || !outColorBuffer || !outDepthBuffer)
        return LN_ERROR_INVALID_ARGUMENT;
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

    // Create (or reuse) WebRenderer for this context.
    // We store a single static WebRenderer and rebind each frame — this is
    // sufficient for single-window usage.
    static ln::Ref<WebRenderer> s_webRenderer;
    if (!s_webRenderer) {
        s_webRenderer = ln::Ref<WebRenderer>::adopt(LN_NEW WebRenderer());
    }
    s_webRenderer->bind(ctx);

    *outRenderer = wrapObjectFromGet(s_webRenderer.get());

    const ln::FramebufferInfo* fb = ctx->currentFramebuffer();
    *outColorBuffer = wrapObjectFromGet(fb->colorTexture.get());
    *outDepthBuffer = wrapObjectFromGet(fb->depthTexture.get());
    return LN_OK;
}

LUMINO_API LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;
    if (graphicsContext == LN_NULL_HANDLE) return LN_ERROR_INVALID_HANDLE;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    ctx->m_currentCmd = nullptr;
    ctx->endFrame();
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNRenderPassDesc
//------------------------------------------------------------------------------

LUMINO_API void LNRenderPassDesc_Init(LNRenderPassDesc* desc) {
    if (!desc) return;
    std::memset(desc, 0, sizeof(*desc));
    desc->depthStencil.clearDepth = 1.0f;
}

//------------------------------------------------------------------------------
// LNRenderer (WebRenderer)
//------------------------------------------------------------------------------

LUMINO_API LNResult LNRenderer_BeginRenderPass(
    LNHandle renderer,
    LNHandle graphicsContext,
    const LNRenderPassDesc* desc,
    LNHandle camera) {
    (void)camera; // Camera not yet supported on web.

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* wr = instance->objectRegistry()->resolve<WebRenderer>(renderer);
    if (!wr) return LN_ERROR_INVALID_HANDLE;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    return wr->beginRenderPass(ctx, desc);
}

LUMINO_API LNResult LNRenderer_EndRenderPass(LNHandle renderer) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* wr = instance->objectRegistry()->resolve<WebRenderer>(renderer);
    if (!wr) return LN_ERROR_INVALID_HANDLE;

    return wr->endRenderPass();
}

} // extern "C"

#endif // __EMSCRIPTEN__
