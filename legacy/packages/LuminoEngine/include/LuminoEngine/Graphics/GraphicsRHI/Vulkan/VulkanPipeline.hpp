#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {

// Dynamic としてマークしている state は次の通り。
// - VK_DYNAMIC_STATE_VIEWPORT,
// - VK_DYNAMIC_STATE_SCISSOR,
// - VK_DYNAMIC_STATE_BLEND_CONSTANTS,
// - VK_DYNAMIC_STATE_STENCIL_REFERENCE,
// なお、これらは computeHash に含める必要はない。
class VulkanPipeline : public IPipeline {
public:
    VulkanPipeline();
    Result_deprecated<> init(VulkanDevice* deviceContext, const DevicePipelineCreateInfo& createInfo);
    void onDestroy() override;
    VkPipeline nativePipeline() const { return m_pipeline; }

private:
    Result_deprecated<> createGraphicsPipeline(const DevicePipelineCreateInfo& createInfo);
    Result_deprecated<> createComputePipeline(const DevicePipelineCreateInfo& createInfo);

    VulkanDevice* m_device;
    VulkanRenderPass2* m_ownerRenderPass;
    VkPipeline m_pipeline;
};

} // namespace detail
} // namespace ln
