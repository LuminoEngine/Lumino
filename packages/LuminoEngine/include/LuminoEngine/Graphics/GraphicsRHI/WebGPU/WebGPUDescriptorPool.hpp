#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUDescriptorPool : public IDescriptorPool {
public:
    WebGPUDescriptorPool();
    ~WebGPUDescriptorPool() override;
    MaybeResult_deprecated init(WebGPUDevice* device, WebGPUShaderPass* shaderPass);
    WebGPUDevice* device() const { return m_device; }
    WebGPUShaderPass* shaderPass() const { return m_shaderPass; }

    void reset() override;
    Result_deprecated<> allocate(IDescriptor** outDescriptor) override;

private:
    WebGPUDevice* m_device;
    WebGPUShaderPass* m_shaderPass;
    std::vector<Ref<WebGPUDescriptor>> m_descriptors;
    int m_descriptorsUsed = 0;
};

} // namespace detail
} // namespace ln
