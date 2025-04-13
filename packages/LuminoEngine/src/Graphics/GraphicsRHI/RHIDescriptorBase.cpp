#include <LuminoEngine/Graphics/GraphicsRHI/RHIDescriptorBase.hpp>

namespace ln {
namespace detail {

IDescriptor::~IDescriptor() {
}

void IDescriptor::setData(const ShaderDescriptorTableUpdateInfo& data) {
    for (int32_t i = 0; i < ShaderDescriptorTableUpdateInfo::MaxElements; i++) {
        if (LN_ENSURE(!data.uniforms[i].stamplerState)) return;
        m_buffers[i].object = data.uniforms[i].object;
        m_buffers[i].offset = data.uniforms[i].offset;

        m_resources[i].object = data.resources[i].object;
        m_resources[i].samplerState = data.resources[i].stamplerState;

        if (LN_ENSURE(!data.samplers[i].object)) return;
        m_samplers[i].samplerState = data.samplers[i].stamplerState;

        if (LN_ENSURE(!data.storages[i].stamplerState)) return;
        m_storages[i].object = data.storages[i].object;
    }

    onUpdateData(data);
}

void IDescriptor::reset() {
    for (int32_t i = 0; i < ShaderDescriptorTableUpdateInfo::MaxElements; i++) {
        m_buffers[i].object = nullptr;
        m_resources[i].object = nullptr;
        m_resources[i].samplerState = nullptr;
        m_samplers[i].samplerState = nullptr;
        m_storages[i].object = nullptr;
    }
}

} // namespace detail
} // namespace ln
