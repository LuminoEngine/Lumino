#pragma once

/**
 * @file StagingBufferPool.hpp
 * Host-visible staging buffer pool for synchronous CPU → GPU uploads.
 * 
 * Device-local buffers (Vertex, Index) cannot be mapped by the CPU.
 * This pool maintains a set of HOST_VISIBLE | HOST_COHERENT staging buffers
 * and provides `uploadImmediate()` to copy data through a one-time command
 * buffer, waiting for completion before returning.
 * 
 * Freed pages are returned to the pool and reused, avoiding repeated
 * vkAllocateMemory calls.  Phase 1-4 uses synchronous transfer only;
 * async pipelined staging can be added in Phase 2 when needed.
 */

#include <LuminoBase/Types.hpp>

#include <cstring>
#include <vector>
#include <vulkan/vulkan.h>

namespace ln::rhi::vulkan {

class StagingBufferPool {
public:
    void init(VkDevice device, VkPhysicalDevice physicalDevice) {
        m_device = device;
        m_physicalDevice = physicalDevice;
    }

    StagingBufferPool() = default;
    StagingBufferPool(const StagingBufferPool&) = delete;
    StagingBufferPool& operator=(const StagingBufferPool&) = delete;

    void destroy() {
        for (auto& page : m_freePages) {
            vkDestroyBuffer(m_device, page.buffer, nullptr);
            vkFreeMemory(m_device, page.memory, nullptr);
        }
        m_freePages.clear();
    }

    /**
     * Copy `size` bytes from `data` into `dstBuffer` (device-local).
     * Allocates or recycles a staging page, records a one-time command
     * buffer, submits it, and waits for the queue to be idle before returning.
     */
    void uploadImmediate(VkQueue queue, VkCommandPool cmdPool,
                         VkBuffer dstBuffer, const void* data, VkDeviceSize size) {
        Page staging = acquirePage(size);

        // Map and fill the staging buffer.
        void* mapped = nullptr;
        vkMapMemory(m_device, staging.memory, 0, size, 0, &mapped);
        std::memcpy(mapped, data, size);
        vkUnmapMemory(m_device, staging.memory);

        // Allocate and record a one-time command buffer.
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = cmdPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer cmd = VK_NULL_HANDLE;
        vkAllocateCommandBuffers(m_device, &allocInfo, &cmd);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(cmd, &beginInfo);

        VkBufferCopy region{0, 0, size};
        vkCmdCopyBuffer(cmd, staging.buffer, dstBuffer, 1, &region);

        vkEndCommandBuffer(cmd);

        // Submit and wait for completion.
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmd;
        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(m_device, cmdPool, 1, &cmd);
        releasePage(staging);
    }

    /**
     * Copy `size` bytes from `data` into `dstImage` (device-local).
     * Transitions the image layout and copies via a staging buffer.
     */
    void uploadTextureImmediate(VkQueue queue, VkCommandPool cmdPool,
                                VkImage dstImage, const void* data, VkDeviceSize size,
                                u32 width, u32 height) {
        Page staging = acquirePage(size);

        void* mapped = nullptr;
        vkMapMemory(m_device, staging.memory, 0, size, 0, &mapped);
        std::memcpy(mapped, data, size);
        vkUnmapMemory(m_device, staging.memory);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = cmdPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer cmd = VK_NULL_HANDLE;
        vkAllocateCommandBuffers(m_device, &allocInfo, &cmd);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(cmd, &beginInfo);

        // Transition UNDEFINED → TRANSFER_DST_OPTIMAL
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = dstImage;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        vkCmdPipelineBarrier(cmd,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            0, 0, nullptr, 0, nullptr, 1, &barrier);

        // Copy buffer → image
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;   // tightly packed
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {width, height, 1};
        vkCmdCopyBufferToImage(cmd, staging.buffer, dstImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        // Transition TRANSFER_DST_OPTIMAL → SHADER_READ_ONLY_OPTIMAL
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(cmd,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0, 0, nullptr, 0, nullptr, 1, &barrier);

        vkEndCommandBuffer(cmd);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmd;
        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);

        vkFreeCommandBuffers(m_device, cmdPool, 1, &cmd);
        releasePage(staging);
    }

private:
    struct Page {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDeviceSize capacity = 0;
    };

    VkDevice m_device = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    std::vector<Page> m_freePages;

    Page acquirePage(VkDeviceSize size) {
        // Reuse any free page that is large enough.
        for (auto it = m_freePages.begin(); it != m_freePages.end(); ++it) {
            if (it->capacity >= size) {
                Page p = *it;
                m_freePages.erase(it);
                return p;
            }
        }
        return createPage(size);
    }

    void releasePage(Page page) {
        m_freePages.push_back(page);
    }

    Page createPage(VkDeviceSize size) {
        Page page;
        page.capacity = size;

        VkBufferCreateInfo bufInfo{};
        bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufInfo.size = size;
        bufInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vkCreateBuffer(m_device, &bufInfo, nullptr, &page.buffer);

        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(m_device, page.buffer, &memReqs);

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
        vkAllocateMemory(m_device, &allocInfo, nullptr, &page.memory);
        vkBindBufferMemory(m_device, page.buffer, page.memory, 0);

        return page;
    }
};

} // namespace ln::rhi::vulkan
