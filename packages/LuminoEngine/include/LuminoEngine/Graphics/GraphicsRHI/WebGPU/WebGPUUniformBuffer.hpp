#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUUniformBuffer : public RHIResource {
public:
    WebGPUUniformBuffer();
    MaybeResult init(WebGPUDevice* device, uint32_t size);
    void onDestroy() override;
    void* map() override;
    void unmap() override;
    WGPUBuffer nativeBuffer() const { return m_nativeBuffer; }

private:
    WebGPUDevice* m_device;
    WGPUBuffer m_nativeBuffer;
    std::vector<uint8_t> m_mappedData;
};

} // namespace detail
} // namespace ln
