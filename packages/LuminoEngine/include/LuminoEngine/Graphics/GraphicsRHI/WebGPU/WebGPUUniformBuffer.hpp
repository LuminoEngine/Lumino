#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUUniformBuffer : public RHIResource {
public:
    WebGPUUniformBuffer();
    MaybeResult init(WebGPUDevice* device, uint32_t size);
    void onDestroy() override;
    //void* map() override;
    //void unmap() override;

private:
    WebGPUDevice* m_device;
    WGPUBuffer m_nativeUniformBuffer;
};

} // namespace detail
} // namespace ln
