#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    VkSurfaceTransformFlagBitsKHR preTransform;
    VkCompositeAlphaFlagBitsKHR compositeAlpha;
};

class VulkanSwapChain
	: public ISwapChain
{
public:
	VulkanSwapChain();
    Result_deprecated<> init(VulkanDevice* deviceContext, PlatformWindow* window, const SizeI& backbufferSize);
    void onDestroy() override;
    uint32_t getBackbufferCount() override;
    void acquireNextImage(int* outImageIndex) override;
    RHIResource* getRenderTarget(int imageIndex) const override;
    Result_deprecated<> resizeBackbuffer(uint32_t width, uint32_t height) override;
	
    void present();

    VkSwapchainKHR vulkanSwapchain() const { return m_swapchain; }
    VkFormat vulkanSwapchainImageFormat() const { return m_swapchainImageFormat; }
    VkExtent2D vulkanSwapchainExtent() const { return m_swapchainExtent; }
    const std::vector<Ref<VulkanRenderTarget>>& swapchainRenderTargets() const { return m_swapchainRenderTargets; }
    uint32_t imageIndex() const { return m_imageIndex; }

    uint32_t maxFrameCount() const { return m_swapchainRenderTargets.size(); }

    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t requiredWidth, uint32_t requiredHeight);
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
	bool createNativeSwapchain(const SizeI& backbufferSize);
    void cleanupNativeSwapchain();

    VulkanDevice* m_deviceContext;
    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapchain;
    VkQueue m_presentQueue;
    VkFormat m_swapchainImageFormat;
    VkExtent2D m_swapchainExtent;

    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<Ref<VulkanRenderTarget>> m_swapchainRenderTargets;
    uint32_t m_imageIndex;

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    uint32_t m_currentFrame;
};

} // namespace detail
} // namespace ln
