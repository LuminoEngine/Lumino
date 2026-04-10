#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

class WebGPUDevice;

// ------ WebGPUBindGroupLayout -------------------------------------------------------------------------------------------------

class WebGPUBindGroupLayout final : public BindGroupLayout {
public:
    WebGPUBindGroupLayout();
    VoidResult init(WebGPUDevice* device, const BindGroupLayoutDesc& desc);
    WGPUBindGroupLayout handle() const { return m_layout; }

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUBindGroupLayout m_layout = nullptr;
};

// ------ WebGPUBindGroup -------------------------------------------------------------------------------------------------------

class WebGPUBindGroup final : public BindGroup {
public:
    WebGPUBindGroup();
    VoidResult init(WebGPUDevice* device, WebGPUBindGroupLayout* layout,
                    const std::vector<BindGroupEntry>& entries);
    WGPUBindGroup handle() const { return m_bindGroup; }

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUBindGroup m_bindGroup = nullptr;
};

// ------ WebGPUPipelineLayout --------------------------------------------------------------------------------------------------

class WebGPUPipelineLayout final : public PipelineLayout {
public:
    WebGPUPipelineLayout();
    VoidResult init(WebGPUDevice* device, const PipelineLayoutDesc& desc);
    WGPUPipelineLayout handle() const { return m_layout; }

    Result<Ref<BindGroup>> createBindGroup(
        u32 setIndex, const std::vector<BindGroupEntry>& entries) override;

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUPipelineLayout m_layout = nullptr;
    std::vector<Ref<WebGPUBindGroupLayout>> m_bindGroupLayouts;
};

} // namespace ln::rhi::webgpu
