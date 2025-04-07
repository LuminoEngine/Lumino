#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDescriptor.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDescriptorPool.hpp>

namespace ln {
namespace detail {

WebGPUDescriptorPool::WebGPUDescriptorPool() {
}

WebGPUDescriptorPool::~WebGPUDescriptorPool() {
}

MaybeResult WebGPUDescriptorPool::init(WebGPUDevice* device, WebGPUShaderPass* shaderPass) {
    return LN_MAKE_SUCCESS();
}

void WebGPUDescriptorPool::reset() {
}

Result<> WebGPUDescriptorPool::allocate(IDescriptor** outDescriptor) {
    if (m_nextDescriptorIndex < m_maxDescriptorCount) {
        auto desc = makeRef<WebGPUDescriptor>(this);
        m_descriptors.push_back(desc);
        *outDescriptor = desc.get();
        m_nextDescriptorIndex++;
    }
    return {};
}

} // namespace detail
} // namespace ln
