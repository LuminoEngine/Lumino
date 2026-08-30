#include "VulkanHelpers.hpp"

namespace ln::rhi::vulkan {

void VulkanHelpers::logDeviceProperties(VkPhysicalDeviceProperties deviceProperties) {
    LN_LOG_VERBOSE("Vulkan device: %s (API %u.%u.%u)",
        deviceProperties.deviceName,
        VK_VERSION_MAJOR(deviceProperties.apiVersion),
        VK_VERSION_MINOR(deviceProperties.apiVersion),
        VK_VERSION_PATCH(deviceProperties.apiVersion));
    LN_LOG_VERBOSE("  minUniformBufferOffsetAlignment: %llu",
        static_cast<unsigned long long>(deviceProperties.limits.minUniformBufferOffsetAlignment));
    LN_LOG_VERBOSE("  maxUniformBufferRange: %u", deviceProperties.limits.maxUniformBufferRange);
}

} // namespace ln::rhi::vulkan
