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

    /** WebGPU オブジェクトのラベルに使っている識別名。 */
    const std::string& debugName() const { return m_debugName; }

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUShaderModule m_module = nullptr;
    // WGPUStringView はポインタを保持するだけなので、ラベル文字列はここで所有する。
    std::string m_debugName;
};

} // namespace ln::rhi::webgpu
