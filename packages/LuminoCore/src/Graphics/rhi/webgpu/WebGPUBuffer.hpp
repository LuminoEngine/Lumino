#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>
#include <vector>

namespace ln::rhi::webgpu {

class WebGPUDevice;

class WebGPUBuffer final : public Buffer {
public:
    WebGPUBuffer();
    VoidResult init(WebGPUDevice* device, const BufferDesc& desc);

    u64 size() const override { return m_size; }
    void* map() override;
    void unmap() override;

    WGPUBuffer handle() const { return m_buffer; }

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUBuffer m_buffer = nullptr;
    u64 m_size = 0;
    std::vector<uint8_t> m_shadow;
};

} // namespace ln::rhi::webgpu
