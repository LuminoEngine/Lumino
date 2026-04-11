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

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPURenderPipeline m_pipeline = nullptr;
};

} // namespace ln::rhi::webgpu
