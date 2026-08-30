#include <cstring>
#include "VulkanDevice.hpp"
#include "StagingBufferPool.hpp"

namespace ln::rhi::vulkan {

// イメージ全体 (mip 0, layer 0) を指すサブリソース範囲。
static VkImageSubresourceRange colorSubresourceRange() {
    VkImageSubresourceRange range{};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.levelCount = 1;
    range.layerCount = 1;
    return range;
}

// 隙間なく詰めたイメージ全体のコピー範囲。
static VkBufferImageCopy fullImageCopyRegion(uint32_t width, uint32_t height) {
    VkBufferImageCopy region{};
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.layerCount = 1;
    region.imageExtent = {width, height, 1};
    return region;
}

void StagingBufferPool::init(VulkanDevice* device, VkPhysicalDevice physicalDevice) {
    m_device = device;
    m_vkDevice = device->vkDevice();
    m_physicalDevice = physicalDevice;
}

void StagingBufferPool::destroy() {
    for (auto& page : m_freePages) {
        vkDestroyBuffer(m_vkDevice, page.buffer, nullptr);
        vkFreeMemory(m_vkDevice, page.memory, nullptr);
    }
    m_freePages.clear();
}

void StagingBufferPool::uploadImmediate(VkBuffer dstBuffer, const void* data, VkDeviceSize size,
                                        VkDeviceSize dstOffset) {
    Page staging = acquirePage(size);
    writePage(staging, data, size);

    if (auto cmd = m_device->beginSingleTimeCommands()) {
        VkBufferCopy region{0, dstOffset, size};
        vkCmdCopyBuffer(*cmd, staging.buffer, dstBuffer, 1, &region);
        m_device->endSingleTimeCommands(*cmd);
    }
    releasePage(staging);
}

void StagingBufferPool::uploadTextureImmediate(VkImage dstImage, const void* data, VkDeviceSize size,
                                               uint32_t width, uint32_t height) {
    Page staging = acquirePage(size);
    writePage(staging, data, size);

    if (auto cmd = m_device->beginSingleTimeCommands()) {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = dstImage;
        barrier.subresourceRange = colorSubresourceRange();

        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        vkCmdPipelineBarrier(*cmd,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            0, 0, nullptr, 0, nullptr, 1, &barrier);

        VkBufferImageCopy region = fullImageCopyRegion(width, height);
        vkCmdCopyBufferToImage(*cmd, staging.buffer, dstImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(*cmd,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0, 0, nullptr, 0, nullptr, 1, &barrier);

        m_device->endSingleTimeCommands(*cmd);
    }
    releasePage(staging);
}

std::vector<uint8_t> StagingBufferPool::downloadTextureImmediate(
    VkImage srcImage, VkImageLayout currentLayout, uint32_t width, uint32_t height) {
    const VkDeviceSize size = static_cast<VkDeviceSize>(width) * height * 4; // BGRA8 / RGBA8
    Page staging = acquirePage(size);

    if (auto cmd = m_device->beginSingleTimeCommands()) {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = srcImage;
        barrier.subresourceRange = colorSubresourceRange();

        barrier.oldLayout = currentLayout;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        vkCmdPipelineBarrier(*cmd,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            0, 0, nullptr, 0, nullptr, 1, &barrier);

        VkBufferImageCopy region = fullImageCopyRegion(width, height);
        vkCmdCopyImageToBuffer(*cmd, srcImage,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, staging.buffer, 1, &region);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = currentLayout;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        vkCmdPipelineBarrier(*cmd,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            0, 0, nullptr, 0, nullptr, 1, &barrier);

        m_device->endSingleTimeCommands(*cmd);
    }

    // ステージングバッファをマップして CPU 側の vector へコピーする。
    void* mapped = nullptr;
    vkMapMemory(m_vkDevice, staging.memory, 0, size, 0, &mapped);
    std::vector<uint8_t> result(size);
    std::memcpy(result.data(), mapped, size);
    vkUnmapMemory(m_vkDevice, staging.memory);

    releasePage(staging);
    return result;
}

void StagingBufferPool::writePage(const Page& page, const void* data, VkDeviceSize size) {
    void* mapped = nullptr;
    vkMapMemory(m_vkDevice, page.memory, 0, size, 0, &mapped);
    std::memcpy(mapped, data, size);
    vkUnmapMemory(m_vkDevice, page.memory);
}

StagingBufferPool::Page StagingBufferPool::acquirePage(VkDeviceSize size) {
    // 十分な大きさの空きページがあれば再利用する。
    for (auto it = m_freePages.begin(); it != m_freePages.end(); ++it) {
        if (it->capacity >= size) {
            Page p = *it;
            m_freePages.erase(it);
            return p;
        }
    }
    return createPage(size);
}

StagingBufferPool::Page StagingBufferPool::createPage(VkDeviceSize size) {
    Page page;
    page.capacity = size;

    VkBufferCreateInfo bufInfo{};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.size = size;
    bufInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkCreateBuffer(m_vkDevice, &bufInfo, nullptr, &page.buffer);

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(m_vkDevice, page.buffer, &memReqs);

    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProps);

    constexpr VkMemoryPropertyFlags required =
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    uint32_t memTypeIdx = 0;
    for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i) {
        if ((memReqs.memoryTypeBits & (1u << i)) &&
            (memProps.memoryTypes[i].propertyFlags & required) == required) {
            memTypeIdx = i;
            break;
        }
    }

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = memTypeIdx;
    vkAllocateMemory(m_vkDevice, &allocInfo, nullptr, &page.memory);
    vkBindBufferMemory(m_vkDevice, page.buffer, page.memory, 0);

    return page;
}

} // namespace ln::rhi::vulkan
