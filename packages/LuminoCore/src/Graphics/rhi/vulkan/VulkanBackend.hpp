#pragma once
#include <vector>
#include "VulkanCommon.hpp"
#include "DescriptorPoolManager.hpp"

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

    /** バインディングごとの動的オフセットフラグ (desc.entries と同じ並び)。 */
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
                    VulkanBindGroupLayout* layout,
                    const BindGroupEntry* entries, size_t entryCount);
    VkDescriptorSet handle() const { return m_set; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkDescriptorPool m_pool = VK_NULL_HANDLE; ///< m_set を所有するプール。vkFreeDescriptorSets に使う。
    VkDescriptorSet m_set = VK_NULL_HANDLE;
};

// ------ VulkanPipelineLayout ------------------------------------------------------------------------------------------------

class VulkanPipelineLayout final : public PipelineLayout {
public:
    VulkanPipelineLayout();
    VoidResult init(VulkanDevice* device, const PipelineLayoutDesc& desc);
    VkPipelineLayout handle() const { return m_layout; }

    Result<Ref<BindGroup>> createBindGroup(
        uint32_t setIndex, const BindGroupEntry* entries, size_t entryCount) override;

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
    VulkanRenderPass(VkRenderPass handle, const RenderPassLayoutDesc& desc)
        : m_vkRenderPass(handle), m_desc(desc) {}
    ~VulkanRenderPass() override = default;

    const RenderPassLayoutDesc& layoutDesc() const override { return m_desc; }
    VkRenderPass handle() const { return m_vkRenderPass; }

    // VulkanCommandBuffer::beginRenderPass から呼ばれ、エンコードを開始する
    void beginEncoding(VkCommandBuffer cmd, VkFramebuffer framebuffer,
                       VkExtent2D extent, const RenderPassDesc& desc);

    // エンコード用メソッド
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
    VkRenderPass m_vkRenderPass;
    RenderPassLayoutDesc m_desc;
    // エンコード状態 (beginEncoding から end の間だけ有効)
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
