#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPURenderTarget : public RHIResource {
public:
    WebGPURenderTarget();
    Result_deprecated<> initForSwapChainWrapper(
        WebGPUDevice* rhiDevice,
        int width,
        int height,
        TextureFormat format,
        WGPUTextureFormat nativeFormat);
    void onDestroy() override;
    void wrapTextureView(WGPUTextureView view);

    WGPUTextureView wgpuTextureView() const { return m_wgpuTextureView; }
    WGPUTextureFormat nativeFormat() const { return m_nativeFormat; }

private:
    WebGPUDevice* m_rhiDevice;
    WGPUTextureView m_wgpuTextureView;
    WGPUTextureFormat m_nativeFormat;
};

} // namespace detail
} // namespace ln
