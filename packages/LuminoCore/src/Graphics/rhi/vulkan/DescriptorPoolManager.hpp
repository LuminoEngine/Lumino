#pragma once

/**
 * @file DescriptorPoolManager.hpp
 * 拡張可能な Vulkan ディスクリプタプールの集合。
 *
 * 固定サイズの VkDescriptorPool が 1 つだけだと、BindGroup を多数作成した場合
 * (ドローコールごと、マテリアルごとなど) に溢れてしまう。このマネージャは
 * VK_ERROR_OUT_OF_POOL_MEMORY が返されるたびに新しいプールを透過的に追加するため、
 * 呼び出し側がプールの枯渇を手動で処理する必要はない。
 *
 * BindGroup の破棄時に個々のディスクリプタセットを解放できるよう、プールは
 * VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT を付けて作成する。
 */

#include <LuminoBase/Types.hpp>

#include <utility>
#include <vector>
#include "VulkanLoader.hpp"

namespace ln::rhi::vulkan {

class DescriptorPoolManager {
public:
    /**
     * プールページあたりのディスクリプタセット数。典型的な使用量
     * (セットあたり 1-4 個) をまかなうため、各ページは各タイプにつき
     * SETS_PER_POOL * 4 個のディスクリプタも確保する。
     */
    static constexpr uint32_t SETS_PER_POOL = 64;

    void init(VkDevice device) {
        m_device = device;
        allocateNewPool();
    }

    void destroy() {
        for (auto pool : m_pools) {
            vkDestroyDescriptorPool(m_device, pool, nullptr);
        }
        m_pools.clear();
    }

    /**
     * `layout` からディスクリプタセットを 1 つ確保する。
     * {pool, set} を返す。失敗時は両方のハンドルが VK_NULL_HANDLE になる。
     */
    std::pair<VkDescriptorPool, VkDescriptorSet> allocate(VkDescriptorSetLayout layout) {
        for (;;) {
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = m_pools.back();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &layout;

            VkDescriptorSet set = VK_NULL_HANDLE;
            VkResult result = vkAllocateDescriptorSets(m_device, &allocInfo, &set);
            if (result == VK_SUCCESS) {
                return {m_pools.back(), set};
            }
            if (result == VK_ERROR_OUT_OF_POOL_MEMORY ||
                result == VK_ERROR_FRAGMENTED_POOL) {
                allocateNewPool();
                continue;
            }
            // 想定外のエラー - null ハンドルを返す。
            return {VK_NULL_HANDLE, VK_NULL_HANDLE};
        }
    }

private:
    VkDevice m_device = VK_NULL_HANDLE;
    std::vector<VkDescriptorPool> m_pools;

    void allocateNewPool() {
        VkDescriptorPoolSize poolSizes[] = {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          SETS_PER_POOL * 4},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,  SETS_PER_POOL * 2},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,           SETS_PER_POOL * 4},
            {VK_DESCRIPTOR_TYPE_SAMPLER,                 SETS_PER_POOL * 4},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,          SETS_PER_POOL},
        };

        VkDescriptorPoolCreateInfo dpInfo{};
        dpInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        // FREE_DESCRIPTOR_SET_BIT により個々のセットを解放できる。
        dpInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        dpInfo.maxSets = SETS_PER_POOL;
        dpInfo.poolSizeCount = 5;
        dpInfo.pPoolSizes = poolSizes;

        VkDescriptorPool pool = VK_NULL_HANDLE;
        vkCreateDescriptorPool(m_device, &dpInfo, nullptr, &pool);
        m_pools.push_back(pool);
    }
};

} // namespace ln::rhi::vulkan
