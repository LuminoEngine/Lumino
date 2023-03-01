#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUSwapChain : public ISwapChain {
public:
    // WebGPU ではいくつバッファがあるかは取得できないようだった
    static const int BackbufferCount = 3;
	
    WebGPUSwapChain();
    Result init(WebGPUDevice* device, PlatformWindow* window, const SizeI& backbufferSize);
    void dispose() override;
	
    uint32_t getBackbufferCount() override;
    void acquireNextImage(int* outImageIndex) override;
    RHIResource* getRenderTarget(int imageIndex) const override;
    Result resizeBackbuffer(uint32_t width, uint32_t height) override;
    void present() override;

private:
    WebGPUDevice* m_device;
    WGPUSurface m_wgpuSurface;
    WGPUSwapChain m_wgpuSwapChain;
    TextureFormat m_format;
    int m_width;
    int m_height;
    int m_imageIndex;
    std::array<Ref<WebGPURenderTarget>, 3> m_currentRenderTargets;
};

} // namespace detail
} // namespace ln
