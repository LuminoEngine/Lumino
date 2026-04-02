#include <LuminoCore/graphics/GraphicsContext.hpp>
#include <LuminoCore/graphics/Renderer.hpp>
#include <LuminoCore/graphics/DebugPrint.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <cstdio>

namespace ln {

GraphicsContext::~GraphicsContext() {
    auto* dev = device();
    if (dev) {
        // Destroy Renderer first (holds material cache, UBO allocators, etc.)
        m_renderer.reset();
        if (m_pipelineCache) {
            m_pipelineCache->clear();
        }
        dev->waitIdle();
    }
}

rhi::Device* GraphicsContext::device() const {
    auto* inst = CoreInstance::instance();
    return inst ? inst->device() : nullptr;
}

rhi::CommandBuffer* GraphicsContext::currentCommandBuffer() const {
    return m_swapChain->getCurrentCommandBuffer();
}

Result<Ref<GraphicsContext>> GraphicsContext::createForWindow(
    platform::PlatformWindow* window,
    const GraphicsContextDesc& desc)
{
    auto* dev = CoreInstance::instance()->device();
    if (!dev) return tl::make_unexpected(Error{ErrorCode::NotInitialized, "CoreInstance device not initialized"});

    auto ctx = Ref<GraphicsContext>::adopt(new GraphicsContext());
    ctx->m_window = window;
    //ctx->m_colorFormat = desc.colorFormat;

    u32 fbWidth, fbHeight;
    window->framebufferSize(fbWidth, fbHeight);
    ctx->m_width = fbWidth;
    ctx->m_height = fbHeight;

    ctx->m_pipelineCache = std::make_unique<PipelineCache>(dev);

    // 1. SwapChain
    rhi::SwapChainDesc scDesc;
    scDesc.nativeWindowHandle = window->nativeHandle().glfwWindow;
    scDesc.width = fbWidth;
    scDesc.height = fbHeight;
    //scDesc.format = desc.colorFormat;
    scDesc.vsync = desc.vsync;
    auto swapChainResult = dev->createSwapChain(scDesc);
    if (!swapChainResult) return tl::make_unexpected(swapChainResult.error());
    ctx->m_swapChain = std::move(*swapChainResult);

    // 2. Depth texture + view
    rhi::TextureDesc depthTexDesc;
    depthTexDesc.width = fbWidth;
    depthTexDesc.height = fbHeight;
    depthTexDesc.format = ctx->m_depthFormat;
    depthTexDesc.usage = rhi::TextureUsage::DepthStencil;
    auto depthTexResult = dev->createTexture(depthTexDesc);
    if (!depthTexResult) return tl::make_unexpected(depthTexResult.error());
    ctx->m_depthTexture = std::move(*depthTexResult);

    auto depthViewResult = dev->createTextureView(ctx->m_depthTexture.get());
    if (!depthViewResult) return tl::make_unexpected(depthViewResult.error());
    ctx->m_depthView = std::move(*depthViewResult);

    // 3. Renderer
    auto rendererResult = Renderer::create(ctx.get());
    if (!rendererResult) return tl::make_unexpected(rendererResult.error());
    ctx->m_renderer = std::move(*rendererResult);

    return ctx;
}

Result<FrameInfo> GraphicsContext::beginFrame() {
    auto* colorTarget = m_swapChain->acquireNextTexture();
    if (!colorTarget) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Failed to acquire next texture"});
    }

    m_frameBeginTime = Clock::now();

    FrameInfo info;
    info.colorTarget = colorTarget;
    info.depthTarget = m_depthView.get();
    info.width = m_width;
    info.height = m_height;
    return info;
}

void GraphicsContext::endFrame() {
    m_swapChain->present();

    auto now = Clock::now();
    auto elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(now - m_frameBeginTime).count();
    m_lastFrameTimeMs = static_cast<float>(elapsedUs) / 1000.0f;
    m_fps = (m_lastFrameTimeMs > 0.0f) ? (1000.0f / m_lastFrameTimeMs) : 0.0f;
}

Result<void> GraphicsContext::initDebugPrint() {
    auto result = DebugPrint::create(this);
    if (!result) return tl::make_unexpected(result.error());
    m_debugPrint = std::move(*result);
    return {};
}

void GraphicsContext::debugPrintText(const char* str) {
    if (!str) return;
    if (!m_debugPrint) {
        // Lazy init on first use.
        auto r = initDebugPrint();
        if (!r) return; // silently ignore if initialization fails
    }
    m_debugPrint->print(str);
}

void GraphicsContext::waitIdle() {
    auto* dev = device();
    if (dev) dev->waitIdle();
}

} // namespace ln
