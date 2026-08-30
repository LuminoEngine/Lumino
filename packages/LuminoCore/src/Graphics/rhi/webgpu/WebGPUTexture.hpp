#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

class WebGPUDevice;

class WebGPUTexture final : public Texture {
public:
    WebGPUTexture();

    /** このオブジェクトが所有するテクスチャを作成する。 */
    VoidResult init(WebGPUDevice* device, const TextureDesc& desc);

    uint32_t width() const override { return m_width; }
    uint32_t height() const override { return m_height; }
    TextureFormat format() const override { return m_format; }
    WGPUTexture handle() const { return m_texture; }

protected:
    void finalize() override;

private:
    WGPUTexture m_texture = nullptr;
    TextureFormat m_format = TextureFormat::RGBA8Unorm;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
};

} // namespace ln::rhi::webgpu
