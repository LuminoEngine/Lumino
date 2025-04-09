#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUDescriptor : public IDescriptor {
public:
    WebGPUDescriptor(WebGPUDescriptorPool* owner);
    void onUpdateData(const ShaderDescriptorTableUpdateInfo& data) override;

    WGPUBindGroup nativeBindGroup() const { return m_nativeBindGroup; }

private:
    WebGPUDescriptorPool* m_owner;
    WGPUBindGroup m_nativeBindGroup;
};

} // namespace detail
} // namespace ln
