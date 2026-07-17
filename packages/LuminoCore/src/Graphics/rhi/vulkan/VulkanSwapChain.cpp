#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <LuminoBase/Logger.hpp>
#include "VulkanHelpers.hpp"
#include "VulkanBackend.hpp"
#include "VulkanTextureView.hpp"
#include "VulkanDevice.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanSwapChain.hpp"

namespace ln::rhi::vulkan {

VulkanSwapChain::VulkanSwapChain()
    : m_device(nullptr)
//, m_format(TextureFormat::BGRA8Unorm)
{
}

VoidResult VulkanSwapChain::init(VulkanDevice* device, const SwapChainDesc& desc) {
    m_device = device;
    // Create surface
    glfwCreateWindowSurface(
        m_device->instance(),
        static_cast<GLFWwindow*>(desc.nativeWindowHandle),
        nullptr,
        &m_surface);

    // Query surface capabilities
    VkSurfaceCapabilitiesKHR caps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device->physicalDevice(), m_surface, &caps);

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(
        m_device->physicalDevice(),
        m_surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    //m_format = toTextureFormat(surfaceFormat.format);

    m_extent = caps.currentExtent;
    if (m_extent.width == UINT32_MAX) {
        m_extent.width = desc.width;
        m_extent.height = desc.height;
    }

    uint32_t imageCount = caps.minImageCount + 1;
    if (caps.maxImageCount > 0 && imageCount > caps.maxImageCount) imageCount = caps.maxImageCount;

    VkSwapchainCreateInfoKHR swapInfo{};
    swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapInfo.surface = m_surface;
    swapInfo.minImageCount = imageCount;
    swapInfo.imageFormat = surfaceFormat.format;
    swapInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapInfo.imageExtent = m_extent;
    swapInfo.imageArrayLayers = 1;
    swapInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT; // readData できるようにするため、VK_IMAGE_USAGE_TRANSFER_SRC_BIT も指定しておく
    swapInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapInfo.preTransform = caps.currentTransform;
    swapInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapInfo.presentMode = desc.vsync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_MAILBOX_KHR;
    swapInfo.clipped = VK_TRUE;
    vkCreateSwapchainKHR(m_device->vkDevice(), &swapInfo, nullptr, &m_swapchain);

    // Get swap chain images
    vkGetSwapchainImagesKHR(m_device->vkDevice(), m_swapchain, &imageCount, nullptr);
    m_images.resize(imageCount);
    m_maxFrames = imageCount;
    vkGetSwapchainImagesKHR(m_device->vkDevice(), m_swapchain, &imageCount, m_images.data());

    // バックバッファのデフォルトレイアウトは VK_IMAGE_LAYOUT_UNDEFINED であり、
    // RenderPass 側 (VulkanDevice.cpp) は initialLayout=PRESENT_SRC_KHR を要求する。
    // この差分を初回描画前に埋めたいが、presentable image は vkAcquireNextImageKHR で
    // 取得した後でなければレイアウト遷移を含む一切の使用が許可されていない
    // (VUID UNASSIGNED-non-acquired-swapchain-image-used)。
    // そのため init 時に一括遷移するのではなく、acquireNextTexture() で各イメージを
    // 初めて取得したときにのみ UNDEFINED -> PRESENT_SRC_KHR の barrier を積む。
    m_imageLayoutInitialized.assign(m_images.size(), false);

    // Create image views
    VkFormat vkFmt = swapInfo.imageFormat;
    m_views.resize(imageCount);
    for (uint32_t i = 0; i < imageCount; ++i) {
        auto view = Ref<VulkanTextureView>::adopt(new VulkanTextureView());
        if (!view->init(
                m_device,
                m_images[i],
                vkFmt,
                VK_IMAGE_ASPECT_COLOR_BIT,
                m_extent.width,
                m_extent.height)) {
            return LN_MAKE_ERROR("Failed to create swap chain image view.");
        }
        view->setSwapchainBackbufferIndex(i);
        m_views[i] = view;
    }

    // Create CommandBuffers for rendering.
    for (uint32_t i = 0; i < m_maxFrames; ++i) {
        auto commandBuffer = m_device->createCommandBuffer();
        if (!commandBuffer) {
            // TODO: Error handling
            throw std::runtime_error("Failed to create command buffer for swap chain.");
        }
        m_commandBuffers.push_back(*commandBuffer);
    }

    // Create sync objects
    VkSemaphoreCreateInfo semInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

    m_imageAvailableSemaphores.resize(m_maxFrames);
    m_renderFinished.resize(m_maxFrames);
    for (uint32_t i = 0; i < m_maxFrames; ++i) {
        vkCreateSemaphore(m_device->vkDevice(), &semInfo, nullptr, &m_imageAvailableSemaphores[i]);
        vkCreateSemaphore(m_device->vkDevice(), &semInfo, nullptr, &m_renderFinished[i]);
    }
    return LN_MAKE_SUCCESS();
}

void VulkanSwapChain::finalize() {
    auto dev = m_device->vkDevice();
    vkDeviceWaitIdle(dev);

    for (uint32_t i = 0; i < m_maxFrames; ++i) {
        if (m_imageAvailableSemaphores[i])
            vkDestroySemaphore(dev, m_imageAvailableSemaphores[i], nullptr);
        if (m_renderFinished[i]) vkDestroySemaphore(dev, m_renderFinished[i], nullptr);
    }
    m_views.clear();
    if (m_swapchain) vkDestroySwapchainKHR(dev, m_swapchain, nullptr);
    if (m_surface) vkDestroySurfaceKHR(m_device->instance(), m_surface, nullptr);
}

VulkanSwapChain::SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(
    VkPhysicalDevice device,
    VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device,
            surface,
            &presentModeCount,
            details.presentModes.data());
    }

    if (details.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        details.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else {
        details.preTransform = details.capabilities.currentTransform;
    }

    // Find a supported composite alpha mode
    details.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };
    for (uint32_t i = 0; i < sizeof(compositeAlphaFlags) / sizeof(compositeAlphaFlags[0]); i++) {
        if (details.capabilities.supportedCompositeAlpha & compositeAlphaFlags[i]) {
            details.compositeAlpha = compositeAlphaFlags[i];
            break;
        }
    }

    return details;
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

TextureView* VulkanSwapChain::acquireNextTexture() {
    VkDevice vkDevice = m_device->vkDevice();
    if (!m_commandBuffers[m_currentFrame]->begin()) {
        return nullptr;
    }

    // Run deferred cleanups for this frame index now that the GPU is done with it.
    m_device->beginFrame(m_currentFrame);

    // https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Rendering_and_presentation
    VkResult acquireResult = m_device->checkDeviceLost(
        vkAcquireNextImageKHR(
            vkDevice,
            m_swapchain,
            UINT64_MAX,
            m_imageAvailableSemaphores[m_currentFrame],
            VK_NULL_HANDLE,
            &m_imageIndex),
        "vkAcquireNextImageKHR");
    // VK_SUBOPTIMAL_KHR はイメージ取得自体は成功しているため続行する。
    // VK_ERROR_OUT_OF_DATE_KHR / VK_ERROR_SURFACE_LOST_KHR はデバイスロストとは
    // 別事象なので markDeviceLost せず、フレーム失敗として返す (リサイズ対応は別タスク)。
    if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR) {
        LN_LOG_ERROR("[Vulkan] vkAcquireNextImageKHR failed: %d", static_cast<int>(acquireResult));
        return nullptr;
    }

    // 取得したイメージが初めての acquire であれば、RenderPass が要求する
    // initialLayout=PRESENT_SRC_KHR に合わせて UNDEFINED から遷移させる。
    // 2 フレーム目以降は直前の present がイメージを PRESENT_SRC_KHR のまま残すため不要。
    // acquire 後・present 前のこのタイミングで遷移するのは仕様上許可されている。
    if (!m_imageLayoutInitialized[m_imageIndex]) {
        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_images[m_imageIndex];
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = 0;
        vkCmdPipelineBarrier(
            m_commandBuffers[m_currentFrame]->vkCommandBuffer(),
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &barrier);
        m_imageLayoutInitialized[m_imageIndex] = true;
    }

    m_device->setActiveSwapChain(this);
    return m_views[m_imageIndex].get();
}

void VulkanSwapChain::present() {
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores =
        &m_renderFinished[m_currentFrame]; // このセマフォの通知を待ってから実際に present する
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapchain;
    presentInfo.pImageIndices = &m_imageIndex;
    VkResult presentResult = m_device->checkDeviceLost(
        vkQueuePresentKHR(m_device->graphicsQueue(), &presentInfo), "vkQueuePresentKHR");
    if (presentResult != VK_SUCCESS && presentResult != VK_SUBOPTIMAL_KHR) {
        LN_LOG_ERROR("[Vulkan] vkQueuePresentKHR failed: %d", static_cast<int>(presentResult));
    }

    m_currentFrame = (m_currentFrame + 1) % m_maxFrames;
}

VkSemaphore VulkanSwapChain::imageAvailableSemaphore() const {
    return m_imageAvailableSemaphores[m_currentFrame];
}
VkSemaphore VulkanSwapChain::renderFinishedSemaphore() const {
    return m_renderFinished[m_currentFrame];
}

CommandBuffer* VulkanSwapChain::getCurrentCommandBuffer() {
    return m_commandBuffers[m_currentFrame].get();
}

VoidResult VulkanSwapChain::resize(uint32_t /*width*/, uint32_t /*height*/) {
    return LN_MAKE_ERROR("VulkanSwapChain::resize is not yet implemented.");
}


} // namespace ln::rhi::vulkan
