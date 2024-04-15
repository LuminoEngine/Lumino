#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPURenderTarget : public RHIResource {
public:
    WebGPURenderTarget();
    Result<> initForSwapChainWrapper(WebGPUDevice* rhiDevice, int width, int height, TextureFormat format);
    void onDestroy() override;
    void wrapTextureView(WGPUTextureView view);

	WGPUTextureView wgpuTextureView() const { return m_wgpuTextureView; }
	
private:
    WebGPUDevice* m_rhiDevice;
    WGPUTextureView m_wgpuTextureView;
};

} // namespace detail
} // namespace ln
