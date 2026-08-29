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

    VkCommandBuffer vkCommandBuffer() const { return m_cmd; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkCommandBuffer m_cmd = VK_NULL_HANDLE;
    VulkanRenderPass* m_encoder = nullptr; // 所有しない。m_renderPassCache が所有する
    VkFence m_inFlightFences = VK_NULL_HANDLE;

    // アタッチメントレイアウトをキーとする RenderPass オブジェクトのキャッシュ
    std::unordered_map<RenderPassKey, Ref<VulkanRenderPass>, RenderPassKeyHash> m_renderPassCache;
};

} // namespace ln::rhi::vulkan
