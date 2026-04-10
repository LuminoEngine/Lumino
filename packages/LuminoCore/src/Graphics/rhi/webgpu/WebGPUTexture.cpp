#include "WebGPUTexture.hpp"
#include "WebGPUDevice.hpp"
#include "WebGPUHelpers.hpp"

namespace ln::rhi::webgpu {

WebGPUTexture::WebGPUTexture() = default;

VoidResult WebGPUTexture::init(WebGPUDevice* device, const TextureDesc& desc) {
    m_width = desc.width;
    m_height = desc.height;
    m_format = desc.format;
    m_ownsTexture = true;

    WGPUTextureDescriptor texDesc = WGPU_TEXTURE_DESCRIPTOR_INIT;
    texDesc.usage = toWGPUTextureUsage(desc.usage);
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.size = {desc.width, desc.height, desc.depthOrArrayLayers};
    texDesc.format = toWGPUTextureFormat(desc.format);
    texDesc.mipLevelCount = desc.mipLevels;
    texDesc.sampleCount = 1;

    m_texture = wgpuDeviceCreateTexture(device->wgpuDevice(), &texDesc);
    if (!m_texture) {
        return LN_MAKE_ERROR("wgpuDeviceCreateTexture failed.");
    }

    return LN_MAKE_SUCCESS();
}

void WebGPUTexture::initFromExternal(WGPUTexture texture, TextureFormat format, u32 width, u32 height) {
    m_texture = texture;
    m_format = format;
    m_width = width;
    m_height = height;
    m_ownsTexture = false;
}

void WebGPUTexture::finalize() {
    if (m_ownsTexture && m_texture) {
        wgpuTextureRelease(m_texture);
    }
    m_texture = nullptr;
    Texture::finalize();
}

} // namespace ln::rhi::webgpu
