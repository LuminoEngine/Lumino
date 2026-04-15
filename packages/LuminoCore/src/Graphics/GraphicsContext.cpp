#include <algorithm>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/Texture2D.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <cstdio>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>
#include <LuminoCore/Graphics/PipelineCache.hpp>
#if !defined(__EMSCRIPTEN__)
#include <LuminoCore/Graphics/DebugPrint.hpp>
#endif

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

uint32_t GraphicsContext::maxFramesInFlight() const {
    return m_swapChain->maxFramesInFlight();
}

#if !defined(__EMSCRIPTEN__)

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

    uint32_t fbWidth, fbHeight;
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
    if (!swapChainResult) return LN_FORWARD_ERROR(swapChainResult);
    ctx->m_swapChain = std::move(*swapChainResult);

    // InFlightFrame ごとにフレームバッファを1つずつ作成して管理する。
    // バックバッファは毎フレーム更新されるため、ここではダミーのテクスチャを作成しておく。
    const uint32_t inFlights = ctx->m_swapChain->maxFramesInFlight();
    for (uint32_t i = 0; i < inFlights; i++) {
        FramebufferInfo fb;
        fb.colorTexture = Texture::createBackbufferWrapper();
        auto result = Texture::createDepthStencil(dev, fbWidth, fbHeight);
        if (!result) return LN_FORWARD_ERROR(result);
        fb.depthTexture = *result;
        ctx->m_framebuffers.push_back(std::move(fb));
    }

    // Renderer
    auto rendererResult = Renderer::create(ctx.get());
    if (!rendererResult) return LN_FORWARD_ERROR(rendererResult);
    ctx->m_renderer = std::move(*rendererResult);

    return ctx;
}

#endif // !defined(__EMSCRIPTEN__)

Result<const FramebufferInfo*> GraphicsContext::beginFrame() {
    auto* colorTarget = m_swapChain->acquireNextTexture();
    if (!colorTarget) {
        return LN_MAKE_ERROR("Failed to acquire next texture");
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
        return LN_MAKE_ERROR("No backbuffer available. Call after endFrame().");
    }

    auto* dev = device();
    if (!dev) {
        return LN_MAKE_ERROR("Device not available.");
    }

    dev->waitIdle();

    auto result = dev->readbackTexture(m_lastColorTarget);
    if (!result) {
        return LN_FORWARD_ERROR(result);
    }

    m_captureBuffer = std::move(*result);

    // BGRA → RGBA swizzle (swapchain format is BGRA8Unorm)
    for (size_t i = 0; i < m_captureBuffer.size(); i += 4) {
        std::swap(m_captureBuffer[i], m_captureBuffer[i + 2]);
    }

    return {};
}

#if !defined(__EMSCRIPTEN__)

Result<void> GraphicsContext::initDebugPrint() {
    auto result = DebugPrint::create(this);
    if (!result) return LN_FORWARD_ERROR(result);
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

#else // __EMSCRIPTEN__

Result<void> GraphicsContext::initDebugPrint() {
    return {}; // not supported on web
}

void GraphicsContext::debugPrintText(const char* /*str*/) {
    // not supported on web
}

#endif // __EMSCRIPTEN__

void GraphicsContext::waitIdle() {
    auto* dev = device();
    if (dev) dev->waitIdle();
}

rhi::Device* GraphicsContext::device() const {
    return m_module ? m_module->device() : nullptr;
}

Result<Ref<GraphicsContext>> GraphicsContext::createForCanvas(
    GraphicsModule* module,
    const std::string& canvasSelector,
    uint32_t width,
    uint32_t height,
    const GraphicsContextDesc& desc)
{
    if (!module) {
        return LN_MAKE_ERROR("GraphicsModule is null.");
    }
    auto* dev = module->device();
    if (!dev) {
        return LN_MAKE_ERROR("RHI device is null.");
    }

    auto ctx = Ref<GraphicsContext>::adopt(new GraphicsContext());
    ctx->m_module = module;
    ctx->m_window = nullptr;
    ctx->m_width = width;
    ctx->m_height = height;

    ctx->m_pipelineCache = std::make_unique<PipelineCache>(dev);

    // SwapChain - pass the canvas selector via nativeWindowHandle
    rhi::SwapChainDesc scDesc;
    scDesc.nativeWindowHandle = const_cast<char*>(canvasSelector.c_str());
    scDesc.width = width;
    scDesc.height = height;
    scDesc.vsync = desc.vsync;
    auto swapChainResult = dev->createSwapChain(scDesc);
    if (!swapChainResult) return LN_FORWARD_ERROR(swapChainResult);
    ctx->m_swapChain = std::move(*swapChainResult);

    // InFlightFrame framebuffers
    const uint32_t inFlights = ctx->m_swapChain->maxFramesInFlight();
    for (uint32_t i = 0; i < inFlights; i++) {
        FramebufferInfo fb;
        fb.colorTexture = Texture::createBackbufferWrapper();
        auto result = Texture::createDepthStencil(dev, width, height);
        if (!result) return LN_FORWARD_ERROR(result);
        fb.depthTexture = *result;
        ctx->m_framebuffers.push_back(std::move(fb));
    }

    // Renderer
    auto rendererResult = Renderer::create(ctx.get());
    if (!rendererResult) return LN_FORWARD_ERROR(rendererResult);
    ctx->m_renderer = std::move(*rendererResult);

    return ctx;
}

} // namespace ln
