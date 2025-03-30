#pragma once

#if defined(LN_OS_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#define VK_NO_PROTOTYPES 1
#include <vulkan/vulkan.h>

#include <LuminoEngine/Graphics/GraphicsRHI/GraphicsDeviceContext.hpp>

namespace ln {
namespace detail {

class VulkanSwapChain;
class VulkanRenderTarget;

} // namespace detail
} // namespace ln
