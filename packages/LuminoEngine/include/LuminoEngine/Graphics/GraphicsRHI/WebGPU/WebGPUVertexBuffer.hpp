#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUVertexBuffer : public RHIResource {
public:
    WebGPUVertexBuffer();
    MaybeResult init(
        WebGPUDevice* device, GraphicsResourceUsage usage, size_t bufferSize,
        const void* initialData);
    void onDestroy() override;
    void* map() override;
    void unmap() override;

private:
    WebGPUDevice* m_device;
    WGPUBuffer m_nativeBuffer;
    std::vector<uint8_t> m_mappedData;
};

} // namespace detail
} // namespace ln
