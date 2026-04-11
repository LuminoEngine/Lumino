#include "WebGPUTextureView.hpp"
#include "WebGPUDevice.hpp"

namespace ln::rhi::webgpu {

WebGPUTextureView::WebGPUTextureView() = default;

VoidResult WebGPUTextureView::init(
    WebGPUDevice* device, WGPUTexture texture, WGPUTextureFormat format,
    WGPUTextureAspect aspect, u32 width, u32 height)
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
    return LN_MAKE_SUCCESS();
}

void WebGPUTextureView::initFromExternal(WGPUTextureView view, WGPUTextureFormat format, u32 width, u32 height) {
    m_view = view;
    m_format = format;
    m_width = width;
    m_height = height;
    m_ownsView = false;
}

void WebGPUTextureView::rewrap(WGPUTextureView view) {
    if (m_ownsView && m_view) {
        wgpuTextureViewRelease(m_view);
    }
    m_view = view;
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
