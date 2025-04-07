#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDescriptor.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDescriptorPool.hpp>

namespace ln {
namespace detail {

WebGPUDescriptorPool::WebGPUDescriptorPool() {
}

WebGPUDescriptorPool::~WebGPUDescriptorPool() {
}

MaybeResult WebGPUDescriptorPool::init(WebGPUDevice* device, WebGPUShaderPass* shaderPass) {
    m_device = device;
    m_shaderPass = shaderPass;
    return LN_MAKE_SUCCESS();
}

void WebGPUDescriptorPool::reset() {
    m_descriptorsUsed = 0;
}

Result<> WebGPUDescriptorPool::allocate(IDescriptor** outDescriptor) {
    if (m_descriptorsUsed >= m_descriptors.size()) {
        auto desc = makeRef<WebGPUDescriptor>(this);
        m_descriptors.push_back(desc);
        *outDescriptor = desc.get();
        m_descriptorsUsed++;
    }
    else {
        *outDescriptor = m_descriptors[m_descriptorsUsed].get();
        m_descriptorsUsed++;
    }
    return {};
}

} // namespace detail
} // namespace ln
