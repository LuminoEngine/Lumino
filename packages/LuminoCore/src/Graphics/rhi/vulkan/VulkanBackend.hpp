#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include <mutex>
#include "VulkanCommon.hpp"
#include "VulkanCacheKeys.hpp"
#include "DescriptorPoolManager.hpp"
#include "FrameResourceManager.hpp"
#include "StagingBufferPool.hpp"

namespace ln::rhi::vulkan {

// ------ VulkanSampler --------------------------------------------------------------------------------------------------------------

class VulkanSampler final : public Sampler {
public:
    VulkanSampler();
    VoidResult init(VulkanDevice* device, const SamplerDesc& desc);
    VkSampler handle() const { return m_sampler; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkSampler m_sampler = VK_NULL_HANDLE;
};

// ------ VulkanShaderModule ----------------------------------------------------------------------------------------------------

class VulkanShaderModule final : public ShaderModule {
public:
    VulkanShaderModule();
    VoidResult init(VkDevice device, const ShaderModuleDesc& desc);
    VkShaderModule handle() const { return m_module; }
    const u32* spirvData() const { return m_spirv.data(); }
    size_t spirvSizeBytes() const { return m_spirv.size() * sizeof(u32); }

protected:
    void finalize() override;

private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkShaderModule m_module = VK_NULL_HANDLE;
    std::vector<u32> m_spirv;
};

// ------ VulkanBindGroupLayout ----------------------------------------------------------------------------------------------

class VulkanBindGroupLayout final : public BindGroupLayout {
public:
    VulkanBindGroupLayout();
    VoidResult init(VkDevice device, const BindGroupLayoutDesc& desc);
    VkDescriptorSetLayout handle() const { return m_layout; }

    /** Per-binding dynamic offset flags (parallel to desc.entries). */
    const std::vector<bool>& dynamicFlags() const { return m_dynamicFlags; }

protected:
    void finalize() override;

private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
    std::vector<bool> m_dynamicFlags;
};

// ------ VulkanBindGroup ----------------------------------------------------------------------------------------------------------

class VulkanBindGroup final : public BindGroup {
public:
    VulkanBindGroup();
    VoidResult init(VulkanDevice* device, DescriptorPoolManager& poolManager,
                    VulkanBindGroupLayout* layout, const BindGroupDesc& desc);
    VkDescriptorSet handle() const { return m_set; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkDescriptorPool m_pool = VK_NULL_HANDLE; ///< Pool that owns m_set; used for vkFreeDescriptorSets.
    VkDescriptorSet m_set = VK_NULL_HANDLE;
};

// ------ VulkanPipelineLayout ------------------------------------------------------------------------------------------------

class VulkanPipelineLayout final : public PipelineLayout {
public:
    VulkanPipelineLayout();
    VoidResult init(VulkanDevice* device, const PipelineLayoutDesc& desc);
    VkPipelineLayout handle() const { return m_layout; }

    Result<Ref<BindGroup>> createBindGroup(
        u32 setIndex, const std::vector<BindGroupEntry>& entries) override;

protected:
    void finalize() override;

private:
    VulkanDevice* m_vulkanDevice = nullptr;
    VkDevice m_device = VK_NULL_HANDLE;
    VkPipelineLayout m_layout = VK_NULL_HANDLE;
    std::vector<Ref<VulkanBindGroupLayout>> m_bindGroupLayouts;
};

// ------ VulkanRenderPass ----------------------------------------------------------------------------------------------------

class VulkanRenderPass final : public RenderPass {
public:
    VulkanRenderPass(VkRenderPass handle, const RenderPassLayoutDesc& desc)
        : m_vkRenderPass(handle), m_desc(desc) {}
    ~VulkanRenderPass() override = default;

    const RenderPassLayoutDesc& layoutDesc() const override { return m_desc; }
    VkRenderPass handle() const { return m_vkRenderPass; }

    // Called by VulkanCommandBuffer::beginRenderPass to activate encoding
    void beginEncoding(VkCommandBuffer cmd, VkFramebuffer framebuffer,
                       VkExtent2D extent, const RenderPassDesc& desc);

    // Encoding methods
    void setPipeline(RenderPipeline* pipeline) override;
    void setVertexBuffer(u32 slot, Buffer* buffer, u64 offset) override;
    void setIndexBuffer(Buffer* buffer, IndexFormat format, u64 offset) override;
    void setBindGroup(u32 index, BindGroup* group) override;
    void setBindGroup(u32 index, BindGroup* group,
                      const u32* dynamicOffsets, u32 dynamicOffsetCount) override;
    void setViewport(f32 x, f32 y, f32 w, f32 h, f32 minDepth, f32 maxDepth) override;
    void setScissorRect(u32 x, u32 y, u32 w, u32 h) override;
    void setStencilReference(u32 reference) override;
    void draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) override;
    void drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, i32 baseVertex, u32 firstInstance) override;
    void end() override;

private:
    VkRenderPass m_vkRenderPass;
    RenderPassLayoutDesc m_desc;
    // Encoding state (valid between beginEncoding and end)
    VkCommandBuffer m_cmd = VK_NULL_HANDLE;
    VkPipelineLayout m_currentPipelineLayout = VK_NULL_HANDLE;
};

// ------ VulkanRenderPipeline ------------------------------------------------------------------------------------------------

class VulkanRenderPipeline final : public RenderPipeline {
public:
    VulkanRenderPipeline();
    VoidResult init(VulkanDevice* device, VkRenderPass renderPass, const RenderPipelineDesc& desc);
    VkPipeline handle() const { return m_pipeline; }
    VkPipelineLayout layoutHandle() const { return m_layout; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_layout = VK_NULL_HANDLE;
};

} // namespace ln::rhi::vulkan
