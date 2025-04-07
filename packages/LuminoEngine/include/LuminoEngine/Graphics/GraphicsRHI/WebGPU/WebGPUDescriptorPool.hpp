#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUDescriptorPool : public IDescriptorPool {
public:
    WebGPUDescriptorPool();
    ~WebGPUDescriptorPool() override;
    MaybeResult init(WebGPUDevice* device, WebGPUShaderPass* shaderPass);
    WebGPUDevice* device() const { return m_device; }
    WebGPUShaderPass* shaderPass() const { return m_shaderPass; }

    void reset() override;
    Result<> allocate(IDescriptor** outDescriptor) override;

private:
    WebGPUDevice* m_device;
    WebGPUShaderPass* m_shaderPass;
    std::vector<Ref<WebGPUDescriptor>> m_descriptors;
    int32_t m_nextDescriptorIndex = 0;
    int32_t m_maxDescriptorCount = 0;
};

} // namespace detail
} // namespace ln
