
#include "Internal.hpp"
#include "VulkanDeviceContext.hpp"
#include "VulkanShaderPass.hpp"
#include "VulkanDescriptorPool.hpp"

namespace ln {
namespace detail {

//=============================================================================
// VulkanDescriptor2

VulkanDescriptor2::VulkanDescriptor2(VulkanDescriptorPool2* pool)
    : m_pool(pool)
    , m_descriptorSets()
{
}
void VulkanDescriptor2::onUpdateData(const ShaderDescriptorTableUpdateInfo& data)
{
    VulkanDevice* device = m_pool->device();
    VulkanShaderPass* shaderPass = m_pool->shaderPass();
    const std::vector<VkWriteDescriptorSet>& writeInfos = shaderPass->submitDescriptorWriteInfo(nullptr, m_descriptorSets, data);

    m_refarencedResourceCount = 0;
    for (const auto& i : data.resources) {
        if (i.object) {
            m_refarencedResources[m_refarencedResourceCount] = i.object;
            m_refarencedResourceCount++;
        }
    }
    for (const auto& i : data.storages) {
        if (i.object) {
            m_refarencedResources[m_refarencedResourceCount] = i.object;
            m_refarencedResourceCount++;
        }
    }

    vkUpdateDescriptorSets(device->vulkanDevice(), static_cast<uint32_t>(writeInfos.size()), writeInfos.data(), 0, nullptr);
}

//=============================================================================
// VulkanDescriptorPool2

VulkanDescriptorPool2::VulkanDescriptorPool2()
	: m_device(nullptr)
	, m_shaderPass(nullptr)
	, m_pages()
	, m_freePages()
	, m_activePage(VK_NULL_HANDLE)
	, m_activePageUsedCount(0)
{
}

bool VulkanDescriptorPool2::init(VulkanDevice* device, VulkanShaderPass* shaderPass)
{
	if (LN_REQUIRE(device)) return false;
	if (LN_REQUIRE(shaderPass)) return false;
    m_device = device;
	m_shaderPass = shaderPass;
	return true;
}

void VulkanDescriptorPool2::dispose()
{
	for (const auto& page : m_pages) {
		vkDestroyDescriptorPool(m_device->vulkanDevice(), page->pool, m_device->vulkanAllocator());
	}
	m_pages.clear();
	m_freePages.clear();
	m_activePage = nullptr;
    IDescriptorPool::dispose();
}

void VulkanDescriptorPool2::reset()
{
	for (const auto& page : m_pages) {
        for (auto& d : page->descriptors) {
            d->reset();
        }
		vkResetDescriptorPool(m_device->vulkanDevice(), page->pool, 0);
		m_freePages.push_back(page.get());
	}

	m_activePage = nullptr;
	m_activePageUsedCount = 0;
}

Result VulkanDescriptorPool2::allocate(IDescriptor** outDescriptor)
{
    if (!m_activePage || m_activePageUsedCount >= MAX_DESCRIPTOR_SET_COUNT)
    {
        // active pool を使い切ったので次の pool を確保

        m_activePage = nullptr;
        if (!m_freePages.empty()) {
            m_activePage = m_freePages.front();
            m_freePages.pop_front();
        }

        if (!m_activePage) {
            std::array<VkDescriptorPoolSize, DescriptorType_Count> poolSizes;
            poolSizes[DescriptorType_UniformBuffer].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSizes[DescriptorType_UniformBuffer].descriptorCount = MAX_DESCRIPTOR_SET_COUNT * MAX_DESCRIPTOR_COUNT2;
            poolSizes[DescriptorType_Texture].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            poolSizes[DescriptorType_Texture].descriptorCount = MAX_DESCRIPTOR_SET_COUNT * MAX_DESCRIPTOR_COUNT2;
            poolSizes[DescriptorType_SamplerState].type = VK_DESCRIPTOR_TYPE_SAMPLER;
            poolSizes[DescriptorType_SamplerState].descriptorCount = MAX_DESCRIPTOR_SET_COUNT * MAX_DESCRIPTOR_COUNT2;
            poolSizes[DescriptorType_UnorderdAccess].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            poolSizes[DescriptorType_UnorderdAccess].descriptorCount = MAX_DESCRIPTOR_SET_COUNT * MAX_DESCRIPTOR_COUNT2;

            VkDescriptorPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.maxSets = MAX_DESCRIPTOR_SET_COUNT * DescriptorType_Count;    // 基本4セットなので3倍 // static_cast<uint32_t>(poolSizes.size());//static_cast<uint32_t>(swapChainImages.size());
            poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
            poolInfo.pPoolSizes = poolSizes.data();

            auto page = std::make_unique<PageInfo>();
            LN_VK_CHECK(vkCreateDescriptorPool(m_device->vulkanDevice(), &poolInfo, m_device->vulkanAllocator(), &page->pool));

            for (int i = 0; i < MAX_DESCRIPTOR_SET_COUNT; i++) {
                page->descriptors[i] = makeRef<VulkanDescriptor2>(this);
            }
            m_activePage = page.get();
            m_pages.push_back(std::move(page));

            // NOTE: 
            // - VkDescriptorPoolSize::descriptorCount は、この Pool 全体としてみて、作り出せる Descriptor の最大数。
            // - poolInfo.maxSets は、この Pool から作り出せる VkDescriptorSet の最大数。
            // この2つに直接的な関連性は無い。
            // ひとつ VkDescriptorSet を作るときに、どの種類の Descriptor をいくつ消費するかは VkDescriptorSetLayout に依る。
            // 例えば VkDescriptorSetLayout が
            // - { .binding = 0, .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER },
            // - { .binding = 1, .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER },
            // という2つのエントリからできているなら、VkDescriptorSet を一つ作ると
            // - VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER から1つ、
            // - VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER から1つ、
            // 計2つの Descriptor を消費する。
            //
            // もし Descriptor が枯渇した場合、vkAllocateDescriptorSets() で次のようにレポートされる。
            // - validation layer : Unable to allocate 1 descriptors of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER from pool 0x29. This pool only has 0 descriptors of this type remaining.The Vulkan spec states : descriptorPool must have enough free descriptor capacity remaining to allocate the descriptor sets of the specified layouts(https ://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkDescriptorSetAllocateInfo-descriptorPool-00307)
            // - validation layer: Unable to allocate 1 descriptors of type VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER from pool 0x29. This pool only has 0 descriptors of this type remaining. The Vulkan spec states: descriptorPool must have enough free descriptor capacity remaining to allocate the descriptor sets of the specified layouts (https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkDescriptorSetAllocateInfo-descriptorPool-00307)
            // 
            // DescriptorSet が枯渇した場合、vkAllocateDescriptorSets() で次のようにレポートされる。
            // - validation layer : Unable to allocate 1 descriptorSets from pool 0x29. This pool only has 0 descriptorSets remaining.The Vulkan spec states : descriptorSetCount must not be greater than the number of sets that are currently available for allocation in descriptorPool(https ://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkDescriptorSetAllocateInfo-descriptorSetCount-00306)
            //
            // [2020/11/25] 対応方針：
            // VkDescriptorPoolSize は固定長ではなく、ShaderPass が持っているレイアウト情報から作る。
            // maxSets は固定長でも構わない。今のように、不足したら Pool 自体を追加していく。



            //std::cout << "vkCreateDescriptorPool" << std::endl;
            /*
            Note:
            poolInfo.maxSets = MAX_DESCRIPTOR_COUNT *3 のままだと、
            GeForce GTX 1060 で vkAllocateDescriptorSets 16 回目くらいで OutOfMemory

            poolInfo.maxSets = MAX_DESCRIPTOR_COUNT にすると、vkAllocateDescriptorSets 11 回目くらいで OutOfMemory

            GeForce GTX 1060:
                descriptorCount=8
                maxSets = MAX_DESCRIPTOR_SET_COUNT * 3
                にすると、5回目の vkAllocateDescriptorSets で OutOfMemory
            */
        }

        m_activePageUsedCount = 0;
    }

    //std::cout << "m_activePageUsedCount:" << m_activePageUsedCount << std::endl;

    VulkanDescriptor2* descriptor = m_activePage->descriptors[m_activePageUsedCount];

    const auto& layout = m_shaderPass->descriptorSetLayouts();

    VkDescriptorSetAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = m_activePage->pool;
    allocInfo.descriptorSetCount = layout.size();
    allocInfo.pSetLayouts = layout.data();
    LN_VK_CHECK(vkAllocateDescriptorSets(m_device->vulkanDevice(), &allocInfo, descriptor->descriptorSets().data()));
    m_activePageUsedCount++;

    *outDescriptor = descriptor;
    return ok();
}

} // namespace detail
} // namespace ln
