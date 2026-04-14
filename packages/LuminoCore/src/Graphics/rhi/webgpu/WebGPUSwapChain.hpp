#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>
#include <vector>

namespace ln::rhi::webgpu {

class WebGPUDevice;
class WebGPUTextureView;
class WebGPUCommandBuffer;

class WebGPUSwapChain final : public SwapChain {
public:
    WebGPUSwapChain();
    VoidResult init(WebGPUDevice* device, const SwapChainDesc& desc);

    TextureView* acquireNextTexture() override;
    void present() override;
    uint32_t width() const override { return m_width; }
    uint32_t height() const override { return m_height; }
    uint32_t maxFramesInFlight() const override { return m_maxFrames; }
    uint32_t currentFrame() const override { return m_currentFrame; }
    CommandBuffer* getCurrentCommandBuffer() override;

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUSurface m_surface = nullptr;
    WGPUTextureFormat m_surfaceFormat = WGPUTextureFormat_Undefined;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    uint32_t m_maxFrames = 2;
    uint32_t m_currentFrame = 0;

    // Per-frame resources
    std::vector<Ref<WebGPUCommandBuffer>> m_commandBuffers;
    Ref<WebGPUTextureView> m_currentBackbufferView;

    // Current frame's texture (from wgpuSurfaceGetCurrentTexture)
    WGPUTexture m_currentTexture = nullptr;
    WGPUTextureView m_currentTextureView = nullptr;
};

} // namespace ln::rhi::webgpu
