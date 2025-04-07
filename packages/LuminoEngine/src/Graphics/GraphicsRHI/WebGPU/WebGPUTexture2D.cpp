#include <LuminoEngine/Graphics/GraphicsRHI/RHIHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU//WebGPUHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUTexture2D.hpp>

namespace ln {
namespace detail {

WebGPUTexture2D::WebGPUTexture2D()
    : m_device(nullptr)
    , m_nativeTexture(nullptr)
    , m_nativeTextureView(nullptr) {
}

MaybeResult WebGPUTexture2D::init(
    WebGPUDevice* device, GraphicsResourceUsage usage, uint32_t width, uint32_t height,
    TextureFormat requestFormat, bool mipmap, const void* initialData) {
    if (!RHIResource::initAsTexture2D(usage, width, height, requestFormat, mipmap))
        return err();
    m_device = device;



    WGPUTextureFormat nativeFormat = WebGPUHelper::TextureFormatToWGPUTextureFormat(requestFormat);
    WGPUTextureDescriptor textureDesc = WGPU_TEXTURE_DESCRIPTOR_INIT;
    textureDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
    textureDesc.dimension = WGPUTextureDimension_2D;
    textureDesc.size.width = width;
    textureDesc.size.height = height;
    textureDesc.size.depthOrArrayLayers = 1;
    textureDesc.format = nativeFormat;
    textureDesc.mipLevelCount = 1; // TODO:
    textureDesc.sampleCount = 1;
    textureDesc.viewFormatCount = 1;
    textureDesc.viewFormats = &nativeFormat;
    m_nativeTexture = wgpuDeviceCreateTexture(m_device->wgpuDevice(), &textureDesc);
    if (!m_nativeTexture) {
        return LN_MAKE_ERROR("wgpuDeviceCreateTexture Failed.");
    }

    WGPUTextureViewDescriptor textureViewDesc = WGPU_TEXTURE_VIEW_DESCRIPTOR_INIT;
    textureViewDesc.format = textureDesc.format;
    textureViewDesc.dimension = WGPUTextureViewDimension_2D;
    textureViewDesc.baseMipLevel = 0;
    textureViewDesc.mipLevelCount = 1;
    textureViewDesc.baseArrayLayer = 0;
    textureViewDesc.arrayLayerCount = 1;
    textureViewDesc.aspect = WGPUTextureAspect_All;
    textureViewDesc.usage = WGPUTextureUsage_None;
    m_nativeTextureView = wgpuTextureCreateView(m_nativeTexture, &textureViewDesc);
    if (!m_nativeTextureView) {
        return LN_MAKE_ERROR("wgpuTextureCreateView Failed.");
    }
    
    // Upload texture data
    if (initialData) {
        int pixelSize = RHIHelper::getPixelSize(requestFormat);

        WGPUTexelCopyTextureInfo destination = WGPU_TEXEL_COPY_TEXTURE_INFO_INIT;
        destination.texture = m_nativeTexture;
        destination.mipLevel = 0;
        destination.origin.x = 0;
        destination.origin.y = 0;
        destination.origin.z = 0;
        destination.aspect = WGPUTextureAspect_All;

        WGPUTexelCopyBufferLayout layout = WGPU_TEXEL_COPY_BUFFER_LAYOUT_INIT;
        layout.offset = 0;
        layout.bytesPerRow = width * pixelSize;
        layout.rowsPerImage = height;

        WGPUExtent3D size = { (uint32_t)width, (uint32_t)height, 1 };
        wgpuQueueWriteTexture(
            m_device->wgpuQueue(),
            &destination,
            initialData,
            width * height * pixelSize,
            &layout,
            &textureDesc.size);
    }

    return ok();
}

void WebGPUTexture2D::onDestroy() {
    if (m_nativeTextureView) {
        wgpuTextureViewRelease(m_nativeTextureView);
        m_nativeTextureView = nullptr;
    }
    if (m_nativeTexture) {
        wgpuTextureRelease(m_nativeTexture);
        m_nativeTexture = nullptr;
    }
    RHIResource::onDestroy();
}

} // namespace detail
} // namespace ln
