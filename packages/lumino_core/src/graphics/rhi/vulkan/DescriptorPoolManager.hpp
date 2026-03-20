#pragma once

/// @file DescriptorPoolManager.hpp
/// Growing pool of Vulkan descriptor pools.
///
/// A single fixed-size VkDescriptorPool will overflow if many BindGroups are
/// created (e.g., one per draw call, per material, etc.).  This manager
/// transparently adds new pools whenever VK_ERROR_OUT_OF_POOL_MEMORY is
/// returned, so callers never need to handle pool exhaustion manually.
///
/// The pools are created with VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
/// so that individual descriptor sets can be freed on BindGroup destruction.

#include <lumino_base/Types.hpp>

#include <utility>
#include <vector>
#include <vulkan/vulkan.h>

namespace lumino::rhi::vulkan {

class DescriptorPoolManager {
public:
    /// Number of descriptor sets per pool page.  Each page also allocates
    /// SETS_PER_POOL * 4 descriptors of each type to cover typical usage
    /// (1–4 descriptors per set).
    static constexpr u32 SETS_PER_POOL = 64;

    void init(VkDevice device) {
        device_ = device;
        allocateNewPool();
    }

    void destroy() {
        for (auto pool : pools_) {
            vkDestroyDescriptorPool(device_, pool, nullptr);
        }
        pools_.clear();
    }

    /// Allocate one descriptor set from `layout`.
    /// Returns {pool, set}.  On failure both handles are VK_NULL_HANDLE.
    std::pair<VkDescriptorPool, VkDescriptorSet> allocate(VkDescriptorSetLayout layout) {
        for (;;) {
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = pools_.back();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &layout;

            VkDescriptorSet set = VK_NULL_HANDLE;
            VkResult result = vkAllocateDescriptorSets(device_, &allocInfo, &set);
            if (result == VK_SUCCESS) {
                return {pools_.back(), set};
            }
            if (result == VK_ERROR_OUT_OF_POOL_MEMORY ||
                result == VK_ERROR_FRAGMENTED_POOL) {
                allocateNewPool();
                continue;
            }
            // Unexpected error — return null handles.
            return {VK_NULL_HANDLE, VK_NULL_HANDLE};
        }
    }

private:
    VkDevice device_ = VK_NULL_HANDLE;
    std::vector<VkDescriptorPool> pools_;

    void allocateNewPool() {
        VkDescriptorPoolSize poolSizes[] = {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,  SETS_PER_POOL * 4},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,   SETS_PER_POOL * 4},
            {VK_DESCRIPTOR_TYPE_SAMPLER,         SETS_PER_POOL * 4},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,  SETS_PER_POOL},
        };

        VkDescriptorPoolCreateInfo dpInfo{};
        dpInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        // FREE_DESCRIPTOR_SET_BIT allows individual set deallocation.
        dpInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        dpInfo.maxSets = SETS_PER_POOL;
        dpInfo.poolSizeCount = 4;
        dpInfo.pPoolSizes = poolSizes;

        VkDescriptorPool pool = VK_NULL_HANDLE;
        vkCreateDescriptorPool(device_, &dpInfo, nullptr, &pool);
        pools_.push_back(pool);
    }
};

} // namespace lumino::rhi::vulkan
