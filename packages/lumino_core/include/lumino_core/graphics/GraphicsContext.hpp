#pragma once
#include <lumino_base/Result.hpp>
#include <lumino_core/Object.hpp>
#include <lumino_core/graphics/rhi/Rhi.hpp>
#include <lumino_core/platform/Window.hpp>

namespace ln {

/** Descriptor for creating a GraphicsContext (device + swap chain + depth buffer). */
struct GraphicsContextDesc {
    rhi::Backend preferredBackend = rhi::Backend::Vulkan;
    bool enableValidation = false;
    rhi::TextureFormat colorFormat = rhi::TextureFormat::BGRA8UnormSrgb;
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

    /** The underlying RHI device. */
    rhi::Device* device() const { return device_.get(); }

    /** Color format of the swap chain. */
    rhi::TextureFormat colorFormat() const { return colorFormat_; }

    /** Depth format managed by this context. */
    rhi::TextureFormat depthFormat() const { return depthFormat_; }

    /** Current framebuffer width in pixels. */
    u32 width() const { return width_; }

    /** Current framebuffer height in pixels. */
    u32 height() const { return height_; }

    // フレームスコープの一時状態 (BeginFrame〜EndFrame 間有効)
    rhi::CommandBuffer*        currentCmd_         = nullptr;
    rhi::TextureView*          currentColorTarget_ = nullptr;
    rhi::TextureView*          currentDepthTarget_ = nullptr;
    rhi::RenderPassEncoder*    currentPass_        = nullptr;

private:
    GraphicsContext() = default;

    platform::PlatformWindow* window_ = nullptr; // non-owning
    Ref<rhi::Device> device_;
    Ref<rhi::SwapChain> swapChain_;
    Ref<rhi::Texture> depthTexture_;
    Ref<rhi::TextureView> depthView_;
    rhi::TextureFormat colorFormat_ = rhi::TextureFormat::BGRA8UnormSrgb;
    rhi::TextureFormat depthFormat_ = rhi::TextureFormat::Depth32Float;
    u32 width_ = 0;
    u32 height_ = 0;
};

} // namespace ln
