#pragma once
#include "VulkanCommon.hpp"
#include "VulkanCacheKeys.hpp"

namespace ln::rhi::vulkan {

class VulkanCommandBuffer final : public CommandBuffer {
public:
    VulkanCommandBuffer();
    VoidResult init(VulkanDevice* device, VkCommandBuffer cmd);

    void dispose();

    VoidResult begin();
    RenderPass* beginRenderPass(const RenderPassDesc& desc) override;
    void submit() override;

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkCommandBuffer m_cmd = VK_NULL_HANDLE;
    VulkanRenderPass* m_encoder = nullptr; // non-owning; owned by m_renderPassCache
    VkFence m_inFlightFences = VK_NULL_HANDLE;
    /** Frame index recorded at submit() time; used to schedule deferred cleanup. */
    u32 m_submittedFrame = 0;
    bool m_submitted = false;

    // Cached RenderPass objects keyed by attachment layout
    std::unordered_map<RenderPassKey, Ref<VulkanRenderPass>, RenderPassKeyHash> m_renderPassCache;
};

} // namespace ln::rhi::vulkan
