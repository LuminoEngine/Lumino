#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPURenderTarget : public RHIResource {
public:
    WebGPURenderTarget();
    Result resetFromSwapChain(WebGPUDevice* rhiDevice, WGPUTextureView view, int width, int height, TextureFormat format);
    void dispose() override;
	
private:
    WebGPUDevice* m_rhiDevice;
    WGPUTextureView m_wgpuTextureView;
};

} // namespace detail
} // namespace ln
