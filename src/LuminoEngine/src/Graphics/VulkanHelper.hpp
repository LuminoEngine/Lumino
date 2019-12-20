
#pragma once

#if defined(LN_OS_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#define VK_NO_PROTOTYPES 1
#include <vulkan/vulkan.h>

//#if defined(LN_OS_WIN32)
//#include <vulkan/vulkan_win32.h>
//#endif

#include "GraphicsDeviceContext.hpp"
#include "MixHash.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>

// VK_core
extern PFN_vkCreateInstance vkCreateInstance;
extern PFN_vkDestroyInstance vkDestroyInstance;
extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
extern PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
extern PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
extern PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
extern PFN_vkCreateDevice vkCreateDevice;
extern PFN_vkDestroyDevice vkDestroyDevice;
extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
extern PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
extern PFN_vkGetDeviceQueue vkGetDeviceQueue;
extern PFN_vkQueueSubmit vkQueueSubmit;
extern PFN_vkQueueWaitIdle vkQueueWaitIdle;
extern PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
extern PFN_vkAllocateMemory vkAllocateMemory;
extern PFN_vkFreeMemory vkFreeMemory;
extern PFN_vkMapMemory vkMapMemory;
extern PFN_vkUnmapMemory vkUnmapMemory;
extern PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
extern PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
extern PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
extern PFN_vkBindBufferMemory vkBindBufferMemory;
extern PFN_vkBindImageMemory vkBindImageMemory;
extern PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
extern PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
extern PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
extern PFN_vkQueueBindSparse vkQueueBindSparse;
extern PFN_vkCreateFence vkCreateFence;
extern PFN_vkDestroyFence vkDestroyFence;
extern PFN_vkResetFences vkResetFences;
extern PFN_vkGetFenceStatus vkGetFenceStatus;
extern PFN_vkWaitForFences vkWaitForFences;
extern PFN_vkCreateSemaphore vkCreateSemaphore;
extern PFN_vkDestroySemaphore vkDestroySemaphore;
extern PFN_vkCreateEvent vkCreateEvent;
extern PFN_vkDestroyEvent vkDestroyEvent;
extern PFN_vkGetEventStatus vkGetEventStatus;
extern PFN_vkSetEvent vkSetEvent;
extern PFN_vkResetEvent vkResetEvent;
extern PFN_vkCreateQueryPool vkCreateQueryPool;
extern PFN_vkDestroyQueryPool vkDestroyQueryPool;
extern PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
extern PFN_vkCreateBuffer vkCreateBuffer;
extern PFN_vkDestroyBuffer vkDestroyBuffer;
extern PFN_vkCreateBufferView vkCreateBufferView;
extern PFN_vkDestroyBufferView vkDestroyBufferView;
extern PFN_vkCreateImage vkCreateImage;
extern PFN_vkDestroyImage vkDestroyImage;
extern PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
extern PFN_vkCreateImageView vkCreateImageView;
extern PFN_vkDestroyImageView vkDestroyImageView;
extern PFN_vkCreateShaderModule vkCreateShaderModule;
extern PFN_vkDestroyShaderModule vkDestroyShaderModule;
extern PFN_vkCreatePipelineCache vkCreatePipelineCache;
extern PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
extern PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
extern PFN_vkMergePipelineCaches vkMergePipelineCaches;
extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
extern PFN_vkCreateComputePipelines vkCreateComputePipelines;
extern PFN_vkDestroyPipeline vkDestroyPipeline;
extern PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
extern PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
extern PFN_vkCreateSampler vkCreateSampler;
extern PFN_vkDestroySampler vkDestroySampler;
extern PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
extern PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
extern PFN_vkResetDescriptorPool vkResetDescriptorPool;
extern PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
extern PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
extern PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
extern PFN_vkCreateFramebuffer vkCreateFramebuffer;
extern PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
extern PFN_vkCreateRenderPass vkCreateRenderPass;
extern PFN_vkDestroyRenderPass vkDestroyRenderPass;
extern PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
extern PFN_vkCreateCommandPool vkCreateCommandPool;
extern PFN_vkDestroyCommandPool vkDestroyCommandPool;
extern PFN_vkResetCommandPool vkResetCommandPool;
extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
extern PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
extern PFN_vkResetCommandBuffer vkResetCommandBuffer;
extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
extern PFN_vkCmdSetViewport vkCmdSetViewport;
extern PFN_vkCmdSetScissor vkCmdSetScissor;
extern PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
extern PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
extern PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
extern PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
extern PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
extern PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
extern PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
extern PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
extern PFN_vkCmdDraw vkCmdDraw;
extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
extern PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
extern PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
extern PFN_vkCmdDispatch vkCmdDispatch;
extern PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
extern PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
extern PFN_vkCmdCopyImage vkCmdCopyImage;
extern PFN_vkCmdBlitImage vkCmdBlitImage;
extern PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
extern PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
extern PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
extern PFN_vkCmdFillBuffer vkCmdFillBuffer;
extern PFN_vkCmdClearColorImage vkCmdClearColorImage;
extern PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
extern PFN_vkCmdClearAttachments vkCmdClearAttachments;
extern PFN_vkCmdResolveImage vkCmdResolveImage;
extern PFN_vkCmdSetEvent vkCmdSetEvent;
extern PFN_vkCmdResetEvent vkCmdResetEvent;
extern PFN_vkCmdWaitEvents vkCmdWaitEvents;
extern PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
extern PFN_vkCmdBeginQuery vkCmdBeginQuery;
extern PFN_vkCmdEndQuery vkCmdEndQuery;
extern PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
extern PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
extern PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
extern PFN_vkCmdPushConstants vkCmdPushConstants;
extern PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
extern PFN_vkCmdNextSubpass vkCmdNextSubpass;
extern PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
extern PFN_vkCmdExecuteCommands vkCmdExecuteCommands;

// VK_KHR_surface
extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;

// VK_KHR_swapchain
extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
extern PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
extern PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
extern PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
extern PFN_vkQueuePresentKHR vkQueuePresentKHR;

// VK_KHR_display
extern PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
extern PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
extern PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
extern PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
extern PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
extern PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
extern PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;

// VK_KHR_display_swapchain
extern PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;

#ifdef VK_USE_PLATFORM_XLIB_KHR
// VK_KHR_xlib_surface
extern PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
extern PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
// VK_KHR_xcb_surface
extern PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
extern PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
// VK_KHR_wayland_surface
extern PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
extern PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
// VK_KHR_mir_surface
extern PFN_vkCreateMirSurfaceKHR vkCreateMirSurfaceKHR;
extern PFN_vkGetPhysicalDeviceMirPresentationSupportKHR vkGetPhysicalDeviceMirPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
// VK_KHR_android_surface
extern PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
// VK_KHR_win32_surface
extern PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
extern PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif

namespace ln {
namespace detail {
class VulkanDevice;
class VulkanVertexDeclaration;
class VulkanShaderPass;
class VulkanDescriptorSetsPool;
class VulkanRenderPass;
class VulkanFramebuffer;

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
    static VkPrimitiveTopology LNPrimitiveTopologyToVkPrimitiveTopology(PrimitiveTopology value);
	static VkFormat LNVertexElementTypeToVkFormat(VertexElementType value);

    static bool initVulkanFunctions();

    static const char* getVkResultName(VkResult result);
    static bool hasStencilComponent(VkFormat format) { return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT; }
    static bool checkValidationLayerSupport();
    static int getPrimitiveVertexCount(PrimitiveTopology primitive, int primitiveCount);

    static Result createImageView(VulkanDevice* deviceContext, VkImage image, VkFormat format, uint32_t mipLevel, VkImageAspectFlags aspectFlags, VkImageView* outView);
    static bool resolveStd140Layout(const ShaderUniformInfo& info, size_t* aligndElemenSize);
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
{
public:
    VulkanBuffer();
    Result init(VulkanDevice* deviceContext, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const VkAllocationCallbacks* allocator);
    void dispose();
    VulkanDevice* deviceContext() const { return m_deviceContext; }
    VkBuffer nativeBuffer() const { return m_nativeBuffer; }
    VkDeviceMemory nativeBufferMemory() const { return m_nativeBufferMemory; }
    VkDeviceSize size() const { return m_size; }
    void* map();
    void unmap();
    void setData(size_t offset, const void* data, VkDeviceSize size);

private:
    VulkanDevice* m_deviceContext;
    VkBuffer m_nativeBuffer;
    VkDeviceMemory m_nativeBufferMemory;
    VkDeviceSize m_size;
	const VkAllocationCallbacks* m_allocator;	// CommandBuffer が使用する一時メモリは Device とは Allocator が異なる
};

// テクスチャ、レンダーターゲット、デプスバッファなどに使用される Image
class VulkanImage
{
public:
	VulkanImage();
	Result init(VulkanDevice* deviceContext, uint32_t width, uint32_t height, VkFormat format, uint32_t mipLevel, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);
    Result init(VulkanDevice* deviceContext/*, uint32_t width, uint32_t height*/, VkFormat format, VkImage image, VkImageView imageView);
    void dispose();
    VkFormat vulkanFormat() const { return m_format; }
	VkImage vulkanImage() const { return m_image; }
	VkDeviceMemory vulkanDeviceMemory() const { return m_imageMemory; }
    VkImageView vulkanImageView() const { return m_imageView; }
    bool IsExternalManagement() const { return m_externalManagement; }

    //void VulkanSwapchainRenderTargetTexture::reset(const VulkanSwapChain::SwapChainDesc& desc, std::vector<VkImage> images, std::vector<VkImageView> views);

private:
	VulkanDevice* m_deviceContext;
    //uint32_t m_width;
    //uint32_t m_height;
    VkFormat m_format;
	VkImage m_image;
	VkDeviceMemory m_imageMemory;
    VkImageView m_imageView;
    bool m_externalManagement;
};

// ひとつのコマンドバッファ。通常、記録中バッファと実行中バッファなどに分かれるため、インスタンスは複数作られる。
// VkCommandBuffer のほか、動的な各種バッファの変更などで必要となるメモリプールの管理も行う。
class VulkanCommandBuffer
	: public RefObject
{
public:
	VulkanCommandBuffer();
    ~VulkanCommandBuffer();
	Result init(VulkanDevice* deviceContext);
	void dispose();

    VkCommandBuffer vulkanCommandBuffer() const { return m_commandBuffer; }
    VkFence vulkanInFlightFence() const { return m_inFlightFence; }

    Result beginRecording();
    Result endRecording();
    void endRenderPassInRecordingIfNeeded();
    Result submit(VkSemaphore waitSemaphore, VkSemaphore signalSemaphore);

    Result allocateDescriptorSets(VulkanShaderPass* shaderPass, std::array<VkDescriptorSet, DescriptorType_Count>* outSets);

    VulkanBuffer* allocateBuffer(size_t size, VkBufferUsageFlags usage);

	// データを destination へ送信するためのコマンドを push する。
	// 元データは戻り値のメモリ領域に書き込むこと。
	VulkanBuffer* cmdCopyBuffer(size_t size, VulkanBuffer* destination);
    VulkanBuffer* cmdCopyBufferToImage(size_t size, const VkBufferImageCopy& region, VulkanImage* destination);

public:
    VulkanRenderPass* m_currentRenderPass = nullptr;
    VulkanFramebuffer* m_lastFoundFramebuffer = nullptr;
    //bool m_insideRendarPass = false;

private:
    void cleanInFlightResources();
	//struct StagingBuffer
	//{
	//	VkBuffer buffer;
	//	VkDeviceMemory bufferMemory;
	//};

	void resetAllocator(size_t pageSize);
	Result glowStagingBufferPool();

	VulkanDevice* m_deviceContext;
    VkCommandBuffer m_commandBuffer;
    VkFence m_inFlightFence;

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
    Result init(VulkanDevice* deviceContext, VulkanShaderPass* owner);
    void dispose();

    VulkanShaderPass* owner() const { return m_owner; }
    Result allocateDescriptorSets(VulkanCommandBuffer* commandBuffer, std::array<VkDescriptorSet, DescriptorType_Count>* sets);
    void reset();

private:
	static const uint32_t MAX_DESCRIPTOR_COUNT = 32;
    VulkanDevice* m_deviceContext;
    VulkanShaderPass* m_owner;
    //VkDescriptorPool m_descriptorPool;

	std::vector<VkDescriptorPool> m_pages;		// page instances
	std::deque<VkDescriptorPool> m_freePages;	// page references
	VkDescriptorPool m_activePage;
	uint32_t m_activePageUsedCount;
};

template<class T, class TSubClass>
class HashedObjectCache
{
public:
    void add(uint64_t key, T value)
    {
        m_hashMap.insert({ key, value });
    }

    bool find(uint64_t key, T* outObj) const
    {
        auto it = m_hashMap.find(key);
        if (it != m_hashMap.end()) {
            *outObj = it->second;
            return true;
        }
        return false;
    }

    void invalidate(uint64_t key)
    {
        auto it = m_hashMap.find(key);
        if (it != m_hashMap.end()) {
            static_cast<TSubClass*>(this)->onInvalidate(it->second);
            m_hashMap.erase(it);
        }
    }

    template<class TPred>
    void invalidateAllIf(TPred pred)
    {
        for (auto itr = m_hashMap.begin(); itr != m_hashMap.end();) {
            if (pred(itr->second)) {
                static_cast<TSubClass*>(this)->onInvalidate(itr->second);
                itr = m_hashMap.erase(itr);
            }
            else {
                ++itr;
            }
        }
    }

    void clear()
    {
        for (auto it = m_hashMap.begin(), itEnd = m_hashMap.end(); it != itEnd; ++it) {
            static_cast<TSubClass*>(this)->onInvalidate(it->second);
        }

        m_hashMap.clear();
    }

    uint32_t count() const
    {
        return m_hashMap.size();
    }

protected:
    std::unordered_map<uint64_t, T> m_hashMap;
};

// VkRenderPass
class VulkanRenderPass
    : public RefObject
{
public:
    VulkanRenderPass();
    Result init(VulkanDevice* deviceContext, const DeviceFramebufferState& state, bool loadOpClear);
    void dispose();

    VkRenderPass nativeRenderPass() const { return m_nativeRenderPass; }

private:
    VulkanDevice* m_deviceContext;
    VkRenderPass m_nativeRenderPass;
    bool m_loadOpClear;
};

// ColorBuffer と DepthBuffer の「種類」によって決まるので、FrameBuffer に比べてトータル数は少ない。
// 基本的にこのクラスは直接使わず、VulkanFramebuffer が持っている RenderPass を使うこと。
class VulkanRenderPassCache
    : public HashedObjectCache<Ref<VulkanRenderPass>, VulkanRenderPassCache>
{
public:
    struct FetchKey
    {
        const DeviceFramebufferState& state;
        bool loadOpClear = false;
    };

    VulkanRenderPassCache();
    Result init(VulkanDevice* deviceContext);
    void dispose();
    VulkanRenderPass* findOrCreate(const FetchKey& key);

    void onInvalidate(const Ref<VulkanRenderPass>& value) { value->dispose(); }
    static uint64_t computeHash(const FetchKey& key);

private:
    VulkanDevice* m_deviceContext;
};

// Framebuffer は RenderPass の子オブジェクトな位置づけとなる。
// VkFramebuffer と、そのレイアウト定義に相当する VkRenderPass をセットで持つ。
// ※ Vulkan のデータ構造上は Framebuffer と RenderPass は n:1.
//   フォーマットが共通なら VkRenderPass は共有される。
class VulkanFramebuffer
    : public RefObject
{
public:
    VulkanFramebuffer();
    Result init(VulkanDevice* deviceContext, VulkanRenderPass* ownerRenderPass, const DeviceFramebufferState& state/*, bool loadOpClear*/, uint64_t hash);
    void dispose();
    bool containsRenderTarget(ITexture* renderTarget) const;
    bool containsDepthBuffer(IDepthBuffer* depthBuffer) const;
    uint64_t hash() const { return m_hash; }
    VulkanRenderPass* ownerRenderPass() const { return m_ownerRenderPass; }
    VkFramebuffer vulkanFramebuffer() const { return m_framebuffer; }
    //SizeI extent() const { return m_renderTargets[0]->realSize(); }

private:
    VulkanDevice* m_deviceContext;
    VulkanRenderPass* m_ownerRenderPass;
    VkFramebuffer m_framebuffer;
    uint64_t m_hash;
    //size_t m_renderTargetCount;

    // 以下、こちらからは参照を持たない。インスタンスが dispose されたときに、このクラスに対して削除要求が飛んでくる。
    std::array<ITexture*, MaxMultiRenderTargets> m_renderTargets = {};
    IDepthBuffer* m_depthBuffer = nullptr;
};

//// 関連付けられている RenderTarget と DepthBuffer がひとつでも解放されたら
//// 登録してある VkRenderPass も削除する。
//// もっと厳密に参照カウントで管理することもできるけど大変なので、まずはこの方式で。
//class VulkanFramebufferCache
//    : public HashedObjectCache<Ref<VulkanFramebuffer>, VulkanFramebufferCache>
//{
//public:
//    struct FetchKey
//    {
//        const DeviceFramebufferState& state;
//        VulkanRenderPass* renderPass;
//    };
//
//    VulkanFramebufferCache();
//    Result init(VulkanDevice* deviceContext);
//    void dispose();
//    VulkanFramebuffer* findOrCreate(const FetchKey& key);
//
//    static uint64_t computeHash(const FetchKey& key)
//    {
//        MixHash hash;
//        for (size_t i = 0; i < key.state.renderTargets.size(); i++) {
//            hash.add(key.state.renderTargets[i]);
//        }
//        hash.add(key.state.depthBuffer);
//        hash.add(key.renderPass);
//        return hash.value();
//    }
//
//    void invalidateRenderTarget(ITexture* value)
//    {
//		HashedObjectCache<Ref<VulkanFramebuffer>, VulkanFramebufferCache>::invalidateAllIf(
//			[&](Ref<VulkanFramebuffer>& x) { return x->containsRenderTarget(value); });
//    }
//
//    void invalidateDepthBuffer(IDepthBuffer* value)
//    {
//		HashedObjectCache<Ref<VulkanFramebuffer>, VulkanFramebufferCache>::invalidateAllIf(
//			[&](Ref<VulkanFramebuffer>& x) { return x->containsDepthBuffer(value); });
//    }
//
//    void onInvalidate(const Ref<VulkanFramebuffer>& value)
//    {
//        value->dispose();
//    }
//
//private:
//    VulkanDevice* m_deviceContext;
//};

// Framebuffer は RenderPass の子オブジェクトな位置づけとなる。
// Dynamic としてマークしている state は次の通り。
// - VK_DYNAMIC_STATE_VIEWPORT,
// - VK_DYNAMIC_STATE_SCISSOR,
// - VK_DYNAMIC_STATE_BLEND_CONSTANTS,
// - VK_DYNAMIC_STATE_STENCIL_REFERENCE,
// これらは頻繁に変更される可能性があるためマークしている。
// なお、これらは computeHash に含める必要はない。
//
// 依存関係
//	- VertexLayout
//	- ShaderPass
//		- VkVertexInputAttributeDescription を作るために必要
class VulkanPipeline
    : public RefObject
{
public:
    VulkanPipeline();
    Result init(VulkanDevice* deviceContext, VulkanRenderPass* ownerRenderPass, const GraphicsContextState& state);
    void dispose();

    VkPipeline vulkanPipeline() const { return m_pipeline; }
    bool containsShaderPass(VulkanShaderPass* value) const { return m_relatedShaderPass == value; }
    //bool containsVertexDeclaration(VulkanVertexDeclaration* value) const { return m_relatedVertexDeclaration == value; }
    //bool containsFramebuffer(VulkanFramebuffer* value) const { return m_relatedFramebuffer == value; }


private:
    VulkanDevice* m_deviceContext;
    VulkanRenderPass* m_ownerRenderPass;
    VkPipeline m_pipeline;
    VulkanShaderPass* m_relatedShaderPass;                  // pipeline に関連づいている ShaderPass。これが削除されたらこの pipeline も削除する。
    //VulkanVertexDeclaration* m_relatedVertexDeclaration;    // pipeline に関連づいている VertexDeclaration。これが削除されたらこの pipeline も削除する。
    //VulkanFramebuffer* m_relatedFramebuffer;                // pipeline に関連づいている Framebuffer。これに含まれる RenderTarget か DepthBuffer が削除されたらこの pipeline も削除する。
};

class VulkanPipelineCache
    : public HashedObjectCache<Ref<VulkanPipeline>, VulkanPipelineCache>
{
public:
    struct FetchKey
    {
        const GraphicsContextState& state;
        VulkanRenderPass* renderPass;
    };

    VulkanPipelineCache();
    Result init(VulkanDevice* deviceContext);
    void dispose();
    // renderPass : この cache は vkCmdBeginRenderPass ～ vkCmdEndRenderPass の間で呼び出し、pipeline を得ることを目的としている。
    // この renderPass は、その間の RenderPass。あらかじめわかっている値を入れることで、Pipeline 作成の中でもう一度検索の必要がないようにする。
    VulkanPipeline* findOrCreate(const FetchKey& key);

    static uint64_t computeHash(const FetchKey& key);
    
    void invalidateFromShaderPass(VulkanShaderPass* value)
    {
        HashedObjectCache<Ref<VulkanPipeline>, VulkanPipelineCache>::invalidateAllIf([&](Ref<VulkanPipeline>& x) { return x->containsShaderPass(value); });
    }

    //void invalidateFromFrameBuffer(VulkanFramebuffer* value)
    //{
    //    HashedObjectCache<Ref<VulkanPipeline>, VulkanPipelineCache>::removeAllIf([&](Ref<VulkanPipeline>& x) { return x->containsFramebuffer(value); });
    //}

    void onInvalidate(const Ref<VulkanPipeline>& value)
    {
        value->dispose();
    }

private:
    VulkanDevice* m_deviceContext;
};

} // namespace detail
} // namespace ln
