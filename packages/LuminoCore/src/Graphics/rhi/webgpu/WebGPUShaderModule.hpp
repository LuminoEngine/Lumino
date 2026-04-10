#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

class WebGPUDevice;

class WebGPUShaderModule final : public ShaderModule {
public:
    WebGPUShaderModule();
    VoidResult init(WebGPUDevice* device, const ShaderModuleDesc& desc);
    WGPUShaderModule handle() const { return m_module; }

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUShaderModule m_module = nullptr;
};

} // namespace ln::rhi::webgpu
