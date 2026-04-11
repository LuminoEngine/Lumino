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
    // Always allow CopyDst so we can upload via wgpuQueueWriteTexture.
    texDesc.usage |= WGPUTextureUsage_CopyDst;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.size = {desc.width, desc.height, desc.depthOrArrayLayers};
    texDesc.format = toWGPUTextureFormat(desc.format);
    texDesc.mipLevelCount = desc.mipLevels;
    texDesc.sampleCount = 1;

    m_texture = wgpuDeviceCreateTexture(device->wgpuDevice(), &texDesc);
    if (!m_texture) {
        return LN_MAKE_ERROR("wgpuDeviceCreateTexture failed.");
    }

    // Upload initial pixel data via the queue if provided.
    if (desc.initialData) {
        u32 bpp = 4; // Assume 4 bytes per pixel for common formats.
        if (desc.format == TextureFormat::R8Unorm)          bpp = 1;
        else if (desc.format == TextureFormat::RG8Unorm)    bpp = 2;
        else if (desc.format == TextureFormat::RGBA16Float) bpp = 8;
        else if (desc.format == TextureFormat::RGBA32Float) bpp = 16;

        WGPUTexelCopyTextureInfo dst = WGPU_TEXEL_COPY_TEXTURE_INFO_INIT;
        dst.texture = m_texture;
        dst.mipLevel = 0;
        dst.origin = {0, 0, 0};
        dst.aspect = WGPUTextureAspect_All;

        WGPUTexelCopyBufferLayout layout = WGPU_TEXEL_COPY_BUFFER_LAYOUT_INIT;
        layout.offset = 0;
        layout.bytesPerRow = desc.width * bpp;
        layout.rowsPerImage = desc.height;

        WGPUExtent3D writeSize = {desc.width, desc.height, 1};
        size_t dataSize = static_cast<size_t>(desc.width) * desc.height * bpp;
        wgpuQueueWriteTexture(device->wgpuQueue(), &dst,
                              desc.initialData, dataSize, &layout, &writeSize);
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
