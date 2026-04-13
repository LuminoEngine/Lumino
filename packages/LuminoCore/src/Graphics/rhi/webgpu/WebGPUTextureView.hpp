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
                    WGPUTextureAspect aspect, u32 width, u32 height);

    /** Wrap an externally-owned WGPUTextureView (swap chain back-buffer). */
    void initFromExternal(WGPUTextureView view, WGPUTextureFormat format, u32 width, u32 height);

    /** Replace the wrapped view (called each frame for swap chain textures). */
    void rewrap(WGPUTextureView view);

    WGPUTextureView handle() const { return m_view; }
    WGPUTextureFormat format() const { return m_format; }
    u32 width() const override { return m_width; }
    u32 height() const override { return m_height; }

protected:
    void finalize() override;

private:
    WGPUTextureView m_view = nullptr;
    WGPUTextureFormat m_format = WGPUTextureFormat_Undefined;
    u32 m_width = 0;
    u32 m_height = 0;
    bool m_ownsView = true;
};

} // namespace ln::rhi::webgpu
