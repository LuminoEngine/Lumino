#pragma once
#include "VulkanLoader.hpp"
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
    VoidResult init(VulkanDevice* device, const ShaderModuleDesc& desc);
    VkShaderModule handle() const { return m_module; }
    const uint32_t* spirvData() const { return m_spirv.data(); }
    size_t spirvSizeBytes() const { return m_spirv.size() * sizeof(uint32_t); }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkShaderModule m_module = VK_NULL_HANDLE;
    std::vector<uint32_t> m_spirv;
};

// ------ VulkanBindGroupLayout ----------------------------------------------------------------------------------------------

class VulkanBindGroupLayout final : public BindGroupLayout {
public:
    VulkanBindGroupLayout();
    VoidResult init(VulkanDevice* device, const BindGroupLayoutDesc& desc);
    VkDescriptorSetLayout handle() const { return m_layout; }

    /** Per-binding dynamic offset flags (parallel to desc.entries). */
    const std::vector<bool>& dynamicFlags() const { return m_dynamicFlags; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
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
        uint32_t setIndex, const std::vector<BindGroupEntry>& entries) override;

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkPipelineLayout m_layout = VK_NULL_HANDLE;
    std::vector<Ref<VulkanBindGroupLayout>> m_bindGroupLayouts;
};

// ------ VulkanRenderPass ----------------------------------------------------------------------------------------------------

class VulkanRenderPass final : public RenderPass {
public:
    VulkanRenderPass(VulkanDevice* device, VkRenderPass handle, const RenderPassLayoutDesc& desc)
        : m_device(device), m_vkRenderPass(handle), m_desc(desc) {}
    ~VulkanRenderPass() override = default;

    const RenderPassLayoutDesc& layoutDesc() const override { return m_desc; }
    VkRenderPass handle() const { return m_vkRenderPass; }

    // Called by VulkanCommandBuffer::beginRenderPass to activate encoding
    void beginEncoding(VkCommandBuffer cmd, VkFramebuffer framebuffer,
                       VkExtent2D extent, const RenderPassDesc& desc);

    // Encoding methods
    void setPipeline(RenderPipeline* pipeline) override;
    void setVertexBuffer(uint32_t slot, Buffer* buffer, uint64_t offset) override;
    void setIndexBuffer(Buffer* buffer, IndexFormat format, uint64_t offset) override;
    void setBindGroup(uint32_t index, BindGroup* group) override;
    void setBindGroup(uint32_t index, BindGroup* group,
                      const uint32_t* dynamicOffsets, uint32_t dynamicOffsetCount) override;
    void setViewport(float x, float y, float w, float h, float minDepth, float maxDepth) override;
    void setScissorRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;
    void setStencilReference(uint32_t reference) override;
    void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
    void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance) override;
    void end() override;

private:
    // コマンド記録に使う関数テーブルを引くためにデバイスを保持する (非所有)。
    VulkanDevice* m_device = nullptr;
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
