#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

class WebGPUDevice;

class WebGPUTexture final : public Texture {
public:
    WebGPUTexture();

    /** Create a texture owned by this object. */
    VoidResult init(WebGPUDevice* device, const TextureDesc& desc);

    /** Wrap an externally-owned WGPUTexture (e.g. swap chain back-buffer). */
    void initFromExternal(WGPUTexture texture, TextureFormat format, u32 width, u32 height);

    u32 width() const override { return m_width; }
    u32 height() const override { return m_height; }
    TextureFormat format() const override { return m_format; }
    WGPUTexture handle() const { return m_texture; }

protected:
    void finalize() override;

private:
    WGPUTexture m_texture = nullptr;
    TextureFormat m_format = TextureFormat::RGBA8Unorm;
    u32 m_width = 0;
    u32 m_height = 0;
    bool m_ownsTexture = true;
};

} // namespace ln::rhi::webgpu
