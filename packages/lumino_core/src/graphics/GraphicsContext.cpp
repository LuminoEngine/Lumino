#include <lumino_core/graphics/GraphicsContext.hpp>
#include <cstdio>

namespace ln {

GraphicsContext::~GraphicsContext() {
    if (m_device) {
        if (m_pipelineCache) {
            m_pipelineCache->clear();
        }
        m_device->waitIdle();
    }
}

Result<Ref<GraphicsContext>> GraphicsContext::createForWindow(
    platform::PlatformWindow* window,
    const GraphicsContextDesc& desc)
{
    auto ctx = Ref<GraphicsContext>::adopt(new GraphicsContext());
    ctx->m_window = window;
    //ctx->m_colorFormat = desc.colorFormat;

    u32 fbWidth, fbHeight;
    window->framebufferSize(fbWidth, fbHeight);
    ctx->m_width = fbWidth;
    ctx->m_height = fbHeight;

    // 1. Device
    rhi::DeviceDesc devDesc;
    devDesc.backend = desc.preferredBackend;
    devDesc.enableValidation = desc.enableValidation;
    auto deviceResult = rhi::Device::create(devDesc);
    if (!deviceResult) return tl::make_unexpected(deviceResult.error());
    ctx->m_device = std::move(*deviceResult);
    ctx->m_pipelineCache = std::make_unique<PipelineCache>(ctx->m_device.get());

    // 2. SwapChain
    rhi::SwapChainDesc scDesc;
    scDesc.nativeWindowHandle = window->nativeHandle().glfwWindow;
    scDesc.width = fbWidth;
    scDesc.height = fbHeight;
    //scDesc.format = desc.colorFormat;
    scDesc.vsync = desc.vsync;
    auto swapChainResult = ctx->m_device->createSwapChain(scDesc);
    if (!swapChainResult) return tl::make_unexpected(swapChainResult.error());
    ctx->m_swapChain = std::move(*swapChainResult);

    // 3. Depth texture + view
    rhi::TextureDesc depthTexDesc;
    depthTexDesc.width = fbWidth;
    depthTexDesc.height = fbHeight;
    depthTexDesc.format = ctx->m_depthFormat;
    depthTexDesc.usage = rhi::TextureUsage::DepthStencil;
    auto depthTexResult = ctx->m_device->createTexture(depthTexDesc);
    if (!depthTexResult) return tl::make_unexpected(depthTexResult.error());
    ctx->m_depthTexture = std::move(*depthTexResult);

    auto depthViewResult = ctx->m_device->createTextureView(ctx->m_depthTexture.get());
    if (!depthViewResult) return tl::make_unexpected(depthViewResult.error());
    ctx->m_depthView = std::move(*depthViewResult);

    return ctx;
}

Result<FrameInfo> GraphicsContext::beginFrame() {
    auto* colorTarget = m_swapChain->acquireNextTexture();
    if (!colorTarget) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Failed to acquire next texture"});
    }

    FrameInfo info;
    info.colorTarget = colorTarget;
    info.depthTarget = m_depthView.get();
    info.width = m_width;
    info.height = m_height;
    return info;
}

void GraphicsContext::endFrame() {
    m_swapChain->present();
}

void GraphicsContext::waitIdle() {
    if (m_device) m_device->waitIdle();
}

} // namespace ln
