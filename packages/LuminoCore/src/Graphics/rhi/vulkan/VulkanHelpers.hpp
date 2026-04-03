#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <vulkan/vulkan.h>

#define LN_MAKE_VULKAN_ERROR(result, func) LN_MAKE_ERROR("Failed: " func "(%d)", result)

namespace ln::rhi::vulkan {

// ------ VulkanBuffer ----------------------------------------------------------------------------------------------------------------

class VulkanHelpers final {
public:
    static void logDeviceProperties(VkPhysicalDeviceProperties deviceProperties);
};

} // namespace ln::rhi::vulkan
