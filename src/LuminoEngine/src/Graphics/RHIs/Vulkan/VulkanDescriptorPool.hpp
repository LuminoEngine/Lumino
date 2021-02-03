﻿
#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {
class VulkanDescriptorPool2;

class VulkanDescriptor2
    : public IDescriptor
{
public:
    VulkanDescriptor2(VulkanDescriptorPool2* pool);
    void setData(const ShaderDescriptorTableUpdateInfo& data) override;

    std::array<VkDescriptorSet, DescriptorType_Count>& descriptorSets() { return m_descriptorSets; }

private:
    VulkanDescriptorPool2* m_pool;
    std::array<VkDescriptorSet, DescriptorType_Count> m_descriptorSets;
};

class VulkanDescriptorPool2
    : public IDescriptorPool
{
public:
    VulkanDescriptorPool2();
    bool init(VulkanDevice* device, VulkanShaderPass* shaderPass);
    void dispose() override;
    void reset() override;
    IDescriptor* allocate() override;

    VulkanDevice* device() const { return m_device; }
    VulkanShaderPass* shaderPass() const { return m_shaderPass; }

private:
    // 一度に使える各種類ごとの Descripter 数。
    // cbuffer だと _Global, LNRenderViewBuffer, LNRenderElementBuffer 等 5,6個。
    // sampler だと DoF がテクスチャ4つくらい使っている。
    // TODO: エンジン内部だけならこれで足りるが、ユーザーがいろいろ拡張し始めるとちょっと危ないので、警告とか入れておきたいところ。
    static const uint32_t MAX_DESCRIPTOR_COUNT2 = 8;

    static const uint32_t MAX_DESCRIPTOR_SET_COUNT = 32;

    struct PageInfo
    {
        VkDescriptorPool pool;
        std::array<Ref<VulkanDescriptor2>, MAX_DESCRIPTOR_SET_COUNT> descriptors;
    };

    VulkanDevice* m_device;
    VulkanShaderPass* m_shaderPass;

    std::vector<std::unique_ptr<PageInfo>> m_pages;		// page instances
    std::deque<PageInfo*> m_freePages;	// page references
    PageInfo* m_activePage;
    uint32_t m_activePageUsedCount;
};

} // namespace detail
} // namespace ln
