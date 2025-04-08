#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUPipeline : public IPipeline {
public:
    WebGPUPipeline();
    ~WebGPUPipeline() override;
    MaybeResult_deprecated init(WebGPUDevice* m_wgpuDevice, const DevicePipelineStateDesc& state);
    void onDestroy() override;

    WGPURenderPipeline nativePipeline() const { return m_nativePipeline; }

private:
    WebGPUDevice* m_wgpuDevice;
    WGPURenderPipeline m_nativePipeline;
};

} // namespace detail
} // namespace ln
