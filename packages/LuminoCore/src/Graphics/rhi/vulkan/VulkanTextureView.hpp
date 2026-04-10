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
        u32 width,
        u32 height);

    VkImageView handle() const { return m_view; }
    VkImage image() const { return m_image; }
    VkFormat vkFormat() const { return m_format; }
    u32 width() const { return m_width; }
    u32 height() const { return m_height; }
    bool isSwapchainBackbuffer() const { return m_isSwapchainBackbuffer; }
    void setIsSwapchainBackbuffer(bool v) { m_isSwapchainBackbuffer = v; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkImageView m_view = VK_NULL_HANDLE;
    VkImage m_image = VK_NULL_HANDLE;
    VkFormat m_format = VK_FORMAT_UNDEFINED;
    u32 m_width = 0, m_height = 0;
    bool m_isSwapchainBackbuffer = false;
};

} // namespace ln::rhi::vulkan
