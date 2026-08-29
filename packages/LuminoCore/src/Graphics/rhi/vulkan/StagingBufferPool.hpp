#pragma once

/**
 * @file StagingBufferPool.hpp
 * CPU から GPU への同期アップロード用の、ホスト可視ステージングバッファプール。
 *
 * デバイスローカルなバッファ (Vertex, Index) は CPU からマップできない。
 * このプールは HOST_VISIBLE | HOST_COHERENT なステージングバッファの集合を保持し、
 * ワンタイムのコマンドバッファ経由でデータをコピーして完了を待ってから返る
 * `uploadImmediate()` を提供する。
 *
 * 解放されたページはプールに戻して再利用し、vkAllocateMemory の繰り返し呼び出しを避ける。
 * Phase 1-4 では同期転送のみ。非同期パイプライン化したステージングは
 * 必要になれば Phase 2 で追加できる。
 */

#include <LuminoBase/Types.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>

#include <cstring>
#include <vector>
#include "VulkanLoader.hpp"

namespace ln::rhi::vulkan {

class StagingBufferPool {
public:
    void init(VkDevice device, VkPhysicalDevice physicalDevice, Device* owner) {
        m_device = device;
        m_physicalDevice = physicalDevice;
        m_owner = owner;
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
     * `data` から `size` バイトを `dstBuffer` (デバイスローカル) へコピーする。
     * ステージングページを確保または再利用し、ワンタイムのコマンドバッファを記録して
     * 送信し、キューがアイドルになるのを待ってから返る。
     */
    void uploadImmediate(VkQueue queue, VkCommandPool cmdPool,
                         VkBuffer dstBuffer, const void* data, VkDeviceSize size,
                         VkDeviceSize dstOffset = 0) {
        Page staging = acquirePage(size);

        // ステージングバッファをマップして書き込む。
        void* mapped = nullptr;
        vkMapMemory(m_device, staging.memory, 0, size, 0, &mapped);
        std::memcpy(mapped, data, size);
        vkUnmapMemory(m_device, staging.memory);

        // ワンタイムのコマンドバッファを確保して記録する。
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

        VkBufferCopy region{0, dstOffset, size};
        vkCmdCopyBuffer(cmd, staging.buffer, dstBuffer, 1, &region);

        vkEndCommandBuffer(cmd);

        // 送信して完了を待つ。
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmd;
        checkQueueResult(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE), "vkQueueSubmit (staging)");
        checkQueueResult(vkQueueWaitIdle(queue), "vkQueueWaitIdle (staging)");

        vkFreeCommandBuffers(m_device, cmdPool, 1, &cmd);
        releasePage(staging);
    }

    /**
     * `data` から `size` バイトを `dstImage` (デバイスローカル) へコピーする。
     * イメージレイアウトを遷移し、ステージングバッファ経由でコピーする。
     */
    void uploadTextureImmediate(VkQueue queue, VkCommandPool cmdPool,
                                VkImage dstImage, const void* data, VkDeviceSize size,
                                uint32_t width, uint32_t height) {
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

        // UNDEFINED -> TRANSFER_DST_OPTIMAL へ遷移
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

        // バッファ -> イメージへコピー
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;   // 隙間なく詰める
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {width, height, 1};
        vkCmdCopyBufferToImage(cmd, staging.buffer, dstImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        // TRANSFER_DST_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL へ遷移
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
        checkQueueResult(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE), "vkQueueSubmit (staging)");
        checkQueueResult(vkQueueWaitIdle(queue), "vkQueueWaitIdle (staging)");

        vkFreeCommandBuffers(m_device, cmdPool, 1, &cmd);
        releasePage(staging);
    }

    /**
     * `srcImage` (デバイスローカル) のピクセルデータを CPU 側のバッファへコピーする。
     * 転送用にイメージレイアウトを遷移し、完了後に元へ戻す。
     */
    std::vector<uint8_t> downloadTextureImmediate(
        VkQueue queue, VkCommandPool cmdPool,
        VkImage srcImage, VkImageLayout currentLayout,
        uint32_t width, uint32_t height, VkFormat /*format*/)
    {
        const VkDeviceSize size = static_cast<VkDeviceSize>(width) * height * 4; // BGRA8 / RGBA8
        Page staging = acquirePage(size);

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

        // currentLayout -> TRANSFER_SRC_OPTIMAL へ遷移
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = currentLayout;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = srcImage;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        vkCmdPipelineBarrier(cmd,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
            0, 0, nullptr, 0, nullptr, 1, &barrier);

        // イメージ -> バッファへコピー
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;   // 隙間なく詰める
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {width, height, 1};
        vkCmdCopyImageToBuffer(cmd, srcImage,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, staging.buffer, 1, &region);

        // TRANSFER_SRC_OPTIMAL -> 元のレイアウトへ遷移
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = currentLayout;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        vkCmdPipelineBarrier(cmd,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            0, 0, nullptr, 0, nullptr, 1, &barrier);

        vkEndCommandBuffer(cmd);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmd;
        checkQueueResult(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE), "vkQueueSubmit (staging)");
        checkQueueResult(vkQueueWaitIdle(queue), "vkQueueWaitIdle (staging)");

        // ステージングバッファをマップして CPU 側の vector へコピーする。
        void* mapped = nullptr;
        vkMapMemory(m_device, staging.memory, 0, size, 0, &mapped);
        std::vector<uint8_t> result(size);
        std::memcpy(result.data(), mapped, size);
        vkUnmapMemory(m_device, staging.memory);

        vkFreeCommandBuffers(m_device, cmdPool, 1, &cmd);
        releasePage(staging);
        return result;
    }

private:
    struct Page {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDeviceSize capacity = 0;
    };

    VkDevice m_device = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    Device* m_owner = nullptr; // 所有しない (親の VulkanDevice)
    std::vector<Page> m_freePages;

    /** キュー操作の結果を検査し、VK_ERROR_DEVICE_LOST であれば owner をロスト状態にする。 */
    void checkQueueResult(VkResult r, const char* what) {
        if (r == VK_ERROR_DEVICE_LOST && m_owner) {
            m_owner->markDeviceLost(what);
        }
    }

    Page acquirePage(VkDeviceSize size) {
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

    void releasePage(Page page) {
        m_freePages.push_back(page);
    }

    Page createPage(VkDeviceSize size) {
        Page page;
        page.capacity = size;

        VkBufferCreateInfo bufInfo{};
        bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufInfo.size = size;
        bufInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
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
