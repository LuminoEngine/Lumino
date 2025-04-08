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
    RHIRef<RHIBitmap> readData() override;

    void wrapTextureView(WGPUTexture nativeTexture, WGPUTextureView nativeTextureView);

    WGPUTexture nativeTexture() const { return m_nativeTexture; }
    WGPUTextureView wgpuTextureView() const { return m_nativeTextureView; }
    WGPUTextureFormat nativeFormat() const { return m_nativeFormat; }

private:
    WebGPUDevice* m_rhiDevice;
    WGPUTexture m_nativeTexture;
    WGPUTextureView m_nativeTextureView;
    WGPUTextureFormat m_nativeFormat;
};

} // namespace detail
} // namespace ln
