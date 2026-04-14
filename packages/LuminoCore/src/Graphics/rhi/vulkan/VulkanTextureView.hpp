#pragma once
#include "VulkanCommon.hpp"

namespace ln::rhi::vulkan {

class VulkanTextureView final : public TextureView {
public:
    VulkanTextureView();
    VoidResult init(
        VulkanDevice* device,
        VkImage image,
        VkFormat format,
        VkImageAspectFlags aspect,
        uint32_t width,
        uint32_t height);

    VkImageView handle() const { return m_view; }
    VkImage image() const { return m_image; }
    VkFormat vkFormat() const { return m_format; }
    uint32_t width() const override { return m_width; }
    uint32_t height() const override { return m_height; }
    bool isSwapchainBackbuffer() const { return m_isSwapchainBackbuffer; }
    void setIsSwapchainBackbuffer(bool v) { m_isSwapchainBackbuffer = v; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkImageView m_view = VK_NULL_HANDLE;
    VkImage m_image = VK_NULL_HANDLE;
    VkFormat m_format = VK_FORMAT_UNDEFINED;
    uint32_t m_width = 0, m_height = 0;
    bool m_isSwapchainBackbuffer = false;
};

} // namespace ln::rhi::vulkan
