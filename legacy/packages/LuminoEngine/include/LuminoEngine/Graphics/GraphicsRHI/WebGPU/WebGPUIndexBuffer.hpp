#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUIndexBuffer : public RHIResource {
public:
    WebGPUIndexBuffer();
    MaybeResult_deprecated init(
        WebGPUDevice* device, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount,
        const void* initialData);
    void onDestroy() override;
    void* map() override;
    void unmap() override;

    WGPUBuffer nativeBuffer() const { return m_nativeBuffer; }
    WGPUIndexFormat indexFormat() const { return m_indexFormat; }

private:
    WebGPUDevice* m_device;
    WGPUBuffer m_nativeBuffer;
    WGPUIndexFormat m_indexFormat;
};

} // namespace detail
} // namespace ln
