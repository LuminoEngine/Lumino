
#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "GraphicsDeviceContext.hpp"
#include "MixHash.hpp"
#include "../../LuminoCore/src/Base/LinearAllocator.hpp"

namespace ln {
namespace detail {
class VulkanDeviceContext;
class VulkanShaderPass;
class VulkanDescriptorSetsPool;

#define LN_VK_CHECK(f) \
{ \
    VkResult r = (f); \
	if (r != VK_SUCCESS) { \
        LN_LOG_ERROR << #f << "; VkResult:" << r << "(" << VulkanHelper::getVkResultName(r) << ")"; \
		return false; \
	} \
}

class VulkanHelper
{
public:
    static const std::vector<const char*> VulkanHelper::validationLayers;

	static VkFormat LNFormatToVkFormat(TextureFormat format);
	static TextureFormat VkFormatToLNFormat(VkFormat format);
	static VkBlendFactor LNBlendFactorToVkBlendFactor_Color(BlendFactor value);
	static VkBlendFactor LNBlendFactorToVkBlendFactor_Alpha(BlendFactor value);
	static VkBlendOp LNBlendOpToVkBlendOp(BlendOp value);
	static VkCompareOp LNComparisonFuncToVkCompareOp(ComparisonFunc value);
	static VkPolygonMode LNFillModeToVkPolygonMode(FillMode value);
	static VkCullModeFlagBits LNCullModeToVkCullMode(CullMode value);
	static VkStencilOp LNStencilOpToVkStencilOp(StencilOp value);
	static VkFilter LNTextureFilterModeToVkFilter(TextureFilterMode value);
	static VkSamplerAddressMode LNTextureAddressModeModeToVkSamplerAddressMode(TextureAddressMode value);

	static VkFormat LNVertexElementTypeToVkFormat(VertexElementType value);
    static const char* getVkResultName(VkResult result);
    static bool hasStencilComponent(VkFormat format) { return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT; }
    static bool checkValidationLayerSupport();

};

// VkAllocationCallbacks の本体。
// インスタンスを VkAllocationCallbacks::pUserData に登録し、static コールバックからメンバをコールする。
class AbstractVulkanAllocator
{
public:
	AbstractVulkanAllocator();
	Result init();
	const VkAllocationCallbacks* vulkanAllocator() const { return &m_allocatorCallbacks; }

	virtual void* alloc(size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept = 0;
	virtual void* realloc(void* ptr, size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept = 0;
	virtual void free(void* ptr) noexcept = 0;

private:
	VkAllocationCallbacks m_allocatorCallbacks;
};

// malloc を使った実装
class VulkanAllocator
	: public AbstractVulkanAllocator
{
public:
	VulkanAllocator();
	Result init();

	virtual void* alloc(size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept override;
	virtual void* realloc(void* ptr, size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept override;
	virtual void free(void* ptr) noexcept override;

private:
	int m_counter;
	size_t m_allocationSize[VK_SYSTEM_ALLOCATION_SCOPE_RANGE_SIZE];
};

// LinearAllocator を使った実装
class VulkanLinearAllocator
	: public AbstractVulkanAllocator
{
public:
	VulkanLinearAllocator();
	Result init();
	void setLinearAllocator(LinearAllocator* linearAllocator) { m_linearAllocator = linearAllocator; }

	virtual void* alloc(size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept override;
	virtual void* realloc(void* ptr, size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept override;
	virtual void free(void* ptr) noexcept override;

private:
	LinearAllocator* m_linearAllocator;
};

// CommandBuffer から直接参照されるオブジェクト。コマンド実行終了までは解放してはならないもの。
// 実装先で retain, release すること。
class IVulkanInFlightResource
{
public:
    virtual void onBind() = 0;
    virtual void onUnBind() = 0;
};

// 頂点バッファ、インデックスバッファ、レンダーターゲット転送のための一時バッファなど、様々な目的で使用する汎用バッファ。
class VulkanBuffer
	//: public RefObject
{
public:
    VulkanBuffer();
    Result init(VulkanDeviceContext* deviceContext, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	Result init(VulkanDeviceContext* deviceContext);
	Result resetBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
	Result resetMemoryBuffer(VkMemoryPropertyFlags properties, const VkAllocationCallbacks* allocator);
    void dispose();
    VulkanDeviceContext* deviceContext() const { return m_deviceContext; }
    VkDeviceSize size() const { return m_size; }
    VkBuffer vulkanBuffer() const { return m_buffer; }
    VkDeviceMemory vulkanBufferMemory() const { return m_bufferMemory; }
    void* map();
    void unmap();
    void setData(size_t offset, const void* data, VkDeviceSize size);

private:
    VulkanDeviceContext* m_deviceContext;
    VkBuffer m_buffer;
    VkDeviceMemory m_bufferMemory;
    VkDeviceSize m_size;
};

// テクスチャ、レンダーターゲット、デプスバッファなどに使用される Image
class VulkanImage
{
public:
	VulkanImage();
	Result init(VulkanDeviceContext* deviceContext, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);
	void dispose();
	VkImage vulkanImage() const { return m_image; }
	VkDeviceMemory vulkanDeviceMemory() const { return m_imageMemory; }
    VkImageView vulkanImageView() const { return m_imageView; }

private:
	VulkanDeviceContext* m_deviceContext;
	VkImage m_image;
	VkDeviceMemory m_imageMemory;
    VkImageView m_imageView;
};

// ひとつのコマンドバッファ。通常、記録中バッファと実行中バッファなどに分かれるため、インスタンスは複数作られる。
// VkCommandBuffer のほか、動的な各種バッファの変更などで必要となるメモリプールの管理も行う。
class VulkanCommandBuffer
	: public RefObject
{
public:
	VulkanCommandBuffer();
	Result init(VulkanDeviceContext* deviceContext);
	void dispose();

    VkCommandBuffer vulkanCommandBuffer() const { return m_commandBuffer; }

    Result beginRecording();
    Result endRecording();

    Result allocateDescriptorSets(VulkanShaderPass* shaderPass, std::array<VkDescriptorSet, DescriptorType_Count>* outSets);

	// データを destination へ送信するためのコマンドを push する。
	// 元データは戻り値のメモリ領域に書き込むこと。
	VulkanBuffer* cmdCopyBuffer(size_t size, VulkanBuffer* destination);

private:
    void cleanInFlightResources();
	//struct StagingBuffer
	//{
	//	VkBuffer buffer;
	//	VkDeviceMemory bufferMemory;
	//};

	void resetAllocator(size_t pageSize);
	Result glowStagingBufferPool();

	VulkanDeviceContext* m_deviceContext;
    VkCommandBuffer m_commandBuffer;

	Ref<LinearAllocatorPageManager> m_linearAllocatorManager;
	Ref<LinearAllocator> m_linearAllocator;
	VulkanLinearAllocator m_vulkanAllocator;

	size_t m_stagingBufferPoolUsed;
	std::vector<VulkanBuffer> m_stagingBufferPool;

    std::vector<Ref<VulkanShaderPass>> m_usingShaderPasses; // m_usingDescriptorSetsPools で持っている VulkanDescriptorSetsPool は VulkanShaderPass への強い参照を持たないので、これでカバーする
    std::vector<Ref<VulkanDescriptorSetsPool>> m_usingDescriptorSetsPools;
    
};

// DescriptorSet と、それにアタッチした UniformBuffer。
// vulkanDescriptorSets() を vkCmdBindDescriptorSets にセットする。
// できるだけ共有できるものは共有したいので、コマンドバッファに入れるとき、前回入れた UniformBuffer と差がなければ共有したい。
// ので、このインスタンスを前回値として CommandBuffer は持っておく。
// UniformBuffer は CommandBuffer の LeniarAllocator (cmdCopyBuffer()) からとる。
//class VulkanDescriptorSets
//{
//public:
//    VulkanDescriptorSets();
//    Result init();
//
//    const std::vector<VkDescriptorSet>& vulkanDescriptorSets() const { return m_vulkanDescriptorSets; }
//
//private:
//    std::vector<VkDescriptorSet> m_vulkanDescriptorSets;
//};

// Layout の原因である ShaderPass が削除されたら、その Layout をもとに作られているこの Pool も削除したい。
// そのため、このインスタンスの生成と削除は ShaderPass が担当する。
class VulkanDescriptorSetsPool
    : public RefObject
{
public:
    VulkanDescriptorSetsPool();
    Result init(VulkanDeviceContext* deviceContext, VulkanShaderPass* owner);
    void dispose();

    VulkanShaderPass* owner() const { return m_owner; }
    Result allocateDescriptorSets(VulkanCommandBuffer* commandBuffer, std::array<VkDescriptorSet, DescriptorType_Count>* sets);
    void reset();

private:
    VulkanDeviceContext* m_deviceContext;
    VulkanShaderPass* m_owner;
    VkDescriptorPool m_descriptorPool;
};

} // namespace detail
} // namespace ln
