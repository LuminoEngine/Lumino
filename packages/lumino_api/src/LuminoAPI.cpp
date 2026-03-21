#include "pch.hpp"
#include <LuminoBase.hpp>
#include <lumino_core/CoreInstance.hpp>
#include <lumino_core/Object.hpp>
#include <lumino_core/runtime/ObjectRegistry.hpp>
#include <lumino_core/graphics/Texture2D.hpp>
#include <lumino_core/platform/Window.hpp>
#include <lumino_core/graphics/GraphicsContext.hpp>
#include <lumino_api/lumino.h>

//------------------------------------------------------------------------------
// LNInstance
//------------------------------------------------------------------------------

LNResult LNInstance_Initialize() {
    auto result = ln::CoreInstance::initialize(ln::CoreInstance::Settings{});
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

    if (!instance->objectRegistry()->release(handle))
        return LN_ERROR_INVALID_HANDLE;

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

    auto texture = ln::Ref<ln::Texture2D>::adopt(LN_NEW ln::Texture2D(width, height, format));
    LNHandle handle = instance->objectRegistry()->registerObject(std::move(texture));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
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

    auto* window = ln::platform::PlatformWindow::create(winDesc, gfxDesc);
    if (!window) return LN_ERROR_UNKNOWN;

    auto obj = ln::Ref<ln::platform::PlatformWindow>::adopt(window);
    LNHandle handle = instance->objectRegistry()->registerObject(std::move(obj));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}

LNResult LNWindow_ProcessEvents(LNHandle handle, int* outContinue) {
    if (!outContinue) return LN_ERROR_INVALID_ARGUMENT;
    *outContinue = 0;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<ln::platform::PlatformWindow>(handle);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    *outContinue = obj->processEvents() ? 1 : 0;
    return LN_OK;
}

LNResult LNWindow_GetGraphicsContext(LNHandle window, LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<ln::platform::PlatformWindow>(window);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    auto* ctx = obj->graphicsContext();
    if (!ctx) return LN_ERROR_UNKNOWN;

    // 既に登録済みの場合はハンドルを再構成して返す
    if (ctx->generation() != 0) {
        *outHandle = ln::ObjectRegistry::makeHandle(ctx->registryIndex(), ctx->generation());
        return LN_OK;
    }

    // 初回登録: window が所有しているので addRef してから登録する
    ctx->addRef();
    LNHandle handle = instance->objectRegistry()->registerObject(
        ln::Ref<ln::GraphicsContext>::adopt(ctx));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNGraphicsContext
//------------------------------------------------------------------------------

LNResult LNGraphicsContext_BeginFrame(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto frameResult = ctx->beginFrame();
    if (!frameResult) return LN_ERROR_UNKNOWN;

    ctx->currentColorTarget_ = frameResult->colorTarget;
    ctx->currentDepthTarget_ = frameResult->depthTarget;
    ctx->currentCmd_ = ctx->device()->createCommandBuffer();
    if (!ctx->currentCmd_) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNGraphicsContext_BeginRenderPass(
    LNHandle graphicsContext, float r, float g, float b, float a) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx || !ctx->currentCmd_) return LN_ERROR_INVALID_HANDLE;

    ln::rhi::ColorAttachment colorAttach;
    colorAttach.view = ctx->currentColorTarget_;
    colorAttach.loadOp = ln::rhi::LoadOp::Clear;
    colorAttach.storeOp = ln::rhi::StoreOp::Store;
    colorAttach.clearColor = {r, g, b, a};

    ln::rhi::DepthStencilAttachment depthAttach;
    depthAttach.view = ctx->currentDepthTarget_;
    depthAttach.depthLoadOp = ln::rhi::LoadOp::Clear;
    depthAttach.depthStoreOp = ln::rhi::StoreOp::Store;
    depthAttach.clearDepth = 1.0f;

    ln::rhi::RenderPassDesc rpDesc;
    rpDesc.colorAttachments = {colorAttach};
    rpDesc.depthStencilAttachment = &depthAttach;

    ctx->currentPass_ = ctx->currentCmd_->beginRenderPass(rpDesc);
    if (!ctx->currentPass_) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNGraphicsContext_EndRenderPass(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx || !ctx->currentPass_) return LN_ERROR_INVALID_HANDLE;

    ctx->currentPass_->end();
    ctx->currentPass_ = nullptr;

    return LN_OK;
}

LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx || !ctx->currentCmd_) return LN_ERROR_INVALID_HANDLE;

    ctx->currentCmd_->submit();
    ctx->currentCmd_ = nullptr;

    ctx->endFrame();

    return LN_OK;
}
