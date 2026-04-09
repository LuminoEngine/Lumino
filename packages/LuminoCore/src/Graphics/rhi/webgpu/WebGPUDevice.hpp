#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>

#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

// ------ WebGPUDevice ----------------------------------------------------------------------------------------------------------------

class WebGPUDevice final : public Device {
public:
    WebGPUDevice();
    VoidResult init(const DeviceDesc& desc);

    bool isValid() const { return m_device != nullptr; }

    // RHI interface
    DeviceLimits deviceLimits() const override;
    Result<Ref<SwapChain>> createSwapChain(const SwapChainDesc& desc) override;
    Result<Ref<Buffer>> createBuffer(const BufferDesc& desc) override;
    Result<Ref<Texture>> createTexture(const TextureDesc& desc) override;
    Result<Ref<TextureView>> createTextureView(Texture* texture) override;
    Result<Ref<Sampler>> createSampler(const SamplerDesc& desc) override;
    Result<Ref<ShaderModule>> createShaderModule(const ShaderModuleDesc& desc) override;
    Result<Ref<PipelineLayout>> createPipelineLayout(const PipelineLayoutDesc& desc) override;
    Result<Ref<RenderPipeline>> createRenderPipeline(const RenderPipelineDesc& desc) override;
    VoidResult writeBuffer(Buffer* dst, u64 dstOffset, const void* data, u64 size) override;
    Result<std::vector<uint8_t>> readbackTexture(TextureView* view) override;
    void waitIdle() override;
    Backend backend() const override { return Backend::WebGPU; }

    // Internal accessors
    WGPUInstance wgpuInstance() const { return m_instance; }
    WGPUAdapter wgpuAdapter() const { return m_adapter; }
    WGPUDevice wgpuDevice() const { return m_device; }
    WGPUQueue wgpuQueue() const { return m_queue; }

private:
    void finalize() override;

    WGPUInstance m_instance = nullptr;
    WGPUAdapter  m_adapter  = nullptr;
    WGPUDevice   m_device   = nullptr;
    WGPUQueue    m_queue    = nullptr;
};

} // namespace ln::rhi::webgpu
