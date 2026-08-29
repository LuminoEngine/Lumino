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
    TextureFormat format() const override;
    VoidResult resize(uint32_t width, uint32_t height) override;
    uint32_t maxFramesInFlight() const override { return m_maxFrames; }
    uint32_t currentFrame() const override { return m_currentFrame; }
    CommandBuffer* getCurrentCommandBuffer() override;

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUSurface m_surface = nullptr;
    WGPUTextureFormat m_surfaceFormat = WGPUTextureFormat_Undefined;
    WGPUTextureUsage m_surfaceUsage = WGPUTextureUsage_RenderAttachment;
    WGPUPresentMode m_presentMode = WGPUPresentMode_Fifo;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    uint32_t m_maxFrames = 2;
    uint32_t m_currentFrame = 0;

    // フレームごとのリソース
    std::vector<Ref<WebGPUCommandBuffer>> m_commandBuffers;
    Ref<WebGPUTextureView> m_currentBackbufferView;

    // 現在フレームのテクスチャ (wgpuSurfaceGetCurrentTexture で取得したもの)
    WGPUTexture m_currentTexture = nullptr;
    WGPUTextureView m_currentTextureView = nullptr;

    // バックバッファのキャプチャ用 (非 Emscripten のみ)。
    // サーフェステクスチャは present 後に破棄され readback できないため、
    // present 直前にここへコピーしておき、readbackTexture はこの永続テクスチャを読む。
    WGPUTexture m_captureTexture = nullptr;

#if !defined(__EMSCRIPTEN__)
    void releaseCaptureTexture();
    VoidResult recreateCaptureTexture();
    void copyBackbufferToCaptureTexture();
#endif
};

} // namespace ln::rhi::webgpu
