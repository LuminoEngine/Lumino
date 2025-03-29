#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/RHIProfiler.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanTextures.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanSwapChain.hpp>

namespace ln {
namespace detail {

VulkanSwapChain::VulkanSwapChain()
    : m_swapchain(VK_NULL_HANDLE) {
}

Result<> VulkanSwapChain::init(VulkanDevice* deviceContext, PlatformWindow* window, const SizeI& backbufferSize) {
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    VkDevice device = m_deviceContext->vulkanDevice();

#if defined(LN_OS_WIN32)
    {
        HWND hWnd = (HWND)PlatformSupport::getWin32WindowHandle(window);
        HINSTANCE hInstance = ::GetModuleHandle(NULL);

        VkWin32SurfaceCreateInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        info.pNext = nullptr;
        info.flags = 0;
        info.hinstance = hInstance;
        info.hwnd = hWnd;

        LN_VK_CHECK(vkCreateWin32SurfaceKHR(m_deviceContext->vulkanInstance(), &info, m_deviceContext->vulkanAllocator(), &m_surface));
    }
#else
#error Invalid target.
#endif

    if (!createNativeSwapchain(backbufferSize)) {
        return err();
    }

    m_imageAvailableSemaphores.resize(maxFrameCount());

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for (size_t i = 0; i < maxFrameCount(); i++) {
        LN_VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, m_deviceContext->vulkanAllocator(), &m_imageAvailableSemaphores[i]));
    }
    m_currentFrame = 0;

    return ok();
}

void VulkanSwapChain::onDestroy() {
    VkDevice device = m_deviceContext->vulkanDevice();

    for (auto& x : m_imageAvailableSemaphores) {
        vkDestroySemaphore(device, x, m_deviceContext->vulkanAllocator());
    }
    m_imageAvailableSemaphores.clear();

    cleanupNativeSwapchain();

    if (m_surface) {
        vkDestroySurfaceKHR(m_deviceContext->vulkanInstance(), m_surface, m_deviceContext->vulkanAllocator());
        m_surface = VK_NULL_HANDLE;
    }

    ISwapChain::onDestroy();
}

uint32_t VulkanSwapChain::getBackbufferCount() {
    return m_swapChainImageViews.size();
}

bool VulkanSwapChain::createNativeSwapchain(const SizeI& backbufferSize) {
    if (LN_REQUIRE(!m_swapchain)) return false;

    VkDevice device = m_deviceContext->vulkanDevice();
    // この Swapchain と対応する Surface と互換性がある QueueFamily を選択する
    uint32_t presentQueueFamily = 0;
    if (m_deviceContext->findPresentQueueFamily(m_surface, &presentQueueFamily)) {
        vkGetDeviceQueue(device, presentQueueFamily, 0, &m_presentQueue);
        // TODO: ↑ここのqueueIndex は 0 ではなく、使ってないインデックスにした方が効率良いかも。今だとほとんどの場合、graphicsQueue と同じになる。
    }
    else {
        LN_ERROR("PresentQueueFamily not found.");
        return false;
    }

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_deviceContext->vulkanPhysicalDevice(), m_surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, backbufferSize.width, backbufferSize.height);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.surface = m_surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT; // readData できるようにするため、VK_IMAGE_USAGE_TRANSFER_SRC_BIT も指定しておく

    ///QueueFamilyIndices indices = m_deviceContext->findQueueFamilies(m_deviceContext->vulkanPhysicalDevice());
    uint32_t queueFamilyIndices[] = { m_deviceContext->graphicsQueueFamilyIndex(), presentQueueFamily };

    if (m_deviceContext->graphicsQueueFamilyIndex() != presentQueueFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.preTransform;
    createInfo.compositeAlpha = swapChainSupport.compositeAlpha;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, m_deviceContext->vulkanAllocator(), &m_swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    std::vector<VkImage> swapChainImages;
    {
        vkGetSwapchainImagesKHR(device, m_swapchain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, m_swapchain, &imageCount, swapChainImages.data());

        m_swapchainImageFormat = surfaceFormat.format;
        m_swapchainExtent = extent;
    }

    // 初期状態ではバックバッファのレイアウトは VK_IMAGE_LAYOUT_UNDEFINED となっている。
    // Vulkan-Tutorial では、初回の VkAttachmentDescription::initialLayout
    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        if (!m_deviceContext->transitionImageLayoutImmediately(swapChainImages[i], m_swapchainImageFormat, 1, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)) {
            return false;
        }
    }

    {
        m_swapChainImageViews.resize(swapChainImages.size());

        for (uint32_t i = 0; i < swapChainImages.size(); i++) {
            if (!VulkanHelper::createImageView(m_deviceContext, swapChainImages[i], m_swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, &m_swapChainImageViews[i])) {
                return false;
            }
        }
    }

    m_swapchainRenderTargets.resize(swapChainImages.size());
    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        auto target = makeRef<VulkanRenderTarget>();
        target->initFromSwapchainImage(m_deviceContext, m_swapchainExtent.width, m_swapchainExtent.height, m_swapchainImageFormat, swapChainImages[i], m_swapChainImageViews[i]);
        target->m_device = m_deviceContext;
        target->m_objectId = m_deviceContext->m_objectNextId++;
        m_deviceContext->profiler()->addRenderTarget(target);
        m_swapchainRenderTargets[i] = target;
    }

    return true;
}

void VulkanSwapChain::cleanupNativeSwapchain() {
    VkDevice device = m_deviceContext->vulkanDevice();

    for (auto& iv : m_swapChainImageViews) {
        vkDestroyImageView(device, iv, m_deviceContext->vulkanAllocator());
    }
    m_swapChainImageViews.clear();

    for (auto& x : m_swapchainRenderTargets) {
        x->destroy();
    }
    m_swapchainRenderTargets.clear();

    if (m_swapchain) {
        vkDestroySwapchainKHR(device, m_swapchain, m_deviceContext->vulkanAllocator());
        m_swapchain = VK_NULL_HANDLE;
    }
}

void VulkanSwapChain::acquireNextImage(int* outIndex) {
    VkResult result = vkAcquireNextImageKHR(
        m_deviceContext->vulkanDevice(), vulkanSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &m_imageIndex);

    // 次に present や readData など、この RenderTarget への書き込みコマンドを実行するとき、これを待たなければならない
    m_swapchainRenderTargets[m_imageIndex]->setSwapchainImageAvailableSemaphoreRef(&m_imageAvailableSemaphores[m_currentFrame]);

    *outIndex = m_imageIndex;
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        //recreateSwapChain();
        throw std::runtime_error("failed to acquire swap chain image!");
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
}

RHIResource* VulkanSwapChain::getRenderTarget(int imageIndex) const {
    return m_swapchainRenderTargets[imageIndex];
}

Result<> VulkanSwapChain::resizeBackbuffer(uint32_t width, uint32_t height) {
    vkDeviceWaitIdle(m_deviceContext->vulkanDevice());

    cleanupNativeSwapchain();

    if (!createNativeSwapchain(SizeI(width, height))) {
        return err();
    }

    return ok();
}

void VulkanSwapChain::present() {
    VkSemaphore renderFinishedSemaphore = m_swapchainRenderTargets[m_imageIndex]->renderFinishedSemaphore();
    //m_deviceContext->recodingCommandBuffer()->submit(
    //    m_swapchainRenderTargets[m_imageIndex]->imageAvailableSemaphore(),
    //    renderFinishedSemaphore);

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphore; // このセマフォの通知を待ってから実際に present する

    VkSwapchainKHR swapChains[] = { vulkanSwapchain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &m_imageIndex;

    VkResult result = vkQueuePresentKHR(m_presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        //framebufferResized = false;
        throw std::runtime_error("failed to present swap chain image!");
        // TODO: recreate
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    //// Swap command buffer
    //{
    //    auto t = m_deviceContext->graphicsContext()->recodingCommandBuffer();
    //    m_deviceContext->graphicsContext()->setRecodingCommandBuffer(m_inFlightCommandBuffers[m_currentFrame]);
    //    m_inFlightCommandBuffers[m_currentFrame] = t;
    //}

    m_currentFrame = (m_currentFrame + 1) % maxFrameCount();
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
        else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            bestMode = availablePresentMode;
        }
    }

    return bestMode;
}

VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t requiredWidth, uint32_t requiredHeight) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        VkExtent2D actualExtent = {
            requiredWidth,
            requiredHeight
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
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
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
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

} // namespace detail
} // namespace ln
