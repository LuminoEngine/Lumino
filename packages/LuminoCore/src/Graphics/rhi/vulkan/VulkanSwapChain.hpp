#pragma once
#include "VulkanCommon.hpp"

namespace ln::rhi::vulkan {

class VulkanSwapChain final : public SwapChain {
public:
    VulkanSwapChain();
    VoidResult init(VulkanDevice* device, const SwapChainDesc& desc);

    TextureView* acquireNextTexture() override;
    void present() override;
    uint32_t width() const override { return m_extent.width; }
    uint32_t height() const override { return m_extent.height; }
    VoidResult resize(uint32_t width, uint32_t height) override;
    uint32_t maxFramesInFlight() const override { return static_cast<uint32_t>(m_maxFrames); }
    //TextureFormat format() const override { return m_format; }

    VkSemaphore imageAvailableSemaphore() const;
    VkSemaphore renderFinishedSemaphore() const;
    uint32_t currentImageIndex() const { return m_imageIndex; }
    uint32_t currentFrame() const { return m_currentFrame; }
    CommandBuffer* getCurrentCommandBuffer() override;

public:
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;

        VkSurfaceTransformFlagBitsKHR preTransform;
        VkCompositeAlphaFlagBitsKHR compositeAlpha;
    };
    static SwapChainSupportDetails querySwapChainSupport(
        VkPhysicalDevice device,
        VkSurfaceKHR surface);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);

    void finalize() override;

    VulkanDevice* m_device;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
    VkExtent2D m_extent{};
    //TextureFormat m_format;

    int m_maxFrames;
    std::vector<VkImage> m_images;
    std::vector<Ref<VulkanTextureView>> m_views;
    std::vector<Ref<VulkanCommandBuffer>> m_commandBuffers;
    // NOTE: なぜ SwapChain に CommandBuffer を持たせるのか？
    //   https://webgpufundamentals.org/webgpu/lessons/ja/webgpu-multiple-canvases.html
    //   コチラを見ると、 WebGPU ではひとつの CommandEncoder を複数の SwapChain で共有できるるように見える。
    //   ただしこれをやろうとすると、 CommandBuffer に RenderTarget として使われた Context を、
    //   次の present 対象として覚えておく必要があるなど、管理が複雑になる。

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinished;
    uint32_t m_currentFrame = 0;
    uint32_t m_imageIndex = 0;
};

} // namespace ln::rhi::vulkan
