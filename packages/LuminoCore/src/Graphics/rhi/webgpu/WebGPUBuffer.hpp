#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

class WebGPUDevice;

class WebGPUBuffer final : public Buffer {
public:
    WebGPUBuffer();
    VoidResult init(WebGPUDevice* device, const BufferDesc& desc);

    uint64_t size() const override { return m_size; }

    WGPUBuffer handle() const { return m_buffer; }

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUBuffer m_buffer = nullptr;
    uint64_t m_size = 0;
};

} // namespace ln::rhi::webgpu
