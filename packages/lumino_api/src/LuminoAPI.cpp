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
// WindowObject PlatformWindow の C_API 向けラッパー
//------------------------------------------------------------------------------

class WindowObject : public ln::Object {
public:
    ~WindowObject() override {
        delete window_;
    }

    ln::platform::PlatformWindow* window_ = nullptr;

    // フレームスコープの一時状態
    ln::rhi::CommandBuffer* currentCmd_ = nullptr;
    ln::rhi::TextureView* currentColorTarget_ = nullptr;
    ln::rhi::TextureView* currentDepthTarget_ = nullptr;
    ln::rhi::RenderPassEncoder* currentPass_ = nullptr;
};

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

    auto obj = ln::Ref<WindowObject>::adopt(LN_NEW WindowObject());
    obj->window_ = window;

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

    auto* obj = instance->objectRegistry()->resolve<WindowObject>(handle);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    *outContinue = obj->window_->processEvents() ? 1 : 0;
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNGraphicsContext
//------------------------------------------------------------------------------

LNResult LNGraphicsContext_BeginFrame(LNHandle window) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<WindowObject>(window);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    auto* ctx = obj->window_->graphicsContext();
    if (!ctx) return LN_ERROR_UNKNOWN;

    auto frameResult = ctx->beginFrame();
    if (!frameResult) return LN_ERROR_UNKNOWN;

    obj->currentColorTarget_ = frameResult->colorTarget;
    obj->currentDepthTarget_ = frameResult->depthTarget;
    obj->currentCmd_ = ctx->device()->createCommandBuffer();
    if (!obj->currentCmd_) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNGraphicsContext_BeginRenderPass(
    LNHandle window, float r, float g, float b, float a) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<WindowObject>(window);
    if (!obj) return LN_ERROR_INVALID_HANDLE;
    if (!obj->currentCmd_) return LN_ERROR_UNKNOWN;

    ln::rhi::ColorAttachment colorAttach;
    colorAttach.view = obj->currentColorTarget_;
    colorAttach.loadOp = ln::rhi::LoadOp::Clear;
    colorAttach.storeOp = ln::rhi::StoreOp::Store;
    colorAttach.clearColor = {r, g, b, a};

    ln::rhi::DepthStencilAttachment depthAttach;
    depthAttach.view = obj->currentDepthTarget_;
    depthAttach.depthLoadOp = ln::rhi::LoadOp::Clear;
    depthAttach.depthStoreOp = ln::rhi::StoreOp::Store;
    depthAttach.clearDepth = 1.0f;

    ln::rhi::RenderPassDesc rpDesc;
    rpDesc.colorAttachments = {colorAttach};
    rpDesc.depthStencilAttachment = &depthAttach;

    obj->currentPass_ = obj->currentCmd_->beginRenderPass(rpDesc);
    if (!obj->currentPass_) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNGraphicsContext_EndRenderPass(LNHandle window) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<WindowObject>(window);
    if (!obj) return LN_ERROR_INVALID_HANDLE;
    if (!obj->currentPass_) return LN_ERROR_UNKNOWN;

    obj->currentPass_->end();
    obj->currentPass_ = nullptr;

    return LN_OK;
}

LNResult LNGraphicsContext_EndFrame(LNHandle window) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<WindowObject>(window);
    if (!obj) return LN_ERROR_INVALID_HANDLE;
    if (!obj->currentCmd_) return LN_ERROR_UNKNOWN;

    obj->currentCmd_->submit();
    obj->currentCmd_ = nullptr;

    obj->window_->graphicsContext()->endFrame();

    return LN_OK;
}
