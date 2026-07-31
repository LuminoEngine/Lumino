#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

class WebGPUDevice;

class WebGPURenderPipeline final : public RenderPipeline {
public:
    WebGPURenderPipeline();
    VoidResult init(WebGPUDevice* device, const RenderPipelineDesc& desc);
    WGPURenderPipeline handle() const { return m_pipeline; }

    /** WebGPU オブジェクトのラベルに使っている識別名。 */
    const std::string& debugName() const { return m_debugName; }

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPURenderPipeline m_pipeline = nullptr;
    // WGPUStringView はポインタを保持するだけなので、ラベル文字列はここで所有する。
    std::string m_debugName;
};

} // namespace ln::rhi::webgpu
