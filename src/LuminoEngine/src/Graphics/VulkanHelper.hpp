
#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "GraphicsDeviceContext.hpp"
#include "MixHash.hpp"

namespace ln {
namespace detail {
class VulkanDeviceContext;

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
	static VkFormat LNVertexElementTypeToVkFormat(VertexElementType value);
    static const char* getVkResultName(VkResult result);
    static bool checkValidationLayerSupport();
};

// VkAllocationCallbacks の本体。
// インスタンスを VkAllocationCallbacks::pUserData に登録し、static コールバックからメンバをコールする。
class VulkanAllocator
{
public:
	VulkanAllocator();
	Result init();
	const VkAllocationCallbacks* vulkanAllocator() const { return &m_allocatorCallbacks; }

	void* alloc(size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept;
	void* realloc(void* ptr, size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept;
	void free(void* ptr) noexcept;

private:
	VkAllocationCallbacks m_allocatorCallbacks;
	int m_counter;
	size_t m_allocationSize[VK_SYSTEM_ALLOCATION_SCOPE_RANGE_SIZE];
};

// 頂点バッファ、インデックスバッファ、レンダーターゲット転送のための一時バッファなど、様々な目的で使用する汎用バッファ。
class VulkanBuffer
    : public RefObject
{
public:
    VulkanBuffer();
    Result init(VulkanDeviceContext* deviceContext, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    void dispose();
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

} // namespace detail
} // namespace ln
