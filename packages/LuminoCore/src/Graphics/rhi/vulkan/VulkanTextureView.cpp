#include "VulkanHelpers.hpp"
#include "VulkanDevice.hpp"
#include "VulkanTextureView.hpp"

namespace ln::rhi::vulkan {

VulkanTextureView::VulkanTextureView() = default;

VoidResult VulkanTextureView::init(
    VulkanDevice* device,
    VkImage image,
    VkFormat format,
    VkImageAspectFlags aspect,
    uint32_t width,
    uint32_t height) {
    m_device = device;
    m_image = image;
    m_format = format;
    m_width = width;
    m_height = height;

    VkImageViewCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = format;
    info.subresourceRange.aspectMask = aspect;
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.layerCount = 1;
    if (m_device->vk().vkCreateImageView(m_device->vkDevice(), &info, nullptr, &m_view) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateImageView failed.");
    }
    return LN_MAKE_SUCCESS();
}

void VulkanTextureView::finalize() {
    const VolkDeviceTable* vk = &m_device->vk();
    VkDevice dev = m_device->vkDevice();
    VkImageView view = m_view;
    m_device->frameResources().queueDelete(m_device->currentFrameIndex(), [vk, dev, view]() {
        if (view) vk->vkDestroyImageView(dev, view, nullptr);
    });
    TextureView::finalize();
}

} // namespace ln::rhi::vulkan
