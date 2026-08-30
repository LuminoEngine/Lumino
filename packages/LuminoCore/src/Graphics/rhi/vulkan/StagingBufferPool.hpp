#pragma once

/**
 * @file StagingBufferPool.hpp
 * CPU から GPU への同期アップロード用の、ホスト可視ステージングバッファプール。
 *
 * デバイスローカルなバッファ (Vertex, Index) は CPU からマップできない。
 * このプールは HOST_VISIBLE | HOST_COHERENT なステージングバッファの集合を保持し、
 * ワンタイムのコマンドバッファ経由でデータをコピーして完了を待ってから返る。
 *
 * 解放されたページはプールに戻して再利用し、vkAllocateMemory の繰り返し呼び出しを避ける。
 * 現状は同期転送のみ。非同期パイプライン化したステージングは必要になれば追加できる。
 */

#include <vector>
#include "VulkanCommon.hpp"

namespace ln::rhi::vulkan {

class StagingBufferPool {
public:
    StagingBufferPool() = default;
    StagingBufferPool(const StagingBufferPool&) = delete;
    StagingBufferPool& operator=(const StagingBufferPool&) = delete;

    void init(VulkanDevice* device, VkPhysicalDevice physicalDevice);
    void destroy();

    /** data から size バイトを dstBuffer (デバイスローカル) へコピーし、完了を待つ。 */
    void uploadImmediate(VkBuffer dstBuffer, const void* data, VkDeviceSize size,
                         VkDeviceSize dstOffset = 0);

    /**
     * data から size バイトを dstImage へコピーする。
     * UNDEFINED -> TRANSFER_DST_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL とレイアウトを遷移させる。
     */
    void uploadTextureImmediate(VkImage dstImage, const void* data, VkDeviceSize size,
                                uint32_t width, uint32_t height);

    /**
     * srcImage のピクセルデータを CPU 側のバッファへコピーする。
     * 転送用にレイアウトを遷移し、完了後に currentLayout へ戻す。
     */
    std::vector<uint8_t> downloadTextureImmediate(VkImage srcImage, VkImageLayout currentLayout,
                                                  uint32_t width, uint32_t height);

private:
    struct Page {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDeviceSize capacity = 0;
    };

    Page acquirePage(VkDeviceSize size);
    void releasePage(Page page) { m_freePages.push_back(page); }
    Page createPage(VkDeviceSize size);
    void writePage(const Page& page, const void* data, VkDeviceSize size);

    VulkanDevice* m_device = nullptr; // 所有しない (親の VulkanDevice)
    VkDevice m_vkDevice = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    std::vector<Page> m_freePages;
};

} // namespace ln::rhi::vulkan
