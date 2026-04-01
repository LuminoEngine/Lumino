#pragma once
#include <memory>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/graphics/rhi/Rhi.hpp>
#include <LuminoCore/graphics/PipelineCache.hpp>
#include <LuminoCore/platform/Window.hpp>

namespace ln {

/** Descriptor for creating a GraphicsContext (swap chain + depth buffer). */
struct GraphicsContextDesc {
    //rhi::TextureFormat colorFormat = rhi::TextureFormat::BGRA8UnormSrgb;
    bool vsync = true;
};

/** Per-frame rendering targets returned by GraphicsContext::beginFrame(). */
struct FrameInfo {
    rhi::TextureView* colorTarget;
    rhi::TextureView* depthTarget;
    u32 width;
    u32 height;
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
        platform::PlatformWindow* window,
        const GraphicsContextDesc& desc);

    // Future creation paths
    // static Result<Ref<GraphicsContext>> createFromCanvas(const char* selector);
    // static Result<Ref<GraphicsContext>> createExternal(const ExternalContextDesc& desc);

    /** Acquire the next back buffer and prepare frame targets. */
    Result<FrameInfo> beginFrame();

    /** Present the current frame. */
    void endFrame();

    void waitIdle();

    /** The underlying RHI device (owned by CoreInstance). */
    rhi::Device* device() const;

    /** Pipeline cache for this context. */
    PipelineCache* pipelineCache() const { return m_pipelineCache.get(); }

    /** Color format of the swap chain. */
    rhi::TextureFormat colorFormat() const { return m_colorFormat; }

    /** Depth format managed by this context. */
    rhi::TextureFormat depthFormat() const { return m_depthFormat; }

    /** Current framebuffer width in pixels. */
    u32 width() const { return m_width; }

    /** Current framebuffer height in pixels. */
    u32 height() const { return m_height; }

    // フレームスコープの一時状態 (BeginFrame〜EndFrame 間有効)
    rhi::CommandBuffer*        m_currentCmd         = nullptr;
    rhi::TextureView*          m_currentColorTarget = nullptr;
    rhi::TextureView*          m_currentDepthTarget = nullptr;
    rhi::RenderPassEncoder*    m_currentPass        = nullptr;

private:
    GraphicsContext() = default;

    platform::PlatformWindow* m_window = nullptr; // non-owning
    Ref<rhi::SwapChain> m_swapChain;
    Ref<rhi::Texture> m_depthTexture;
    Ref<rhi::TextureView> m_depthView;
    rhi::TextureFormat m_colorFormat = rhi::TextureFormat::BGRA8Unorm;
    rhi::TextureFormat m_depthFormat = rhi::TextureFormat::Depth32Float;
    u32 m_width = 0;
    u32 m_height = 0;
    std::unique_ptr<PipelineCache> m_pipelineCache;
};

} // namespace ln
