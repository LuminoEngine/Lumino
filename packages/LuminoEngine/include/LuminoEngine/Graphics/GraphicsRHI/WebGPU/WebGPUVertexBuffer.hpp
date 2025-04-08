#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUVertexBuffer : public RHIResource {
public:
    WebGPUVertexBuffer();
    MaybeResult_deprecated init(
        WebGPUDevice* device, GraphicsResourceUsage usage, size_t bufferSize,
        const void* initialData);
    void onDestroy() override;
    void* map() override;
    void unmap() override;

    WGPUBuffer nativeBuffer() const { return m_nativeBuffer; }

private:
    WebGPUDevice* m_device;
    WGPUBuffer m_nativeBuffer;
};

} // namespace detail
} // namespace ln
