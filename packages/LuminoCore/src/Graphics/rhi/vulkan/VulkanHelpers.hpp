#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <vulkan/vulkan.h>

namespace ln::rhi::vulkan {

// ------ VulkanBuffer ----------------------------------------------------------------------------------------------------------------

class VulkanHelpers final {
public:
    static void logDeviceProperties(VkPhysicalDeviceProperties deviceProperties);
};

} // namespace ln::rhi::vulkan
