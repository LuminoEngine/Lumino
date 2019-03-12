
#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "GraphicsDeviceContext.hpp"
#include "MixHash.hpp"
#include "../../LuminoCore/src/Base/LinearAllocator.hpp"

namespace ln {
namespace detail {
class VulkanDeviceContext;
class VulkanVertexDeclaration;
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

    static Result createImageView(VulkanDeviceContext* deviceContext, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView* outView);
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
    Result init(VulkanDeviceContext* deviceContext/*, uint32_t width, uint32_t height*/, VkFormat format, VkImage image, VkImageView imageView);
    void dispose();
    VkFormat vulkanFormat() const { return m_format; }
	VkImage vulkanImage() const { return m_image; }
	VkDeviceMemory vulkanDeviceMemory() const { return m_imageMemory; }
    VkImageView vulkanImageView() const { return m_imageView; }

    //void VulkanSwapchainRenderTargetTexture::reset(const VulkanSwapChain::SwapChainDesc& desc, std::vector<VkImage> images, std::vector<VkImageView> views);

private:
	VulkanDeviceContext* m_deviceContext;
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
	Result init(VulkanDeviceContext* deviceContext);
	void dispose();

    VkCommandBuffer vulkanCommandBuffer() const { return m_commandBuffer; }
    VkFence vulkanInFlightFence() const { return m_inFlightFence; }

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
            this->TSubClass::onInvalidate(it->second);
            m_hashMap.erase(it);
        }
    }

    template<class TPred>
    void removeAllIf(TPred pred)
    {
        for (auto itr = m_hashMap.begin(); itr != m_hashMap.end();) {
            if (pred(itr->second)) {
                itr = m_hashMap.erase(itr);
                // removeAllIf は dispose から呼ばれるので、ここでは参照を外すだけでよい
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

// ColorBuffer と DepthBuffer の「種類」によって決まるので、FrameBuffer に比べてトータル数は少ない。
// 基本的にこのクラスは直接使わず、VulkanFramebuffer が持っている RenderPass を使うこと。
class VulkanRenderPassCache
    : public HashedObjectCache<VkRenderPass, VulkanRenderPassCache>
{
public:
    VulkanRenderPassCache();
    void dispose();
    Result init(VulkanDeviceContext* deviceContext);
    VkRenderPass findOrCreate(const DeviceFramebufferState& key);

    void onInvalidate(VkRenderPass value);
    static uint64_t computeHash(const DeviceFramebufferState& framebuffer);

private:
    VulkanDeviceContext* m_deviceContext;
};

// VkFramebuffer と、そのレイアウト定義に相当する VkRenderPass をセットで持つ。
// ※VkRenderPass は VulkanRenderPassCache から取り出すので、m_framebuffer と m_renderPass は 1:1 ではない。
//   フォーマットが共通なら VkRenderPass は共有される。
class VulkanFramebuffer
    : public RefObject
{
public:
    VulkanFramebuffer();
    Result init(VulkanDeviceContext* deviceContext, const DeviceFramebufferState& state);
    void dispose();
    bool containsRenderTarget(ITexture* renderTarget) const;
    bool containsDepthBuffer(IDepthBuffer* depthBuffer) const;
    VkRenderPass vulkanRenderPass() const { return m_renderPass; }
    VkFramebuffer vulkanFramebuffer() const { return m_framebuffer; }
    //SizeI extent() const { return m_renderTargets[0]->realSize(); }

private:
    VulkanDeviceContext* m_deviceContext;
    VkRenderPass m_renderPass;
    VkFramebuffer m_framebuffer;
    //size_t m_renderTargetCount;

    // 以下、こちらからは参照を持たない。インスタンスが dispose されたときに、このクラスに対して削除要求が飛んでくる。
    std::array<ITexture*, MaxMultiRenderTargets> m_renderTargets = {};
    IDepthBuffer* m_depthBuffer = nullptr;
};

// 関連付けられている RenderTarget と DepthBuffer がひとつでも解放されたら
// 登録してある VkRenderPass も削除する。
// もっと厳密に参照カウントで管理することもできるけど大変なので、まずはこの方式で。
class VulkanFramebufferCache
    : public HashedObjectCache<Ref<VulkanFramebuffer>, VulkanFramebufferCache>
{
public:
    VulkanFramebufferCache();
    Result init(VulkanDeviceContext* deviceContext);
    void dispose();
    VulkanFramebuffer* findOrCreate(const DeviceFramebufferState& key);

    static uint64_t computeHash(const DeviceFramebufferState& state)
    {
        MixHash hash;
        for (size_t i = 0; i < state.renderTargets.size(); i++) {
            hash.add(state.renderTargets[i]);
        }
        hash.add(state.depthBuffer);
        return hash.value();
    }

    void invalidateRenderTarget(ITexture* renderTarget)
    {
        for (auto itr = m_hashMap.begin(); itr != m_hashMap.end();) {
            if (itr->second->containsRenderTarget(renderTarget)) {
                itr = m_hashMap.erase(itr);
                // invalidateXXXX は dispose から呼ばれるので、ここでは参照を外すだけでよい
            }
            else {
                ++itr;
            }
        }
    }

    void invalidateDepthBuffer(IDepthBuffer* depthBuffer)
    {
        for (auto itr = m_hashMap.begin(); itr != m_hashMap.end();) {
            if (itr->second->containsDepthBuffer(depthBuffer)) {
                itr = m_hashMap.erase(itr);
                // invalidateXXXX は dispose から呼ばれるので、ここでは参照を外すだけでよい
            }
            else {
                ++itr;
            }
        }
    }

    void onInvalidate(const Ref<VulkanFramebuffer>& value)
    {
        value->dispose();
    }

private:
    VulkanDeviceContext* m_deviceContext;
};

// Dynamic としてマークしている state は次の通り。
// - VK_DYNAMIC_STATE_VIEWPORT,
// - VK_DYNAMIC_STATE_SCISSOR,
// - VK_DYNAMIC_STATE_BLEND_CONSTANTS,
// - VK_DYNAMIC_STATE_STENCIL_REFERENCE,
// これらは頻繁に変更される可能性があるためマークしている。
// なお、これらは computeHash に含める必要はない。
class VulkanPipeline
    : public RefObject
{
public:
    VulkanPipeline();
    Result init(VulkanDeviceContext* deviceContext, const IGraphicsDeviceContext::State& state, VkRenderPass renderPass);
    void dispose();

    VkPipeline vulkanPipeline() const { return m_pipeline; }
    bool containsShaderPass(VulkanShaderPass* value) const { return m_relatedShaderPass == value; }
    //bool containsVertexDeclaration(VulkanVertexDeclaration* value) const { return m_relatedVertexDeclaration == value; }
    //bool containsFramebuffer(VulkanFramebuffer* value) const { return m_relatedFramebuffer == value; }

    static uint64_t computeHash(const IGraphicsDeviceContext::State& state);

private:
    VulkanDeviceContext* m_deviceContext;
    VkPipeline m_pipeline;
    VulkanShaderPass* m_relatedShaderPass;                  // pipeline に関連づいている ShaderPass。これが削除されたらこの pipeline も削除する。
    //VulkanVertexDeclaration* m_relatedVertexDeclaration;    // pipeline に関連づいている VertexDeclaration。これが削除されたらこの pipeline も削除する。
    //VulkanFramebuffer* m_relatedFramebuffer;                // pipeline に関連づいている Framebuffer。これに含まれる RenderTarget か DepthBuffer が削除されたらこの pipeline も削除する。
};

class VulkanPipelineCache
    : public HashedObjectCache<Ref<VulkanPipeline>, VulkanPipelineCache>
{
public:
    VulkanPipelineCache();
    Result init(VulkanDeviceContext* deviceContext);
    void dispose();
    // renderPass : この cache は vkCmdBeginRenderPass ～ vkCmdEndRenderPass の間で呼び出し、pipeline を得ることを目的としている。
    // この renderPass は、その間の RenderPass。あらかじめわかっている値を入れることで、Pipeline 作成の中でもう一度検索の必要がないようにする。
    VulkanPipeline* findOrCreate(const IGraphicsDeviceContext::State& key, VkRenderPass renderPass);

    void invalidateFromShaderPass(VulkanShaderPass* value)
    {
        HashedObjectCache<Ref<VulkanPipeline>, VulkanPipelineCache>::removeAllIf([&](Ref<VulkanPipeline>& x) { return x->containsShaderPass(value); });
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
    VulkanDeviceContext* m_deviceContext;
};

} // namespace detail
} // namespace ln
