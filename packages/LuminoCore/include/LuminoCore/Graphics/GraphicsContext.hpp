#pragma once
#include <memory>
#include <chrono>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <LuminoCore/Graphics/PipelineCache.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>
#if !defined(__EMSCRIPTEN__)
#include <LuminoCore/Graphics/DebugPrint.hpp>
#endif
#include <LuminoCore/Platform/Window.hpp>

namespace ln {
class GraphicsModule;
class Texture;

/** Descriptor for creating a GraphicsContext (swap chain + depth buffer). */
struct GraphicsContextDesc {
    //rhi::TextureFormat colorFormat = rhi::TextureFormat::BGRA8UnormSrgb;
    bool vsync = false;
};

/** Per-frame rendering targets returned by GraphicsContext::beginFrame(). */
struct FramebufferInfo {
    Ref<Texture> colorTexture;
    Ref<Texture> depthTexture;
};

/**
 * Unified graphics context that encapsulates RHI device, swap chain,
 * and depth buffer management.
 *
 * Created by PlatformWindow and accessed via PlatformWindow::graphicsContext().
 *
 * Multiple creation paths are planned:
 *   - Desktop window:      createForWindow() builds device + swap chain for a window.
 *   - Browser WebGPU:      createFromCanvas() (future).
 *   - External embedding:  createExternal() (future).
 *
 * After creation, the rendering code is identical regardless of the creation path.
 */
class GraphicsContext : public Object {
public:

    ~GraphicsContext() override;

    /** Create a graphics context for an existing window. */
    static Result<Ref<GraphicsContext>> createForWindow(
        GraphicsModule* module,
        platform::PlatformWindow* window,
        const GraphicsContextDesc& desc);

    /** Create a graphics context from an HTML canvas (web only). */
    static Result<Ref<GraphicsContext>> createForCanvas(
        GraphicsModule* module,
        const std::string& canvasSelector,
        uint32_t width,
        uint32_t height,
        const GraphicsContextDesc& desc);

    // Future creation paths
    // static Result<Ref<GraphicsContext>> createExternal(const ExternalContextDesc& desc);

    /** Acquire the next back buffer and prepare frame targets. */
    Result<const FramebufferInfo*> beginFrame();
    const FramebufferInfo* currentFramebuffer() const;

    /** Present the current frame. */
    void endFrame();

    /**
     * Capture the backbuffer contents as RGBA8 pixel data.
     * Must be called after endFrame(). The returned pointer is valid until
     * the next call to captureBackbuffer().
     */
    Result<void> captureBackbuffer();

    /** Access the pixel data captured by the last captureBackbuffer() call. */
    const std::vector<uint8_t>& captureBuffer() const { return m_captureBuffer; }

    void waitIdle();

    /** The GraphicsModule that owns the RHI device. */
    GraphicsModule* module() const { return m_module; }

    /** The underlying RHI device (owned by CoreInstance). */
    rhi::Device* device() const;

    /** Pipeline cache for this context. */
    PipelineCache* pipelineCache() const { return m_pipelineCache.get(); }

    /** Color format of the swap chain. */
    rhi::TextureFormat colorFormat() const { return m_colorFormat; }

    /** Depth format managed by this context. */
    rhi::TextureFormat depthFormat() const { return m_depthFormat; }

    /** Current framebuffer width in pixels. */
    uint32_t width() const { return m_width; }

    /** Current framebuffer height in pixels. */
    uint32_t height() const { return m_height; }

    /** Number of in-flight frames (from swap chain). */
    uint32_t maxFramesInFlight() const;

    rhi::CommandBuffer* currentCommandBuffer() const;

    /** Renderer owned by this context. */
    Renderer* renderer() const { return m_renderer.get(); }

#if !defined(__EMSCRIPTEN__)
    /** Debug print helper owned by this context. Created lazily on first use. */
    DebugPrint* debugPrint() { return m_debugPrint.get(); }
#endif

    /**
     * Queue a debug string for overlay rendering in the current frame.
     * Safe to call before DebugPrint is initialized - ignored if unavailable.
     */
    void debugPrintText(const char* str);

    /** Initialize (or re-initialize) the DebugPrint subsystem. */
    Result<void> initDebugPrint();

    /** Frame time of the last completed frame in milliseconds. */
    float lastFrameTimeMs() const { return m_lastFrameTimeMs; }

    /** Frames per second (averaged over the last completed frame). */
    float fps() const { return m_fps; }

    // フレームスコープの一時状態 (BeginFrame~EndFrame 間有効)
    rhi::CommandBuffer*        m_currentCmd         = nullptr;
    rhi::RenderPass*           m_currentPass        = nullptr;

private:

    GraphicsContext();

    GraphicsModule* m_module;
    platform::PlatformWindow* m_window = nullptr; // non-owning
    Ref<rhi::SwapChain> m_swapChain;
    std::vector<FramebufferInfo> m_framebuffers; // SwapChain Image (InFlightFrame) ごとに1つ

#if !defined(__EMSCRIPTEN__)
    Ref<DebugPrint> m_debugPrint;
#endif

    // Frame timing
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point m_frameBeginTime = {};
    bool              m_firstFrame     = true;
    float             m_lastFrameTimeMs = 0.0f;
    float             m_fps             = 0.0f;
    rhi::TextureView* m_lastColorTarget = nullptr;
    std::vector<uint8_t> m_captureBuffer;
    rhi::TextureFormat m_colorFormat = rhi::TextureFormat::BGRA8Unorm;
    rhi::TextureFormat m_depthFormat = rhi::TextureFormat::Depth24Stencil8;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    std::unique_ptr<PipelineCache> m_pipelineCache;
    Ref<Renderer> m_renderer;
};

} // namespace ln
