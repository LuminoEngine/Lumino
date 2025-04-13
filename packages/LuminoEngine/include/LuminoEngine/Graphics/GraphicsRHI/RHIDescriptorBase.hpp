#pragma once
#include "Common.hpp"
#include "RHIObject.hpp"
#include "RHISamplerStateBase.hpp"

namespace ln {
namespace detail {

struct ShaderDescriptorTableUpdateItem {
    RHIDeviceObject* object; // UniformBuffer, Texture, VertexBuffer, etc..
    ISamplerState* stamplerState;
    size_t offset; // UniformBuffer offset;
};

struct ShaderDescriptorTableUpdateInfo {
    static const int MaxElements = 32;

    // 各要素番号は DataIndex。detail::DescriptorLayout の各メンバと一致する。BindingIndex ではない点に注意。
    std::array<ShaderDescriptorTableUpdateItem, MaxElements> uniforms = {};
    std::array<ShaderDescriptorTableUpdateItem, MaxElements> resources = {};
    std::array<ShaderDescriptorTableUpdateItem, MaxElements> samplers = {};
    std::array<ShaderDescriptorTableUpdateItem, MaxElements> storages = {};
};

class IDescriptor : public RefObject {
public:
    // CombinedSampler の場合、resources() へ Texture と SamplerState を同時にセットすることで、ペアを示す。
    struct Reference {
        Ref<RHIDeviceObject> object;
        Ref<ISamplerState> samplerState;
        size_t offset; // UniformBuffer offset;
    };
    using ReferenceList = std::array<Reference, ShaderDescriptorTableUpdateInfo::MaxElements>;

    void setData(const ShaderDescriptorTableUpdateInfo& data);
    const ReferenceList& buffers() const { return m_buffers; }
    const ReferenceList& resources() const { return m_resources; }
    const ReferenceList& samplers() const { return m_samplers; }
    const ReferenceList& storages() const { return m_storages; }

    const Reference& bufferSlot(kokage::LayoutSlotIndex i) const { return m_buffers[i.i]; }
    const Reference& resourceSlot(kokage::LayoutSlotIndex i) const { return m_resources[i.i]; }
    const Reference& samplerSlot(kokage::LayoutSlotIndex i) const { return m_samplers[i.i]; }
    const Reference& storageSlot(kokage::LayoutSlotIndex i) const { return m_storages[i.i]; }

protected:
    virtual ~IDescriptor();
    virtual void onUpdateData(const ShaderDescriptorTableUpdateInfo& data) = 0;
    void reset();

private:
    // CommandList 実行中の解放を防ぐため、参照を保持する
    ReferenceList m_buffers = {};
    ReferenceList m_resources = {};
    ReferenceList m_samplers = {};
    ReferenceList m_storages = {};
};

} // namespace detail
} // namespace ln
