#pragma once

/// @file StagingBufferPool.hpp
/// Host-visible staging buffer pool for synchronous CPU → GPU uploads.
///
/// Device-local buffers (Vertex, Index) cannot be mapped by the CPU.
/// This pool maintains a set of HOST_VISIBLE | HOST_COHERENT staging buffers
/// and provides `uploadImmediate()` to copy data through a one-time command
/// buffer, waiting for completion before returning.
///
/// Freed pages are returned to the pool and reused, avoiding repeated
/// vkAllocateMemory calls.  Phase 1-4 uses synchronous transfer only;
/// async pipelined staging can be added in Phase 2 when needed.

#include <lumino_base/Types.hpp>

#include <cstring>
#include <vector>
#include <vulkan/vulkan.h>

namespace lumino::rhi::vulkan {

class StagingBufferPool {
public:
    void init(VkDevice device, VkPhysicalDevice physicalDevice) {
        device_ = device;
        physicalDevice_ = physicalDevice;
    }

    void destroy() {
        for (auto& page : freePages_) {
            vkDestroyBuffer(device_, page.buffer, nullptr);
            vkFreeMemory(device_, page.memory, nullptr);
        }
        freePages_.clear();
    }

    /// Copy `size` bytes from `data` into `dstBuffer` (device-local).
    /// Allocates or recycles a staging page, records a one-time command
    /// buffer, submits it, and waits for the queue to be idle before returning.
    void uploadImmediate(VkQueue queue, VkCommandPool cmdPool,
                         VkBuffer dstBuffer, const void* data, VkDeviceSize size) {
        Page staging = acquirePage(size);

        // Map and fill the staging buffer.
        void* mapped = nullptr;
        vkMapMemory(device_, staging.memory, 0, size, 0, &mapped);
        std::memcpy(mapped, data, size);
        vkUnmapMemory(device_, staging.memory);

        // Allocate and record a one-time command buffer.
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = cmdPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer cmd = VK_NULL_HANDLE;
        vkAllocateCommandBuffers(device_, &allocInfo, &cmd);

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

        vkFreeCommandBuffers(device_, cmdPool, 1, &cmd);
        releasePage(staging);
    }

private:
    struct Page {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDeviceSize capacity = 0;
    };

    VkDevice device_ = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
    std::vector<Page> freePages_;

    Page acquirePage(VkDeviceSize size) {
        // Reuse any free page that is large enough.
        for (auto it = freePages_.begin(); it != freePages_.end(); ++it) {
            if (it->capacity >= size) {
                Page p = *it;
                freePages_.erase(it);
                return p;
            }
        }
        return createPage(size);
    }

    void releasePage(Page page) {
        freePages_.push_back(page);
    }

    Page createPage(VkDeviceSize size) {
        Page page;
        page.capacity = size;

        VkBufferCreateInfo bufInfo{};
        bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufInfo.size = size;
        bufInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vkCreateBuffer(device_, &bufInfo, nullptr, &page.buffer);

        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(device_, page.buffer, &memReqs);

        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &memProps);

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
        vkAllocateMemory(device_, &allocInfo, nullptr, &page.memory);
        vkBindBufferMemory(device_, page.buffer, page.memory, 0);

        return page;
    }
};

} // namespace lumino::rhi::vulkan
