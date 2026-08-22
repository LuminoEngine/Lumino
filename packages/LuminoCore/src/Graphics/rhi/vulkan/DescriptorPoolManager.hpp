#pragma once

/**
 * @file DescriptorPoolManager.hpp
 * Growing pool of Vulkan descriptor pools.
 * 
 * A single fixed-size VkDescriptorPool will overflow if many BindGroups are
 * created (e.g., one per draw call, per material, etc.).  This manager
 * transparently adds new pools whenever VK_ERROR_OUT_OF_POOL_MEMORY is
 * returned, so callers never need to handle pool exhaustion manually.
 * 
 * The pools are created with VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
 * so that individual descriptor sets can be freed on BindGroup destruction.
 */

#include <LuminoBase/Types.hpp>

#include <utility>
#include <vector>
#include "VulkanLoader.hpp"

namespace ln::rhi::vulkan {

class DescriptorPoolManager {
public:
    /**
     * Number of descriptor sets per pool page.  Each page also allocates
     * SETS_PER_POOL * 4 descriptors of each type to cover typical usage
     * (1-4 descriptors per set).
     */
    static constexpr uint32_t SETS_PER_POOL = 64;

    /**
     * @param vk      デバイス専用の Vulkan 関数テーブル (VulkanDevice が所有)。
     * @param device  対象の VkDevice。
     */
    void init(const VolkDeviceTable* vk, VkDevice device) {
        m_vk = vk;
        m_device = device;
        allocateNewPool();
    }

    void destroy() {
        for (auto pool : m_pools) {
            m_vk->vkDestroyDescriptorPool(m_device, pool, nullptr);
        }
        m_pools.clear();
    }

    /**
     * Allocate one descriptor set from `layout`.
     * Returns {pool, set}.  On failure both handles are VK_NULL_HANDLE.
     */
    std::pair<VkDescriptorPool, VkDescriptorSet> allocate(VkDescriptorSetLayout layout) {
        for (;;) {
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = m_pools.back();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &layout;

            VkDescriptorSet set = VK_NULL_HANDLE;
            VkResult result = m_vk->vkAllocateDescriptorSets(m_device, &allocInfo, &set);
            if (result == VK_SUCCESS) {
                return {m_pools.back(), set};
            }
            if (result == VK_ERROR_OUT_OF_POOL_MEMORY ||
                result == VK_ERROR_FRAGMENTED_POOL) {
                allocateNewPool();
                continue;
            }
            // Unexpected error - return null handles.
            return {VK_NULL_HANDLE, VK_NULL_HANDLE};
        }
    }

private:
    const VolkDeviceTable* m_vk = nullptr; // non-owning (親の VulkanDevice が所有)
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
        // FREE_DESCRIPTOR_SET_BIT allows individual set deallocation.
        dpInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        dpInfo.maxSets = SETS_PER_POOL;
        dpInfo.poolSizeCount = 5;
        dpInfo.pPoolSizes = poolSizes;

        VkDescriptorPool pool = VK_NULL_HANDLE;
        m_vk->vkCreateDescriptorPool(m_device, &dpInfo, nullptr, &pool);
        m_pools.push_back(pool);
    }
};

} // namespace ln::rhi::vulkan
