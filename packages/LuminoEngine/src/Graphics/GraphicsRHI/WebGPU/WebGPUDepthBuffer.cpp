#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDepthBuffer.hpp>

namespace ln {
namespace detail {

WebGPUDepthBuffer::WebGPUDepthBuffer()
    : m_device(nullptr)
    , m_nativeDepthTexture(nullptr)
    , m_nativeTextureView(nullptr)
    , m_nativeFormat(WGPUTextureFormat_Undefined) {
}

MaybeResult_deprecated WebGPUDepthBuffer::init(WebGPUDevice* device, uint32_t width, uint32_t height) {
    auto result = RHIResource::initAsDepthBuffer(width, height, true);
    if (!result) return result;

    m_device = device;
    m_nativeFormat = WGPUTextureFormat_Depth24PlusStencil8;

    // Create Texture.
    WGPUTextureDescriptor depthTextureDesc = WGPU_TEXTURE_DESCRIPTOR_INIT;
    depthTextureDesc.format = m_nativeFormat;
    depthTextureDesc.mipLevelCount = 1;
    depthTextureDesc.sampleCount = 1;
    depthTextureDesc.size = { width, height, 1 };
    depthTextureDesc.usage = WGPUTextureUsage_RenderAttachment;
    depthTextureDesc.viewFormatCount = 1;
    depthTextureDesc.viewFormats = &m_nativeFormat;
    m_nativeDepthTexture = wgpuDeviceCreateTexture(device->wgpuDevice(), &depthTextureDesc);
    if (!m_nativeDepthTexture) {
        return LN_MAKE_ERROR_deprecated("wgpuDeviceCreateTexture() failed.");
    }

    // Create TextureView.
    WGPUTextureViewDescriptor depthTextureViewDesc = WGPU_TEXTURE_VIEW_DESCRIPTOR_INIT;
    depthTextureViewDesc.aspect = WGPUTextureAspect_All;
    depthTextureViewDesc.baseArrayLayer = 0;
    depthTextureViewDesc.arrayLayerCount = 1;
    depthTextureViewDesc.baseMipLevel = 0;
    depthTextureViewDesc.mipLevelCount = 1;
    depthTextureViewDesc.dimension = WGPUTextureViewDimension_2D;
    depthTextureViewDesc.format = m_nativeFormat;
    m_nativeTextureView = wgpuTextureCreateView(m_nativeDepthTexture, &depthTextureViewDesc);
    if (!m_nativeTextureView) {
        return LN_MAKE_ERROR_deprecated("wgpuTextureCreateView() failed.");
    }

    return LN_MAKE_SUCCESS();
}

void WebGPUDepthBuffer::onDestroy() {
    if (m_nativeTextureView) {
        wgpuTextureViewRelease(m_nativeTextureView);
        m_nativeTextureView = nullptr;
    }
    if (m_nativeDepthTexture) {
        wgpuTextureDestroy(m_nativeDepthTexture);
        wgpuTextureRelease(m_nativeDepthTexture);
        m_nativeDepthTexture = nullptr;
    }
    RHIResource::onDestroy();
}

} // namespace detail
} // namespace ln
