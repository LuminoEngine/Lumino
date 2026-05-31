#include "WebGPUTextureView.hpp"
#include "WebGPUDevice.hpp"

namespace ln::rhi::webgpu {

WebGPUTextureView::WebGPUTextureView() = default;

VoidResult WebGPUTextureView::init(
    WebGPUDevice* device, WGPUTexture texture, WGPUTextureFormat format,
    WGPUTextureAspect aspect, uint32_t width, uint32_t height)
{
    m_format = format;
    m_width = width;
    m_height = height;
    m_ownsView = true;

    WGPUTextureViewDescriptor viewDesc = WGPU_TEXTURE_VIEW_DESCRIPTOR_INIT;
    viewDesc.format = format;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.baseMipLevel = 0;
    viewDesc.mipLevelCount = 1;
    viewDesc.baseArrayLayer = 0;
    viewDesc.arrayLayerCount = 1;
    viewDesc.aspect = aspect;

    m_view = wgpuTextureCreateView(texture, &viewDesc);
    if (!m_view) {
        return LN_MAKE_ERROR("wgpuTextureCreateView failed.");
    }
    // readback でコピー元として使えるよう、元テクスチャを保持する (非所有)。
    m_sourceTexture = texture;
    return LN_MAKE_SUCCESS();
}

void WebGPUTextureView::initFromExternal(WGPUTextureView view, WGPUTextureFormat format, uint32_t width, uint32_t height) {
    m_view = view;
    m_format = format;
    m_width = width;
    m_height = height;
    m_ownsView = false;
}

void WebGPUTextureView::rewrap(WGPUTextureView view, WGPUTexture sourceTexture) {
    if (m_ownsView && m_view) {
        wgpuTextureViewRelease(m_view);
    }
    m_view = view;
    m_sourceTexture = sourceTexture;
    m_ownsView = false;
}

void WebGPUTextureView::finalize() {
    if (m_ownsView && m_view) {
        wgpuTextureViewRelease(m_view);
    }
    m_view = nullptr;
    TextureView::finalize();
}

} // namespace ln::rhi::webgpu
