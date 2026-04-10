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
    u32 width() const override { return m_width; }
    u32 height() const override { return m_height; }
    u32 maxFramesInFlight() const override { return m_maxFrames; }
    u32 currentFrame() const override { return m_currentFrame; }
    CommandBuffer* getCurrentCommandBuffer() override;

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUSurface m_surface = nullptr;
    WGPUTextureFormat m_surfaceFormat = WGPUTextureFormat_Undefined;
    u32 m_width = 0;
    u32 m_height = 0;
    u32 m_maxFrames = 2;
    u32 m_currentFrame = 0;

    // Per-frame resources
    std::vector<Ref<WebGPUCommandBuffer>> m_commandBuffers;
    Ref<WebGPUTextureView> m_currentBackbufferView;

    // Current frame's texture (from wgpuSurfaceGetCurrentTexture)
    WGPUTexture m_currentTexture = nullptr;
    WGPUTextureView m_currentTextureView = nullptr;
};

} // namespace ln::rhi::webgpu
