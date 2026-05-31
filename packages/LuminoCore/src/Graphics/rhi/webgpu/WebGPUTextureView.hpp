#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

class WebGPUDevice;

class WebGPUTextureView final : public TextureView {
public:
    WebGPUTextureView();

    /** Create a view for a device-created texture. */
    VoidResult init(WebGPUDevice* device, WGPUTexture texture, WGPUTextureFormat format,
                    WGPUTextureAspect aspect, uint32_t width, uint32_t height);

    /** Wrap an externally-owned WGPUTextureView (swap chain back-buffer). */
    void initFromExternal(WGPUTextureView view, WGPUTextureFormat format, uint32_t width, uint32_t height);

    /**
     * Replace the wrapped view (called each frame for swap chain textures).
     * sourceTexture は readbackTexture でのテクスチャ→バッファコピー元として使う
     * (非所有。SwapChain 等が所有権を持つ)。不要なら nullptr。
     */
    void rewrap(WGPUTextureView view, WGPUTexture sourceTexture = nullptr);

    WGPUTextureView handle() const { return m_view; }
    WGPUTextureFormat format() const { return m_format; }
    uint32_t width() const override { return m_width; }
    uint32_t height() const override { return m_height; }

    /** readbackTexture 用。コピー元となる WGPUTexture (非所有)。無い場合 nullptr。 */
    WGPUTexture sourceTexture() const { return m_sourceTexture; }

protected:
    void finalize() override;

private:
    WGPUTextureView m_view = nullptr;
    WGPUTexture m_sourceTexture = nullptr;  // 非所有: readback のコピー元
    WGPUTextureFormat m_format = WGPUTextureFormat_Undefined;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    bool m_ownsView = true;
};

} // namespace ln::rhi::webgpu
