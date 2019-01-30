
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
	static uint64_t computeHash(const IGraphicsDeviceContext::State& state);

	// TODO: 長い間使われない Item を消すような処理
};


class VulkanRenderPassCache
	: public HashedObjectCache<VkRenderPass>
{
public:
	// VkRenderPass は、FrameBuffer のフォーマットというかスキーマのようなものを管理する。
	// ほかのオブジェクトを参照することはない。
	// ひとまず、アプリ動作中に VkRenderPass を Destroy はせず、終了時にすべて Destory する。

	static uint64_t computeHash(ITexture* const* renderTargets, size_t renderTargetCount, IDepthBuffer* depthBuffer);
};

class VulkanFrameBuffer
	: public RefObject
{
public:
	bool init(VulkanDeviceContext* deviceContext, ITexture* const* renderTargets, size_t renderTargetCount, IDepthBuffer* depthBuffer);
	void dispose();
	bool containsRenderTarget(ITexture* renderTarget) const;
	bool containsDepthBuffer(IDepthBuffer* depthBuffer) const;
	VkRenderPass vulkanRenderPass() const { return m_renderPass; }
	VkFramebuffer vulkanFramebuffer() const { return m_framebuffer; }
	SizeI extent() const { return m_renderTargets[0]->realSize(); }

private:
	VulkanDeviceContext* m_deviceContext;
	VkRenderPass m_renderPass;
	VkFramebuffer m_framebuffer;
	std::array<ITexture*, IGraphicsDeviceContext::MaxRenderTargets> m_renderTargets = {};
	size_t m_renderTargetCount;
	IDepthBuffer* m_depthBuffer = nullptr;
};

class VulkanFrameBufferCache
	: public HashedObjectCache<Ref<VulkanFrameBuffer>>
{
public:
	static uint64_t computeHash(ITexture* const* renderTargets, size_t renderTargetCount, IDepthBuffer* depthBuffer)
	{
		return VulkanRenderPassCache::computeHash(renderTargets, renderTargetCount, depthBuffer);
	}

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
    VkPhysicalDevice vulkanPhysicalDevice() const;
    const Ref<VulkanQueue>& graphicsQueue() const { return m_graphicsQueue; }
    const Ref<VulkanQueue>& computeQueue() const { return m_computeQueue; }
    const Ref<VulkanQueue>& transferQueue() const { return m_transferQueue; }
	VulkanRenderPassCache& renderPassCache() { return m_renderPassCache; }
	VulkanPipelineCache& pipelineCache() { return m_pipelineCache; }
	VulkanFrameBufferCache& frameBufferCache() { return m_frameBufferCache; }
    const Ref<VulkanCommandList>& activeCommandBuffer() const { return m_activeCommandBuffer; }
	void setActiveCommandBuffer(const Ref<VulkanCommandList>& commandBuffer) { m_activeCommandBuffer = commandBuffer; }

	bool getVkRenderPass(ITexture* const* renderTargets, size_t renderTargetCount, IDepthBuffer* depthBuffer, VkRenderPass* outPass);
    bool beginActiveCommandBuffer();
    bool endActiveCommandBuffer();

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
    virtual Ref<IShaderPass> onCreateShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag) override;
    virtual void onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState) override;
    virtual void onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer) override;
    virtual void onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize) override;
    virtual void onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer) override;
    virtual void onUpdateShaderPass(IShaderPass* newPass) override;
    virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
    virtual void onDrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount) override;
    virtual void onDrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount) override;
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
    //static const uint32_t MaxBufferCount = 2;

    VulkanQueue();
    bool init(VulkanDeviceContext* deviceContext, uint32_t familyIndex, uint32_t queueIndex, uint32_t maxSubmitCount);
    void dispose();

    uint32_t familyIndex() const { return m_familyIndex; }
    //uint32_t currentBufferIndex() const { return m_currentBufferIndex; }
    //uint32_t previousBufferIndex() const { return m_previousBufferIndex; }

    VkQueue vulkanQueue() const { return m_queue; }
    //VkSemaphore signalSemaphore(uint32_t index) const { return m_signalSemaphore[index]; }
    //VkSemaphore vulkanWaitSemaphore(uint32_t index) const { return m_waitSemaphore[index]; }
    //VkFence vulkanFence(uint32_t index) const { return m_fence[index]; }

private:
    VulkanDeviceContext* m_deviceContext;
    uint32_t m_familyIndex;
    uint32_t m_maxSubmitCount;
    VkQueue m_queue;
    //VkSemaphore m_signalSemaphore[MaxBufferCount];
    //VkSemaphore m_waitSemaphore[MaxBufferCount];
    //VkFence m_fence[MaxBufferCount];
    std::vector<VkCommandBuffer> m_submitList;

    //uint32_t m_submitIndex;
    //uint32_t m_currentBufferIndex;
    //uint32_t m_previousBufferIndex;
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
	VkFence vulkanInFlightFence() const { return m_inFlightFence; }

    bool begin();
	bool end();
	bool flush();

	void addPipelineCmd(VulkanPipeline* pipeline);
	void addFrameBufferCmd(VulkanFrameBuffer* frameBuffer);

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

private:
	VulkanDeviceContext* m_deviceContext;
	VkPipeline m_pipeline;	// 管理は Cache に任せるので、このクラスの中で Destroy しない
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
    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imageViews;
    std::vector<Ref<VulkanRenderTargetTexture>> m_buffers;
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

private:
    std::vector<VkVertexInputBindingDescription> m_bindings;
    std::vector<VkVertexInputAttributeDescription> m_attributes;
};

class VulkanVertexBuffer
    : public IVertexBuffer
{
public:
    VulkanVertexBuffer();
    virtual ~VulkanVertexBuffer();
    void init(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
    virtual void dispose() override;
    virtual size_t getBytesSize() override;
    virtual GraphicsResourceUsage usage() const override;
    virtual void setSubData(size_t offset, const void* data, size_t length) override;
    virtual void* map() override;
    virtual void unmap() override;

private:
};

class VulkanIndexBuffer
    : public IIndexBuffer
{
public:
    VulkanIndexBuffer();
    virtual ~VulkanIndexBuffer();
    void init(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
    virtual void dispose() override;
    virtual size_t getBytesSize() override;
    virtual GraphicsResourceUsage usage() const override;
    virtual void setSubData(size_t offset, const void* data, size_t length) override;
    virtual void* map() override;
    virtual void unmap() override;

private:
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
	bool init(VulkanDeviceContext* deviceContext, const VulkanSwapChain::SwapChainDesc& desc, VkImage image, VkImageView view);
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

private:
	VulkanDeviceContext* m_deviceContext;
	TextureDesc m_desc;
	VkImage m_image;
	VkImageView m_imageView;
	VkImageAspectFlags m_imageAspectFlags;
	VkDeviceMemory m_deviceMemory;
	VkMemoryRequirements m_memoryRequirements;

    TextureFormat m_format;
	bool m_isExternal;
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
    void init(VulkanDeviceContext* context, const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag);
    virtual void dispose() override;

    virtual int getUniformCount() const override;
    virtual IShaderUniform* getUniform(int index) const override;
    virtual void setUniformValue(int index, const void* data, size_t size) override;
    virtual int getUniformBufferCount() const override;
    virtual IShaderUniformBuffer* getUniformBuffer(int index) const override;
    virtual IShaderSamplerBuffer* samplerBuffer() const override;

private:
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
