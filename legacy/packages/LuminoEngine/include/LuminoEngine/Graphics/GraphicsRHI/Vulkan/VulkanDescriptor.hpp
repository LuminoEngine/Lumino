#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {
class VulkanDescriptorPool3;

class VulkanDescriptor3
    : public IDescriptor
{
public:
    VulkanDescriptor3(VulkanDescriptorPool3* owner);
    void onUpdateData(const ShaderDescriptorTableUpdateInfo& data) override;

private:
    VulkanDescriptorPool3* m_owner;
    //std::array<VkDescriptorSet, kokage::DescriptorType_Count> m_descriptorSets;
};

} // namespace detail
} // namespace ln
