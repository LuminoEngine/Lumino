#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanDescriptorPool.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanDescriptor.hpp>

namespace ln {
namespace detail {

VulkanDescriptor3::VulkanDescriptor3(VulkanDescriptorPool3* owner)
    : m_owner(owner) {
}

void VulkanDescriptor3::onUpdateData(const ShaderDescriptorTableUpdateInfo& data) {
    LN_NOTIMPLEMENTED();
}

} // namespace detail
} // namespace ln
