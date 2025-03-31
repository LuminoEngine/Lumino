#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUDepthBuffer : public RHIResource {
public:
    WebGPUDepthBuffer();
    MaybeResult init(WebGPUDevice* device, uint32_t width, uint32_t height);
    void onDestroy();
    WGPUTextureView nativeTextureView() const { return m_nativeTextureView; }

private:
    WebGPUDevice* m_device;
    WGPUTexture m_nativeDepthTexture;
    WGPUTextureView m_nativeTextureView;
};

} // namespace detail
} // namespace ln
