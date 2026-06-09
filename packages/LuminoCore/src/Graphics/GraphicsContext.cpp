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

Result<const FramebufferInfo*> GraphicsContext::beginFrame(uint32_t width, uint32_t height) {
    // 前フレームのキャプチャ結果を無効化する (このフレームで要求が無ければ取得不可)。
    m_captureValid = false;

    // サイズが変更された場合は、スワップチェーンと深度バッファのサイズを変更します。
    if (width != m_width || height != m_height) {
        auto resizeResult = m_swapChain->resize(width, height);
        if (!resizeResult) {
            return LN_FORWARD_ERROR(resizeResult);
        }

        // in-flight frames の深度バッファを再作成する
        auto* dev = device();
        for (auto& fb : m_framebuffers) {
            auto result = Texture::createDepthStencil(dev, width, height);
            if (!result) return LN_FORWARD_ERROR(result);
            fb.depthTexture = *result;
        }

        m_width = width;
        m_height = height;
    }

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
    // present() の後はスワップチェーンイメージが acquire 解除され、
    // レイアウト遷移を含む一切の使用ができなくなる
    // (VUID UNASSIGNED-non-acquired-swapchain-image-used)。
    // そのため、キャプチャ要求があるフレームは present() 直前に読み戻す。
    // この時点では描画コマンドは submit 済みで、イメージは acquire 済み・
    // PRESENT_SRC_KHR レイアウトのまま残っている。
    if (m_captureRequested) {
        (void)captureBackbufferInternal();
        m_captureRequested = false;
    }

    m_swapChain->present();

    auto now = Clock::now();
    auto elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(now - m_frameBeginTime).count();
    m_lastFrameTimeMs = static_cast<float>(elapsedUs) / 1000.0f;
    m_fps = (m_lastFrameTimeMs > 0.0f) ? (1000.0f / m_lastFrameTimeMs) : 0.0f;
}

void GraphicsContext::requestCaptureBackbuffer() {
    m_captureRequested = true;
}

Result<void> GraphicsContext::captureBackbufferInternal() {
    m_captureValid = false;

    auto* dev = device();
    if (!dev) {
        return LN_MAKE_ERROR("Device not available.");
    }

    auto* view = currentFramebuffer()->colorTexture->rhiTextureView();
    if (!view) {
        return LN_MAKE_ERROR("No backbuffer available.");
    }

    auto result = dev->readbackTexture(view);
    if (!result) {
        return LN_FORWARD_ERROR(result);
    }

    m_captureBuffer = std::move(*result);

    // BGRA → RGBA swizzle (swapchain format is BGRA8Unorm)
    for (size_t i = 0; i + 3 < m_captureBuffer.size(); i += 4) {
        std::swap(m_captureBuffer[i], m_captureBuffer[i + 2]);
    }

    m_captureValid = true;
    return {};
}

Result<void> GraphicsContext::captureBackbuffer() {
    if (!m_captureValid) {
        return LN_MAKE_ERROR(
            "No captured backbuffer. Call requestCaptureBackbuffer() before endFrame().");
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
