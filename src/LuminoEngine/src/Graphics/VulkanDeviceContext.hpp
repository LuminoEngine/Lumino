
#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "GraphicsDeviceContext.hpp"
#include "MixHash.hpp"

namespace ln {
namespace detail {
class VulkanDeviceContext;
class VulkanQueue;
class VulkanCommandList;
class VulkanPipeline;
class VulkanSwapChain;
class VulkanIndexBuffer;
class VulkanTexture2D;
class VulkanRenderTargetTexture;
class VulkanSwapchainRenderTargetTexture;
class VulkanShaderPass;
class VulkanShaderUniformBuffer;
class VulkanShaderUniform;
class VulkanLocalShaderSamplerBuffer;

class VulkanAllocator
{
public:
    VulkanAllocator();
    void* alloc(size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept;
    void* realloc(void* ptr, size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept;
    void free(void* ptr) noexcept;

private:
    int m_counter;
    size_t m_allocationSize[VK_SYSTEM_ALLOCATION_SCOPE_RANGE_SIZE];
};

template<class T>
class HashedObjectCache
{
public:
	template<class TDeletor>
    bool init(TDeletor deletor)
    {
		m_deletor = deletor;
        return true;
    }

    void add(uint64_t key, T value)
    {
        invalidate(key);
        m_hashMap.insert({key, value});
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
            m_deletor(it->second);
            m_hashMap.erase(it);
        }
    }

    void clear()
    {
        for (auto it = m_hashMap.begin(), itEnd = m_hashMap.end(); it != itEnd; ++it) {
            m_deletor(it->second);
        }

        m_hashMap.clear();
    }

    uint32_t count() const
    {
        return m_hashMap.size();
    }

protected:
    std::unordered_map<uint64_t, T> m_hashMap;
    std::function<void(T)> m_deletor;
};

class VulkanPipelineCache
	: public HashedObjectCache<Ref<VulkanPipeline>>
{
public:

	// Pipeline state の Hash を計算する。
	// viewportRect, scissorRect, VertexBuffer, IndexBuffer は含まない。
	// RenderTarget と DepthBuffer についてはフォーマットを含む。オブジェクトは含まない。
	static uint64_t computeHash(const DevicePipelineState& state, const DeviceFramebufferState& framebuffer);

	// TODO: 長い間使われない Item を消すような処理
};


class VulkanRenderPassCache
	: public HashedObjectCache<VkRenderPass>
{
public:
	// VkRenderPass は、FrameBuffer のフォーマットというかスキーマのようなものを管理する。
	// ほかのオブジェクトを参照することはない。
	// ひとまず、アプリ動作中に VkRenderPass を Destroy はせず、終了時にすべて Destory する。

	static uint64_t computeHash(const DeviceFramebufferState& state);
};

class VulkanFrameBuffer
	: public RefObject
{
public:
	bool init(VulkanDeviceContext* deviceContext, const DeviceFramebufferState& state);
	void dispose();
	bool containsRenderTarget(ITexture* renderTarget) const;
	bool containsDepthBuffer(IDepthBuffer* depthBuffer) const;
	VkRenderPass vulkanRenderPass() const { return m_renderPass; }
	VkFramebuffer vulkanFramebuffer() const { return m_framebuffer; }
	SizeI extent() const { return m_renderTargets[0]->realSize(); }

public: // TODO:
	VulkanDeviceContext* m_deviceContext;
	VkRenderPass m_renderPass;
	VkFramebuffer m_framebuffer;
	std::array<ITexture*, MaxMultiRenderTargets> m_renderTargets = {};
	size_t m_renderTargetCount;
	IDepthBuffer* m_depthBuffer = nullptr;
};

class VulkanFrameBufferCache
	: public HashedObjectCache<Ref<VulkanFrameBuffer>>
{
public:
	//static uint64_t computeHash(ITexture* const* renderTargets, size_t renderTargetCount, IDepthBuffer* depthBuffer)
	//{
	//	return VulkanRenderPassCache::computeHash(renderTargets, renderTargetCount, depthBuffer);
	//}

	void invalidateRenderTarget(ITexture* renderTarget)
	{
		for (auto itr = m_hashMap.begin(); itr != m_hashMap.end(); ++itr) {
			if (itr->second->containsRenderTarget(renderTarget)) {
				m_deletor(itr->second);
				itr = m_hashMap.erase(itr);
			}
		}
	}

	void invalidateDepthBuffer(IDepthBuffer* depthBuffer)
	{
		for (auto itr = m_hashMap.begin(); itr != m_hashMap.end(); ++itr) {
			if (itr->second->containsDepthBuffer(depthBuffer)) {
				m_deletor(itr->second);
				itr = m_hashMap.erase(itr);
			}
		}
	}

	// 関連付けられている RenderTarget と DepthBuffer がひとつでも解放されたら
	// 登録してある VkRenderPass も削除する。
	// もっと厳密に参照カウントで管理することもできるけど大変なので、まずはこの方式で。
};

class VulkanDeviceContext
    : public IGraphicsDeviceContext
{
public:
    struct Settings
    {
        uint32_t maxGraphicsQueueSubmitCount = 256;
        uint32_t maxComputeQueueSubmitCount = 256;
        uint32_t maxTransferQueueSubmitCount = 256;
        bool debugEnabled = false;
    };

    VulkanDeviceContext();
    virtual ~VulkanDeviceContext();
    bool init(const Settings& settings);
    virtual void dispose() override;

    const VkAllocationCallbacks* vulkanAllocator() const { return &m_allocatorCallbacks; }
    VkInstance vulkanInstance() const { return m_instance; }
    VkDevice vulkanDevice() const { return m_device; }
    VkPhysicalDevice vulkanPhysicalDevice() const { return m_physicalDevice; }
    const Ref<VulkanQueue>& graphicsQueue() const { return m_graphicsQueue; }
    const Ref<VulkanQueue>& computeQueue() const { return m_computeQueue; }
    const Ref<VulkanQueue>& transferQueue() const { return m_transferQueue; }
	VulkanRenderPassCache& renderPassCache() { return m_renderPassCache; }
	VulkanPipelineCache& pipelineCache() { return m_pipelineCache; }
	VulkanFrameBufferCache& frameBufferCache() { return m_frameBufferCache; }
    const Ref<VulkanCommandList>& activeCommandBuffer() const { return m_activeCommandBuffer; }
	void setActiveCommandBuffer(const Ref<VulkanCommandList>& commandBuffer) { m_activeCommandBuffer = commandBuffer; }
	VulkanShaderPass* defaultShaderPass() const { return m_defaultShaderPass; }
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	bool getVkRenderPass(const DeviceFramebufferState& state, VkRenderPass* outPass);
    bool beginActiveCommandBuffer();
    bool endActiveCommandBuffer();

	static const char* getVkResultName(VkResult result);

protected:
    virtual void onGetCaps(GraphicsDeviceCaps* outCaps) override;
    virtual void onEnterMainThread() override;
    virtual void onLeaveMainThread() override;
    virtual void onSaveExternalRenderState() override;
    virtual void onRestoreExternalRenderState() override;
    virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
    virtual Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
    virtual Ref<IVertexBuffer> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
    virtual Ref<IIndexBuffer> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
    virtual Ref<ITexture> onCreateTexture2D(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    virtual Ref<ITexture> onCreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    virtual Ref<ITexture> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap) override;
    virtual Ref<IDepthBuffer> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
    virtual Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) override;
    virtual Ref<IShaderPass> onCreateShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, const ShaderVertexInputAttributeTable* attributeTable, const UnifiedShaderRefrectionInfo* refrection, ShaderCompilationDiag* diag) override;
    virtual void onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState) override;
    virtual void onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer) override;
    virtual void onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize) override;
    virtual void onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer) override;
    virtual void onUpdateShaderPass(IShaderPass* newPass) override;
    virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
    virtual void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount) override;
    virtual void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount) override;
    virtual void onPresent(ISwapChain* swapChain) override;

private:
    struct PhysicalDeviceInfo
    {
        VkPhysicalDevice device;
        VkPhysicalDeviceMemoryProperties memoryProperty;
        VkPhysicalDeviceProperties deviceProperty;
    };
    struct DeviceCaps
    {
        uint32_t ConstantBufferMemoryAlignment;
        uint32_t MaxTargetWidth;
        uint32_t MaxTargetHeight;
        uint32_t MaxTargetArraySize;
        uint32_t MaxColorSampleCount;
        uint32_t MaxDepthSampleCount;
        uint32_t MaxStencilSampleCount;
    };

	bool submitStatus();
    static void CheckInstanceExtension(const char* layer, size_t requestCount, const char** requestNames, std::vector<std::string>* result);
    static void GetDeviceExtension(const char* layer, VkPhysicalDevice physicalDevice, std::vector<std::string>* result);
    template<typename T>
    inline T GetVkInstanceProc(const char* name)
    {
        return reinterpret_cast<T>(vkGetInstanceProcAddr(m_instance, name));
    }
    template<typename T>
    inline T GetVkDeviceProc(const char* name)
    {
        return reinterpret_cast<T>(vkGetDeviceProcAddr(m_device, name));
    }

    VkInstance m_instance;
    VkAllocationCallbacks m_allocatorCallbacks;
    VulkanAllocator m_allocator;
    uint32_t m_physicalDeviceCount;
    std::vector<PhysicalDeviceInfo> m_physicalDeviceInfos;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE; 
    VkDevice m_device;
    Ref<VulkanQueue> m_graphicsQueue; // graphics and presentation queue
    Ref<VulkanQueue> m_computeQueue;
    Ref<VulkanQueue> m_transferQueue;
    DeviceCaps m_caps;
    uint64_t m_timeStampFrequency;
	VulkanRenderPassCache m_renderPassCache;
	VulkanPipelineCache m_pipelineCache;
	VulkanFrameBufferCache m_frameBufferCache;
    Ref<VulkanCommandList> m_activeCommandBuffer;   // present でほかの SwapChain が持っている CommandBuffer と swap
	Ref<VulkanShaderPass> m_defaultShaderPass;		// vulkan は shader の省略はできないのでデフォルトを用意する

	bool m_requiredChangePipeline;
	bool m_requiredChangeRenderPass;

    bool m_ext_EXT_KHR_PUSH_DESCRIPTOR;
    bool m_ext_EXT_KHR_DESCRIPTOR_UPDATE_TEMPLATE;
    bool m_ext_EXT_NVX_DEVICE_GENERATE_COMMAND;
    bool m_ext_EXT_AMD_DRAW_INDIRECT_COUNT;
    bool m_ext_EXT_DEBUG_MARKER;
    bool m_ext_EXT_HDR_METADATA;
};

class VulkanQueue
    : public RefObject
{
public:
    VulkanQueue();
    bool init(VulkanDeviceContext* deviceContext, uint32_t familyIndex, uint32_t queueIndex, uint32_t maxSubmitCount);
    void dispose();

    uint32_t familyIndex() const { return m_familyIndex; }
    VkQueue vulkanQueue() const { return m_queue; }

    bool submit(VulkanCommandList* commandBuffer, VkSemaphore waitSemaphore, VkSemaphore signalSemaphore);

private:
    VulkanDeviceContext* m_deviceContext;
    uint32_t m_familyIndex;
    uint32_t m_maxSubmitCount;
    VkQueue m_queue;
    std::vector<VkCommandBuffer> m_submitList;
};

class VulkanCommandList
    : public RefObject
{
public:
    enum class Type
    {
        COMMANDLIST_TYPE_DIRECT = 0,
        COMMANDLIST_TYPE_BUNDLE = 1,
        COMMANDLIST_TYPE_COPY = 2,
        COMMANDLIST_TYPE_COMPUTE = 3,
    };

    VulkanCommandList();
    virtual ~VulkanCommandList();
    bool init(VulkanDeviceContext* deviceContext, Type type);
    void dispose();
    VkCommandBuffer vulkanCommandBuffer() const { return m_commandBuffer; }
    VkCommandPool vulkanCommandPool() const { return m_commandPool; }
	VkFence vulkanInFlightFence() const { return m_inFlightFence; }

    bool begin();
	bool end();
	bool flush();

	void addFrameBufferCmd(VulkanFrameBuffer* frameBuffer);
	void addPipelineCmd(VulkanPipeline* pipeline);

private:
    VulkanDeviceContext* m_deviceContext;
    VkCommandPool m_commandPool;
    VkCommandBuffer m_commandBuffer;
	VkFence m_inFlightFence;	// コマンド実行完了を通知するための Fence
	VulkanFrameBuffer* m_currentFrameBuffer;
};

class VulkanPipeline
	: public RefObject
{
public:
	VulkanPipeline();
	virtual ~VulkanPipeline();
	bool init(VulkanDeviceContext* deviceContext, const IGraphicsDeviceContext::State& committed);
	void dispose();

    VkPipeline vulkanPipeline() const { return m_pipeline; }

private:
	VulkanDeviceContext* m_deviceContext;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;	// 管理は Cache に任せるので、このクラスの中で Destroy しない
};

// 頂点バッファ、インデックスバッファ、レンダーターゲット転送のための一時バッファなど、様々な目的で使用する汎用バッファ。
class VulkanBuffer
    : public RefObject
{
public:
    Result init(VulkanDeviceContext* deviceContext, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    void dispose();
	size_t size() const { return m_size; }
    VkBuffer vulkanBuffer() const { return m_buffer; }
    VkDeviceMemory vulkanBufferMemory() const { return m_bufferMemory; }

private:
    VulkanDeviceContext* m_deviceContext;
    VkBuffer m_buffer;
    VkDeviceMemory m_bufferMemory;
	size_t m_size;
};

class VulkanSwapChain
    : public ISwapChain
{
public:
    struct SwapChainDesc
    {
        uint32_t Width;
        uint32_t Height;
        TextureFormat Format;
        VkFormat vulkanFormat;
        uint32_t MipLevels;
        uint32_t SampleCount;
        uint32_t BufferCount;
        uint32_t SyncInterval;
        bool EnableFullScreen;
    };

    VulkanSwapChain();
    virtual ~VulkanSwapChain();
    bool init(VulkanDeviceContext* deviceContext, PlatformWindow* window, const SwapChainDesc& desc);
    virtual void dispose() override;
    virtual ITexture* getColorBuffer() const override;

    bool present();

private:
    bool acquireNextImage();

    VulkanDeviceContext* m_deviceContext;
    SwapChainDesc m_desc;
    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapChain;
    //VulkanQueue* m_graphicsQueue;
    std::vector<VkSurfaceFormatKHR> m_surfaceFormats;
    VkFormat m_imageFormat;
    VkColorSpaceKHR m_colorSpace;
    VkSurfaceTransformFlagBitsKHR m_preTransform;
    VkPresentModeKHR m_presentMode;
    //std::vector<VkImage> m_images;
    //std::vector<VkImageView> m_imageViews;
    Ref<VulkanSwapchainRenderTargetTexture> m_colorBuffer;
    //std::vector<Ref<VulkanRenderTargetTexture>> m_buffers;
    uint32_t m_currentBufferIndex;
    Ref<VulkanCommandList> m_inactiveCommandBuffer;

	static const int MaxPresentationFrameIndex = 2;
	int m_currentPresentationFrameIndex;
	VkSemaphore m_renderFinishedSemaphores[MaxPresentationFrameIndex];
	VkSemaphore m_imageAvailableSemaphores[MaxPresentationFrameIndex];
	VkFence m_imageAvailableFences[MaxPresentationFrameIndex];


};

class VulkanVertexDeclaration
    : public IVertexDeclaration
{
public:
    VulkanVertexDeclaration();
    virtual ~VulkanVertexDeclaration();
    bool init(const VertexElement* elements, int elementsCount);
    virtual void dispose() override;
	const std::vector<VertexElement>& elements() const { return m_elements; }
	const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions() const { return m_bindings; }
	const std::vector<VkVertexInputAttributeDescription>& vertexAttributeTemplate() const { return m_attributeTemplate; }

private:
	std::vector<VertexElement> m_elements;
	uint32_t m_maxStreamCount;
    std::vector<VkVertexInputBindingDescription> m_bindings;
    std::vector<VkVertexInputAttributeDescription> m_attributeTemplate;	// location 未設定。Pipleline 作成時にこれをコピーして各定する。
};

class VulkanVertexBuffer
    : public IVertexBuffer
{
public:
    VulkanVertexBuffer();
    virtual ~VulkanVertexBuffer();
	Result init(VulkanDeviceContext* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
    virtual void dispose() override;
	virtual size_t getBytesSize() override { return m_buffer.size(); }
	virtual GraphicsResourceUsage usage() const override { return m_usage; }
    virtual void setSubData(size_t offset, const void* data, size_t length) override;
    virtual void* map() override;
    virtual void unmap() override;

    VkBuffer vulkanBuffer() const { return m_buffer.vulkanBuffer(); }
	VkDeviceMemory vulkanDeviceMemory() const { return m_buffer.vulkanBufferMemory(); }

private:
	VulkanBuffer m_buffer;
	GraphicsResourceUsage m_usage;
};

class VulkanIndexBuffer
    : public IIndexBuffer
{
public:
    VulkanIndexBuffer();
    virtual ~VulkanIndexBuffer();
    Result init(VulkanDeviceContext* deviceContext, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
    virtual void dispose() override;
	virtual size_t getBytesSize() override { return m_buffer.size(); }
	virtual GraphicsResourceUsage usage() const override { return m_usage; }
    virtual void setSubData(size_t offset, const void* data, size_t length) override;
    virtual void* map() override;
    virtual void unmap() override;

	VkBuffer vulkanBuffer() const { return m_buffer.vulkanBuffer(); }
	VkDeviceMemory vulkanDeviceMemory() const { return m_buffer.vulkanBufferMemory(); }

private:
	VulkanBuffer m_buffer;
	GraphicsResourceUsage m_usage;
};

class VulkanTextureBase
    : public ITexture
{
public:
	struct TextureDesc
	{
		//    RESOURCE_DIMENSION      Dimension;
		uint32_t                Width;
		uint32_t                Height;
		uint32_t DepthOrArraySize;
		//    RESOURCE_FORMAT         Format;
		uint32_t MipLevels;
		//    uint32_t                SampleCount;
		//    RESOURCE_LAYOUT         Layout;
		//    uint32_t                Usage;
		//    RESOURCE_STATE          InitState;
		//    HeapProperty            HeapProperty;
	};

    VulkanTextureBase();
    virtual ~VulkanTextureBase();

	virtual const TextureDesc& desc() const = 0;
	virtual VkImage vulkanImage() const = 0;
	virtual VkImageView vulkanImageView() const = 0;
};

class VulkanTexture2D
    : public VulkanTextureBase
{
public:

    VulkanTexture2D();
    virtual ~VulkanTexture2D();
    bool init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData);
    virtual void dispose() override;

	virtual const TextureDesc& desc() const override { return m_desc; }
	virtual VkImage vulkanImage() const override { return m_image; }
	virtual VkImageView vulkanImageView() const override { return m_imageView; }

    virtual DeviceTextureType type() const override;
    virtual void readData(void* outData) override;
    virtual SizeI realSize() override;
    virtual TextureFormat getTextureFormat() const override;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

private:
    VulkanDeviceContext* m_deviceContext;
	TextureDesc m_desc;
	VkImage m_image;
	VkImageView m_imageView;
};

class VulkanTexture3D
    : public VulkanTextureBase
{
public:
    VulkanTexture3D();
    virtual ~VulkanTexture3D();
    void init(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData);
    virtual void dispose() override;

    virtual DeviceTextureType type() const override;
    virtual void readData(void* outData) override;
    virtual SizeI realSize() override;
    virtual TextureFormat getTextureFormat() const override;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

private:
};

class VulkanRenderTargetTexture
    : public VulkanTextureBase
{
public:
    VulkanRenderTargetTexture();
    virtual ~VulkanRenderTargetTexture();
    void init(VulkanDeviceContext* context, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap);
    virtual void dispose() override;

	virtual const TextureDesc& desc() const override { return m_desc; }
	virtual VkImage vulkanImage() const override { return m_image; }
	virtual VkImageView vulkanImageView() const override { return m_imageView; }

    virtual DeviceTextureType type() const override;
    virtual void readData(void* outData) override;
    virtual SizeI realSize() override;
    virtual TextureFormat getTextureFormat() const override;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

    //void resestSwapchainFrame(VkImage image, VkImageView view);

    //static void transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

private:
	VulkanDeviceContext* m_deviceContext;
	TextureDesc m_desc;
	VkImage m_image;
	VkImageView m_imageView;
	VkImageAspectFlags m_imageAspectFlags;
	VkDeviceMemory m_deviceMemory;

    TextureFormat m_format;
    VkFormat m_vulkanFormat;
	bool m_isExternal;
};

// ダブルバッファを管理するため、通常の RenderTargetTexture とは別にする。
// IGraphicsDeviceContext は DX11 相当の IF に合わせているので、ダブルバッファの実装はこちら側で隠蔽したい。
class VulkanSwapchainRenderTargetTexture
    : public VulkanTextureBase
{
public:
    VulkanSwapchainRenderTargetTexture();
    virtual ~VulkanSwapchainRenderTargetTexture();
    bool init(VulkanDeviceContext* deviceContext);
    virtual void dispose() override;

    void reset(const VulkanSwapChain::SwapChainDesc& desc, std::vector<VkImage> images, std::vector<VkImageView> views);
    void setBufferIndex(int index) { m_bufferIndex = index; }
    VkImage vulkanImage(int index) const { return m_images[index]; }
    VkImageView vulkanImageView(int index) const { return m_imageViews[index]; }

    virtual const TextureDesc& desc() const override { return m_desc; }
    virtual VkImage vulkanImage() const override { return m_images[m_bufferIndex]; }
    virtual VkImageView vulkanImageView() const override { return m_imageViews[m_bufferIndex]; }

    virtual DeviceTextureType type() const override;
    virtual void readData(void* outData) override;
    virtual SizeI realSize() override;
    virtual TextureFormat getTextureFormat() const override;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

    int prevBufferIndex() const { return (m_bufferIndex == 0) ? m_images.size() - 1 : m_bufferIndex - 1; }

private:
    VulkanDeviceContext* m_deviceContext;
    TextureDesc m_desc;
    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imageViews;
    int m_bufferIndex;
    //VkImageAspectFlags m_imageAspectFlags;
    //VkDeviceMemory m_deviceMemory;

    TextureFormat m_format;
    VkFormat m_vulkanFormat;
};

class VulkanDepthBuffer
    : public IDepthBuffer
{
public:
    VulkanDepthBuffer();
    virtual ~VulkanDepthBuffer();
    void init(VulkanDeviceContext* context, uint32_t width, uint32_t height);
    virtual void dispose() override;

private:
	VulkanDeviceContext* m_deviceContext;
};

class VulkanSamplerState
    : public ISamplerState
{
public:
    VulkanSamplerState();
    virtual ~VulkanSamplerState();
    void init(const SamplerStateData& desc);
    virtual void dispose() override;

private:
};

class VulkanShaderPass
    : public IShaderPass
{
public:
    VulkanShaderPass();
    virtual ~VulkanShaderPass();
    bool init(VulkanDeviceContext* context, const void* spvVert, size_t spvVertLen, const void* spvFrag, size_t spvFragLen, const ShaderVertexInputAttributeTable* attributeTable, const UnifiedShaderRefrectionInfo* refrection);
    virtual void dispose() override;

	VkShaderModule vertShaderModule() const { return m_vertShaderModule; }
	VkShaderModule fragShaderModule() const { return m_fragShaderModule; }
	const ShaderVertexInputAttributeTable& inputAttributeTable() const { return m_inputAttributeTable; }
	bool findAttributeLocation(VertexElementUsage usage, uint32_t usageIndex, uint32_t* outLocation) const;

    virtual int getUniformCount() const override;
    virtual IShaderUniform* getUniform(int index) const override;
    virtual void setUniformValue(int index, const void* data, size_t size) override;
    virtual int getUniformBufferCount() const override;
    virtual IShaderUniformBuffer* getUniformBuffer(int index) const override;
    virtual IShaderSamplerBuffer* samplerBuffer() const override;

private:
	VulkanDeviceContext* m_deviceContext;
	VkShaderModule m_vertShaderModule;
	VkShaderModule m_fragShaderModule;
	ShaderVertexInputAttributeTable m_inputAttributeTable;
};

class VulkanShaderUniformBuffer
    : public IShaderUniformBuffer
{
public:
    VulkanShaderUniformBuffer();
    virtual ~VulkanShaderUniformBuffer();
    void init();
    virtual void dispose() override;

    virtual const std::string& name() const override;
    virtual int getUniformCount() const override;
    virtual IShaderUniform* getUniform(int index) const override;
    virtual size_t bufferSize() const override;
    virtual void setData(const void* data, size_t size) override;

private:
    std::string m_name;
};

class VulkanShaderUniform
    : public IShaderUniform
{
public:
    VulkanShaderUniform();
    virtual ~VulkanShaderUniform();
    void init();
    virtual void dispose() override;
    virtual const ShaderUniformTypeDesc& desc() const override { return m_desc; }
    virtual const std::string& name() const override { return m_name; }

private:
    ShaderUniformTypeDesc m_desc;
    std::string m_name;
};

class VulkanLocalShaderSamplerBuffer
    : public IShaderSamplerBuffer
{
public:
    VulkanLocalShaderSamplerBuffer();
    virtual ~VulkanLocalShaderSamplerBuffer();
    void init();
    virtual void dispose() override;

    virtual int registerCount() const override;
    virtual const std::string& getTextureRegisterName(int registerIndex) const override;
    virtual const std::string& getSamplerRegisterName(int registerIndex) const override;
    virtual void setTexture(int registerIndex, ITexture* texture) override;
    virtual void setSamplerState(int registerIndex, ISamplerState* state) override;

private:
};

} // namespace detail
} // namespace ln
