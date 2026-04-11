#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

class WebGPUDevice;

class WebGPUSampler final : public Sampler {
public:
    WebGPUSampler();
    VoidResult init(WebGPUDevice* device, const SamplerDesc& desc);
    WGPUSampler handle() const { return m_sampler; }

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUSampler m_sampler = nullptr;
};

} // namespace ln::rhi::webgpu
