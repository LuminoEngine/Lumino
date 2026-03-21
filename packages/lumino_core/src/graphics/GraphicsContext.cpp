#include <lumino_core/graphics/GraphicsContext.hpp>
#include <cstdio>

namespace ln {

GraphicsContext::~GraphicsContext() {
    if (device_) {
        device_->waitIdle();
    }
}

Result<Ref<GraphicsContext>> GraphicsContext::createForWindow(
    platform::PlatformWindow* window,
    const GraphicsContextDesc& desc)
{
    auto ctx = Ref<GraphicsContext>::adopt(new GraphicsContext());
    ctx->window_ = window;
    ctx->colorFormat_ = desc.colorFormat;

    u32 fbWidth, fbHeight;
    window->framebufferSize(fbWidth, fbHeight);
    ctx->width_ = fbWidth;
    ctx->height_ = fbHeight;

    // 1. Device
    rhi::DeviceDesc devDesc;
    devDesc.backend = desc.preferredBackend;
    devDesc.enableValidation = desc.enableValidation;
    auto deviceResult = rhi::Device::create(devDesc);
    if (!deviceResult) return tl::make_unexpected(deviceResult.error());
    ctx->device_ = std::move(*deviceResult);

    // 2. SwapChain
    rhi::SwapChainDesc scDesc;
    scDesc.nativeWindowHandle = window->nativeHandle().glfwWindow;
    scDesc.width = fbWidth;
    scDesc.height = fbHeight;
    scDesc.format = desc.colorFormat;
    scDesc.vsync = desc.vsync;
    auto swapChainResult = ctx->device_->createSwapChain(scDesc);
    if (!swapChainResult) return tl::make_unexpected(swapChainResult.error());
    ctx->swapChain_ = std::move(*swapChainResult);

    // 3. Depth texture + view
    rhi::TextureDesc depthTexDesc;
    depthTexDesc.width = fbWidth;
    depthTexDesc.height = fbHeight;
    depthTexDesc.format = ctx->depthFormat_;
    depthTexDesc.usage = rhi::TextureUsage::DepthStencil;
    auto depthTexResult = ctx->device_->createTexture(depthTexDesc);
    if (!depthTexResult) return tl::make_unexpected(depthTexResult.error());
    ctx->depthTexture_ = std::move(*depthTexResult);

    auto depthViewResult = ctx->device_->createTextureView(ctx->depthTexture_.get());
    if (!depthViewResult) return tl::make_unexpected(depthViewResult.error());
    ctx->depthView_ = std::move(*depthViewResult);

    return ctx;
}

Result<FrameInfo> GraphicsContext::beginFrame() {
    auto* colorTarget = swapChain_->acquireNextTexture();
    if (!colorTarget) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Failed to acquire next texture"});
    }

    FrameInfo info;
    info.colorTarget = colorTarget;
    info.depthTarget = depthView_.get();
    info.width = width_;
    info.height = height_;
    return info;
}

void GraphicsContext::endFrame() {
    swapChain_->present();
}

} // namespace ln
