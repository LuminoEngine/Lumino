#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUBindGroupCache.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDescriptorPool.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDescriptor.hpp>

namespace ln {
namespace detail {

WebGPUDescriptor::WebGPUDescriptor(WebGPUDescriptorPool* owner) 
    : m_owner(owner) {
}

WebGPUDescriptor::~WebGPUDescriptor() {
}

void WebGPUDescriptor::onUpdateData(const ShaderDescriptorTableUpdateInfo& data) {
    WebGPUBindGroupCache* cache = m_owner->device()->bindGroupCache();
    auto result = cache->getOrCreate(m_owner->shaderPass(), data, &m_nativeBindGroup);
    if (!result) {
        LN_LOG_ERROR("Failed getOrCreate");
        return; // TODO: handle error
    }

}

} // namespace detail
} // namespace ln
