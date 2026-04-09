#include <algorithm>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>
#include <LuminoCore/Graphics/Texture2D.hpp>
#include <LuminoCore/Graphics/DebugPrint.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <cstdio>

namespace ln {

GraphicsContext::GraphicsContext()
    : m_module(nullptr) {

}

GraphicsContext::~GraphicsContext() {
    auto* dev = device();
    if (dev) {
        dev->waitIdle();
        // Destroy Renderer first (holds material cache, UBO allocators, etc.)
        m_renderer.reset();
        if (m_pipelineCache) {
            m_pipelineCache->clear();
        }
    }
}

rhi::CommandBuffer* GraphicsContext::currentCommandBuffer() const {
    return m_swapChain->getCurrentCommandBuffer();
}

u32 GraphicsContext::maxFramesInFlight() const {
    return m_swapChain->maxFramesInFlight();
}

Result<Ref<GraphicsContext>> GraphicsContext::createForWindow(
    GraphicsModule* module,
    platform::PlatformWindow* window,
    const GraphicsContextDesc& desc)
{
    auto* dev = module->device();

    auto ctx = Ref<GraphicsContext>::adopt(new GraphicsContext());
    ctx->m_module = module;
    ctx->m_window = window;
    //ctx->m_colorFormat = desc.colorFormat;

    u32 fbWidth, fbHeight;
    window->framebufferSize(fbWidth, fbHeight);
    ctx->m_width = fbWidth;
    ctx->m_height = fbHeight;

    ctx->m_pipelineCache = std::make_unique<PipelineCache>(dev);

    // SwapChain
    rhi::SwapChainDesc scDesc;
    scDesc.nativeWindowHandle = window->nativeHandle().glfwWindow;
    scDesc.width = fbWidth;
    scDesc.height = fbHeight;
    //scDesc.format = desc.colorFormat;
    scDesc.vsync = desc.vsync;
    auto swapChainResult = dev->createSwapChain(scDesc);
    if (!swapChainResult) return tl::make_unexpected(swapChainResult.error());
    ctx->m_swapChain = std::move(*swapChainResult);

    // InFlightFrame ごとにフレームバッファを1つずつ作成して管理する。
    // バックバッファは毎フレーム更新されるため、ここではダミーのテクスチャを作成しておく。
    const uint32_t inFlights = ctx->m_swapChain->maxFramesInFlight();
    for (uint32_t i = 0; i < inFlights; i++) {
        FramebufferInfo fb;
        fb.colorTexture = Texture::createBackbufferWrapper();
        auto result = Texture::createDepthStencil(dev, fbWidth, fbHeight);
        if (!result) return tl::make_unexpected(result.error());
        fb.depthTexture = *result;
        ctx->m_framebuffers.push_back(std::move(fb));
    }

    // Renderer
    auto rendererResult = Renderer::create(ctx.get());
    if (!rendererResult) return tl::make_unexpected(rendererResult.error());
    ctx->m_renderer = std::move(*rendererResult);

    return ctx;
}

Result<const FramebufferInfo*> GraphicsContext::beginFrame() {
    auto* colorTarget = m_swapChain->acquireNextTexture();
    if (!colorTarget) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Failed to acquire next texture"});
    }

    uint32_t currentFrame = m_swapChain->currentFrame();
    FramebufferInfo* fb = &m_framebuffers[currentFrame];
    fb->colorTexture->wrapBackbuffer(
        colorTarget,
        m_width,
        m_height,
        m_colorFormat);

    m_frameBeginTime = Clock::now();

    return fb;
}

const FramebufferInfo* GraphicsContext::currentFramebuffer() const {
    uint32_t currentFrame = m_swapChain->currentFrame();
    return &m_framebuffers[currentFrame];
}

void GraphicsContext::endFrame() {
    m_lastColorTarget = currentFramebuffer()->colorTexture->rhiTextureView();
    m_swapChain->present();

    auto now = Clock::now();
    auto elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(now - m_frameBeginTime).count();
    m_lastFrameTimeMs = static_cast<float>(elapsedUs) / 1000.0f;
    m_fps = (m_lastFrameTimeMs > 0.0f) ? (1000.0f / m_lastFrameTimeMs) : 0.0f;
}

Result<void> GraphicsContext::captureBackbuffer() {
    if (!m_lastColorTarget) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No backbuffer available. Call after endFrame()."});
    }

    auto* dev = device();
    if (!dev) {
        return tl::make_unexpected(Error{ErrorCode::NotInitialized, "Device not available."});
    }

    dev->waitIdle();

    auto result = dev->readbackTexture(m_lastColorTarget);
    if (!result) {
        return tl::make_unexpected(result.error());
    }

    m_captureBuffer = std::move(*result);

    // BGRA → RGBA swizzle (swapchain format is BGRA8Unorm)
    for (size_t i = 0; i < m_captureBuffer.size(); i += 4) {
        std::swap(m_captureBuffer[i], m_captureBuffer[i + 2]);
    }

    return {};
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

rhi::Device* GraphicsContext::device() const {
    return m_module->device();
}

} // namespace ln
