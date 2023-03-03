
#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {


class VulkanShaderPass
    : public IShaderPass
    , public IVulkanInFlightResource
{
public:
    VulkanShaderPass();
    Result<> init(VulkanDevice* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag);
    void onDestroy() override;
    virtual void onBind() override { RefObjectHelper::retain(this); }
    virtual void onUnBind() override { RefObjectHelper::release(this); }

    bool isComputeShader() const { return m_compShaderModule != VK_NULL_HANDLE; }
    VkShaderModule vulkanVertShaderModule() const { return m_vertShaderModule; }
    VkShaderModule vulkanFragShaderModule() const { return m_fragShaderModule; }
    VkShaderModule vulkanCompShaderModule() const { return m_compShaderModule; }
    const std::string& vertEntryPointName() const { return m_vertEntryPointName; }
    const std::string& fragEntryPointName() const { return m_fragEntryPointName; }
    const std::string& compEntryPointName() const { return m_compEntryPointName; }
    VkPipelineLayout vulkanPipelineLayout() const { return m_pipelineLayout; }
    const std::array<VkDescriptorSetLayout, kokage::DescriptorType_Count>& descriptorSetLayouts() const { return m_descriptorSetLayouts; }

    const std::vector<VkWriteDescriptorSet>& submitDescriptorWriteInfo(
        VulkanCommandBuffer* commandBuffer, 
        const std::array<VkDescriptorSet, kokage::DescriptorType_Count>& descriptorSets,
        const ShaderDescriptorTableUpdateInfo& data);


    const VkWriteDescriptorSet& witeInfo(int index) const { return m_descriptorWriteInfo[index]; }

private:
    struct DescriptorInfo2
    {
        VkDescriptorBufferInfo bufferInfo;
        VkDescriptorImageInfo imageInfo;

        DescriptorInfo2()
        {
            bufferInfo.buffer = VK_NULL_HANDLE;
            bufferInfo.offset = 0;
            bufferInfo.range = 0;
            imageInfo.sampler = VK_NULL_HANDLE;
            imageInfo.imageView = VK_NULL_HANDLE;
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        }
    };

    VulkanDevice* m_deviceContext;
    VkShaderModule m_vertShaderModule;
    VkShaderModule m_fragShaderModule;
    VkShaderModule m_compShaderModule;
    std::string m_vertEntryPointName;
    std::string m_fragEntryPointName;
    std::string m_compEntryPointName;
    VkPipelineLayout m_pipelineLayout;
    std::array<VkDescriptorSetLayout, kokage::DescriptorType_Count> m_descriptorSetLayouts;

    std::vector<VkWriteDescriptorSet> m_descriptorWriteInfo;
    std::vector<DescriptorInfo2> m_bufferDescriptorBufferInfo;
    std::vector<DescriptorInfo2> m_textureDescripterImageInfo;
    std::vector<DescriptorInfo2> m_samplerDescripterImageInfo;
    std::vector<DescriptorInfo2> m_storageDescriptorBufferInfo;

    //Ref<VulkanShaderDescriptorTable> m_descriptorTable;
};

//class VulkanShaderDescriptorTable
//    : public RefObject
//{
//public:
//    struct Item
//    {
//        int descriptorWriteInfoIndex = -1;  // index of VulkanShaderPass::m_descriptorWriteInfo
//        int bindingIndex = 0;
//
//        ShaderDescriptorBufferView bufferView;
//
//        VkDescriptorImageInfo imageInfo;
//        Ref<VulkanTexture> texture;
//        Ref<VulkanSamplerState> samplerState;
//    };
//
//    VulkanShaderDescriptorTable();
//    bool init(VulkanDevice* deviceContext, const VulkanShaderPass* ownerPass, const DescriptorLayout* descriptorLayout);
//    void dispose();
//    void setData(const ShaderDescriptorTableUpdateInfo* data);
//
//    const std::vector<Item>& uniforms() const { return m_uniforms; }
//    const std::vector<Item>& resources() const { return m_resources; }
//    const std::vector<Item>& samplers() const { return m_samplers; }
//    const std::vector<Item>& storages() const { return m_storages; }
//
//private:
//
//    std::vector<Item> m_uniforms;   // 'b' register
//    std::vector<Item> m_resources;   // 't' register
//    std::vector<Item> m_samplers;   // 's' register
//    std::vector<Item> m_storages;   // 'u' register
//};
//

} // namespace detail
} // namespace ln
