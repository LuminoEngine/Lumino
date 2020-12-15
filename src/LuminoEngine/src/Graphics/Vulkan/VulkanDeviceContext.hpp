
#pragma once
#include "VulkanHelper.hpp"
#include <LuminoEngine/Graphics/GraphicsExtensionVulkan.hpp>

struct GLFWwindow;

//struct QueueFamilyIndices {
//	ln::Optional<uint32_t> graphicsFamily;
//	//ln::Optional<uint32_t> presentFamily;
//
//	bool isComplete() {
//        return graphicsFamily.hasValue();// && presentFamily.hasValue();
//	}
//};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    VkSurfaceTransformFlagBitsKHR preTransform;
    VkCompositeAlphaFlagBitsKHR compositeAlpha;
};

namespace ln {
namespace detail {
class VulkanGraphicsContext;
class VulkanSwapChain;
class VulkanRenderTarget;
class VulkanDepthBuffer;
class VulkanShaderDescriptorTable;
class VulkanSingleFrameAllocatorPageManager;
class VulkanNativeGraphicsInterface;

class VulkanDevice
	: public IGraphicsDevice
{
public:
	struct Settings
	{
		PlatformWindow* mainWindow = nullptr;
        bool debugMode = false;
	};

	VulkanDevice();
	bool init(const Settings& settings, bool* outIsDriverSupported);
	void dispose() override;

    VkInstance vulkanInstance() const { return m_instance; }
    VkPhysicalDevice vulkanPhysicalDevice() const { return m_physicalDevice; }
    VkDevice vulkanDevice() const { return m_device; }
    const VkAllocationCallbacks* vulkanAllocator() const { return nullptr; }// TODO: return m_allocator.vulkanAllocator();
    VkCommandPool vulkanCommandPool() const { return m_commandPool; }
    uint32_t graphicsQueueFamilyIndex() const { return m_graphicsQueueFamilyIndex; }
    Result findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t* outType);
    const Ref<VulkanSingleFrameAllocatorPageManager>& uniformBufferSingleFrameAllocator() const { return m_uniformBufferSingleFrameAllocator; }
    const Ref<VulkanSingleFrameAllocatorPageManager>& transferBufferSingleFrameAllocator() const { return m_transferBufferSingleFrameAllocator; }
    VulkanNativeGraphicsInterface* vulkanNativeGraphicsInterface() const { return m_nativeInterface.get(); }

protected:
	INativeGraphicsInterface* getNativeInterface() const override;
	void onGetCaps(GraphicsDeviceCaps* outCaps) override;
	Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
	Ref<ICommandList> onCreateCommandList() override;
	Ref<IRenderPass> onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) override;
	Ref<IPipeline> onCreatePipeline(const DevicePipelineStateDesc& state) override;
	Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
	Ref<IVertexBuffer> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
	Ref<IIndexBuffer> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
	Ref<ITexture> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
	Ref<ITexture> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
	Ref<ITexture> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap) override;
    Ref<ITexture> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) override { LN_NOTIMPLEMENTED(); return nullptr; }
    Ref<IDepthBuffer> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
	Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) override;
	Ref<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) override;
    Ref<IUniformBuffer> onCreateUniformBuffer(uint32_t size) override;
	void onFlushCommandBuffer(ICommandList* context, ITexture* affectRendreTarget) override;
	ICommandQueue* getGraphicsCommandQueue() override;
	ICommandQueue* getComputeCommandQueue() override;

public: // TODO:
    struct PhysicalDeviceInfo
    {
        VkPhysicalDevice device;
        VkPhysicalDeviceMemoryProperties memoryProperty;
        VkPhysicalDeviceProperties deviceProperty;
    };

    Result createInstance();
    Result setupDebugMessenger();
    Result pickPhysicalDevice();
	Result createLogicalDevice();
    Result createCommandPool();

	//QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool findPresentQueueFamily(VkSurfaceKHR surface, uint32_t* outIndex);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();

    VkCommandBuffer beginSingleTimeCommands();
    Result endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBufferImmediately(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void copyBufferToImageImmediately(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    Result transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, uint32_t mipLevel, VkImageLayout oldLayout, VkImageLayout newLayout);
    Result transitionImageLayoutImmediately(VkImage image, VkFormat format, uint32_t mipLevel, VkImageLayout oldLayout, VkImageLayout newLayout);


	//GLFWwindow* m_mainWindow; // TODO:
    VulkanSwapChain* m_mainSwapchain = nullptr; // TODO:

    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_device;
    uint32_t m_graphicsQueueFamilyIndex;
	VkQueue m_graphicsQueue;
    VkCommandPool m_commandPool;
    //VulkanAllocator m_allocator;
    Ref<VulkanSingleFrameAllocatorPageManager> m_uniformBufferSingleFrameAllocator;
    Ref<VulkanSingleFrameAllocatorPageManager> m_transferBufferSingleFrameAllocator;


    //VulkanRenderPassCache m_renderPassCache;
    //VulkanFramebufferCache m_framebufferCache;
    //VulkanPipelineCache m_pipelineCache;

	//Ref<VulkanGraphicsContext> m_graphicsContext;
	std::unique_ptr<VulkanNativeGraphicsInterface> m_nativeInterface;


    std::vector<PhysicalDeviceInfo> m_physicalDeviceInfos;
    bool m_enableValidationLayers;
};

class VulkanGraphicsContext
	: public ICommandList
{
public:
	VulkanGraphicsContext();
    bool init(VulkanDevice* owner);
	void dispose();

    const Ref<VulkanCommandBuffer>& recodingCommandBuffer() const { return m_recodingCommandBuffer; }
    void setRecodingCommandBuffer(const Ref<VulkanCommandBuffer>& value) { m_recodingCommandBuffer = value; }

protected:
    void onSaveExternalRenderState() override;
    void onRestoreExternalRenderState() override;
	void onBeginCommandRecoding() override;
	void onEndCommandRecoding() override;
	void onBeginRenderPass(IRenderPass* renderPass) override;
	void onEndRenderPass(IRenderPass* renderPass) override;
	void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) override;
	void* onMapResource(IGraphicsRHIBuffer* resource, uint32_t offset, uint32_t size) override;
	void onUnmapResource(IGraphicsRHIBuffer* resource) override;
	void onSetSubData(IGraphicsRHIBuffer* resource, size_t offset, const void* data, size_t length) override;
	void onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize) override;
	void onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;
    void onSetDescriptorTableData(IShaderDescriptorTable* resource, const ShaderDescriptorTableUpdateInfo* data) override;
	void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
	void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount) override;
	void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset) override;
	void onDrawExtension(INativeGraphicsExtension* extension) override;

private:
	VulkanDevice* m_device;
    Ref<VulkanCommandBuffer> m_recodingCommandBuffer;
};

class VulkanSwapChain
	: public ISwapChain
{
public:
	VulkanSwapChain();
	Result init(VulkanDevice* deviceContext, PlatformWindow* window, const SizeI& backbufferSize);
    void dispose() override;
    uint32_t getBackbufferCount() override;
    void acquireNextImage(int* outImageIndex) override;
    ITexture* getRenderTarget(int imageIndex) const override;
    Result resizeBackbuffer(uint32_t width, uint32_t height) override;
    void present() override;

    VkSwapchainKHR vulkanSwapchain() const { return m_swapchain; }
    VkFormat vulkanSwapchainImageFormat() const { return m_swapchainImageFormat; }
    VkExtent2D vulkanSwapchainExtent() const { return m_swapchainExtent; }
    const std::vector<Ref<VulkanRenderTarget>>& swapchainRenderTargets() const { return m_swapchainRenderTargets; }
    uint32_t imageIndex() const { return m_imageIndex; }

    uint32_t maxFrameCount() const { return m_swapchainRenderTargets.size(); }

    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t requiredWidth, uint32_t requiredHeight);
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
	bool createNativeSwapchain(const SizeI& backbufferSize);
    void cleanupNativeSwapchain();

    VulkanDevice* m_deviceContext;
    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapchain;
    VkQueue m_presentQueue;
    VkFormat m_swapchainImageFormat;
    VkExtent2D m_swapchainExtent;

    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<Ref<VulkanRenderTarget>> m_swapchainRenderTargets;
    uint32_t m_imageIndex;

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    //std::vector<VkSemaphore> m_renderFinishedSemaphores;
    uint32_t m_currentFrame;

    std::vector<Ref<VulkanCommandBuffer>> m_inFlightCommandBuffers;
};

class VulkanFramebuffer2;

// Vulkan のデータ構造としては RenderPass と Framebuffer は 1:n. Subpass とかを使うときに同一のフォーマットであれば、ある RenderPass の begin～end の間に複数の Framebuffer を使うことができる。（ハズ。要確認）
// ただ、複雑さ回避のため、ひとまず subpass は対応しない。
// そのため Lumino のデータ構造としては１つのRenderPassの中に1つのFramebufferを持たせている。
class VulkanRenderPass2
	: public IRenderPass
{
public:
	VulkanRenderPass2();
	Result init(VulkanDevice* device, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil);
	void dispose() override;
	VkRenderPass nativeRenderPass() const { return m_nativeRenderPass; }
	const Ref<VulkanFramebuffer2>& framebuffer() const { return m_framebuffer; }

	ClearFlags clearFlags() const { return m_clearFlags; }
	const Color& clearColor() const { return m_clearColor; }
	float clearDepth() const { return m_clearDepth; }
	uint8_t clearStencil() const { return m_clearStencil; }

private:
	VulkanDevice* m_device;
	VkRenderPass m_nativeRenderPass;
	Ref<VulkanFramebuffer2> m_framebuffer;
	ClearFlags m_clearFlags;
	Color m_clearColor;
	float m_clearDepth;
	uint8_t m_clearStencil;
#ifdef LN_DEBUG
    VkAttachmentDescription m_attachmentDescs[MaxMultiRenderTargets] = {};
    VkAttachmentReference m_attachmentRefs[MaxMultiRenderTargets] = {};
#endif
};

class VulkanFramebuffer2
	: public RefObject
{
public:
	VulkanFramebuffer2();
	Result init(VulkanDevice* device, VulkanRenderPass2* ownerRenderPass, const DeviceFramebufferState& state);
	void dispose();
	VulkanRenderPass2* ownerRenderPass() const { return m_ownerRenderPass; }
	VkFramebuffer nativeFramebuffer() const { return m_framebuffer; }
	const std::array<Ref<VulkanRenderTarget>, MaxMultiRenderTargets>& renderTargets() const { return m_renderTargets; }
	const Ref<VulkanDepthBuffer>& depthBuffer() const { return m_depthBuffer; }

private:
	VulkanDevice* m_device;
	VulkanRenderPass2* m_ownerRenderPass;
	VkFramebuffer m_framebuffer;
	std::array<Ref<VulkanRenderTarget>, MaxMultiRenderTargets> m_renderTargets;
	Ref<VulkanDepthBuffer> m_depthBuffer;
};

// Dynamic としてマークしている state は次の通り。
// - VK_DYNAMIC_STATE_VIEWPORT,
// - VK_DYNAMIC_STATE_SCISSOR,
// - VK_DYNAMIC_STATE_BLEND_CONSTANTS,
// - VK_DYNAMIC_STATE_STENCIL_REFERENCE,
// なお、これらは computeHash に含める必要はない。
class VulkanPipeline2
	: public IPipeline
{
public:
	VulkanPipeline2();
	Result init(VulkanDevice* deviceContext, const DevicePipelineStateDesc& state);
	void dispose() override;
	VkPipeline nativePipeline() const { return m_pipeline; }

private:
	VulkanDevice* m_device;
	VulkanRenderPass2* m_ownerRenderPass;
	VkPipeline m_pipeline;
};

class VulkanVertexDeclaration
    : public IVertexDeclaration
{
public:
    // 実際の VkVertexInputAttributeDescription を作るための元情報。
    // location は、Pipleline 作成時に m_elements が持っているセマンティクス情報と、ShaderPass が持っているセマンティクス情報とつなげて確定させる。
    struct AttributeDescriptionSource
    {
        VertexElementUsage usage;
        uint32_t usageIndex;

        uint32_t binding;   // DirectX でいうところの StreamIndex。頂点バッファを複数分けるときに使う。
        VkFormat format;
        uint32_t offset;
    };

    VulkanVertexDeclaration();
    Result init(const VertexElement* elements, int elementsCount);
    void dispose() override;

    const std::vector<VertexElement>& elements() const { return m_elements; }
    const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions() const { return m_bindings; }
    const std::vector<AttributeDescriptionSource>& vertexAttributeDescriptionSources() const { return m_attributeSources; }
    const VulkanVertexDeclaration::AttributeDescriptionSource* findAttributeDescriptionSource(AttributeUsage usage, int usageIndex) const;

private:

    std::vector<VertexElement> m_elements;
    uint32_t m_maxStreamCount;
    std::vector<VkVertexInputBindingDescription> m_bindings;
    std::vector<AttributeDescriptionSource> m_attributeSources;
};

class VulkanVertexBuffer
    : public IVertexBuffer
{
public:
    VulkanVertexBuffer();
    Result init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
    void dispose() override;
    size_t getBytesSize() override;
    GraphicsResourceUsage usage() const override;
    void* map() override;
    void unmap() override;

    VulkanBuffer* buffer() { return &m_buffer; }
    VkBuffer vulkanBuffer() const { return m_buffer.nativeBuffer(); }
    VkDeviceMemory vulkanDeviceMemory() const { return m_buffer.nativeBufferMemory(); }

    VulkanSingleFrameBufferInfo m_mappedResource = { nullptr, 0 };

private:
    VulkanDevice* m_deviceContext;
    VulkanBuffer m_buffer;
    GraphicsResourceUsage m_usage;
};

class VulkanIndexBuffer
    : public IIndexBuffer
{
public:
    VulkanIndexBuffer();
    Result init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
    void dispose() override;
    size_t getBytesSize() override;
    GraphicsResourceUsage usage() const override;
    void* map() override;
    void unmap() override;

    VulkanBuffer* buffer() { return &m_buffer; }
    VkBuffer vulkanBuffer() const { return m_buffer.nativeBuffer(); }
    VkDeviceMemory vulkanDeviceMemory() const { return m_buffer.nativeBufferMemory(); }
    VkIndexType indexType() const { return m_indexType; }

    VulkanSingleFrameBufferInfo m_mappedResource = { nullptr, 0 };

protected:
    VulkanDevice* m_deviceContext;
    VulkanBuffer m_buffer;
    GraphicsResourceUsage m_usage;
    VkIndexType m_indexType;
};

class VulkanUniformBuffer
    : public IUniformBuffer
{
public:
    VulkanUniformBuffer();
    Result init(VulkanDevice* deviceContext, uint32_t size);
    void dispose() override;
    void* map() override;
    void unmap() override;
    VulkanBuffer* buffer() { return &m_buffer; }

protected:
    VulkanBuffer m_buffer;
};

class VulkanTexture
    : public ITexture
{
public:
    virtual const VulkanImage* image() const = 0;
    virtual void setSubData(VulkanGraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
    virtual void setSubData3D(VulkanGraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

private:
};

class VulkanTexture2D
	: public VulkanTexture
{
public:
	VulkanTexture2D();
	Result init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData);
    virtual void dispose();
	virtual DeviceTextureType type() const { return DeviceTextureType::Texture2D; }
	virtual SizeI realSize() { return m_size; }
    virtual TextureFormat getTextureFormat() const { return m_format; }
	virtual GraphicsResourceUsage usage() const override { return m_usage; }
	virtual void readData(void* outData) { LN_UNREACHABLE(); }
    virtual void setSubData(VulkanGraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) override;
	virtual void setSubData3D(VulkanGraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) { LN_UNREACHABLE(); }

    virtual const VulkanImage* image() const override { return &m_image; }

private:
	Result generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	VulkanDevice* m_deviceContext;
    VulkanImage m_image;
	GraphicsResourceUsage m_usage;
	SizeI m_size;
	TextureFormat m_format;
	uint32_t m_mipLevels;
    VkFormat m_nativeFormat;
};

class VulkanRenderTarget
	: public VulkanTexture
{
public:
	VulkanRenderTarget();
    Result init(VulkanDevice* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap);
	Result init(VulkanDevice* deviceContext, uint32_t width, uint32_t height, VkFormat format, VkImage image, VkImageView imageView);
    virtual void dispose() override;
	virtual DeviceTextureType type() const { return DeviceTextureType::RenderTarget; }
	virtual SizeI realSize() { return m_size; }
	virtual TextureFormat getTextureFormat() const {return TextureFormat::RGBA8; }
	virtual GraphicsResourceUsage usage() const override { return GraphicsResourceUsage::Static; }
    virtual void readData(void* outData) override;
	virtual void setSubData(VulkanGraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) {}
	virtual void setSubData3D(VulkanGraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) {}

	virtual const VulkanImage* image() const override { return m_image.get(); }

    bool isSwapchainBackbuffer() const { return m_image->IsExternalManagement(); }
    void setImageAvailableSemaphoreRef(VkSemaphore* semaphore) { m_imageAvailableSemaphoreRef = semaphore; }
    VkSemaphore imageAvailableSemaphore() const { return (m_imageAvailableSemaphoreRef) ? *m_imageAvailableSemaphoreRef : VK_NULL_HANDLE; }
    VkSemaphore renderFinishedSemaphore() const { return m_renderFinishedSemaphore; }

	Result reset(uint32_t width, uint32_t height, VkFormat format, VkImage image, VkImageView imageView);

protected:
    VulkanDevice* m_deviceContext;
	std::unique_ptr<VulkanImage> m_image;
	SizeI m_size;
	TextureFormat m_format;

    // vkAcquireNextImageKHR により準備ができたかどうかを待つためのセマフォ。
    // この RenderTarget が Swapchain の Backbuffer である場合、そのポインタがセットされる。
    // present または readData による　CommandBuffer の Submit 時、これが nullptr でなければ待つようにしなければならない。
    VkSemaphore* m_imageAvailableSemaphoreRef;

    VkSemaphore m_renderFinishedSemaphore;
};

// ダブルバッファ・トリプルバッファを管理するため、通常の RenderTargetTexture とは別にする。
// IGraphicsDevice は DX11 相当の IF に合わせているので、ダブルバッファの実装はこちら側で隠蔽したい。
// 個別に公開するのは、特に readData の対象を外側で認識できるようにする必要があったりするのでちょっと大変。
//class VulkanSwapchainRenderTargetTexture
//    : public VulkanTexture
//{
//public:
//    VulkanSwapchainRenderTargetTexture();
//    Result init(VulkanDevice* deviceContext);
//    virtual void dispose();
//    virtual DeviceTextureType type() const { return DeviceTextureType::Texture2D; }
//    virtual SizeI realSize() { return m_size; }
//    virtual TextureFormat getTextureFormat() const { return m_format; }
//    virtual void readData(void* outData) { LN_UNREACHABLE(); }
//    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) { LN_NOTIMPLEMENTED(); }
//    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) { LN_UNREACHABLE(); }
//    virtual const VulkanImage* image() const override { return m_images[m_currentBufferIndex].get(); }
//
//    Result reset(uint32_t width, uint32_t height, VkFormat format, const std::vector<VkImage>& images, const std::vector<VkImageView>& imageViews);
//    void setCurrentBufferIndex(int index) { m_currentBufferIndex = index; }
//    uint32_t imageCount() const { return m_images.size(); }
//    VulkanImage* image(uint32_t index) const { return m_images[index].get(); }
//
//    //int prevBufferIndex() const { return (m_bufferIndex == 0) ? m_images.size() - 1 : m_bufferIndex - 1; }
//
//private:
//    void clear();
//
//    VulkanDevice* m_deviceContext;
//    std::vector<std::shared_ptr<VulkanImage>> m_images;
//    SizeI m_size;
//    TextureFormat m_format;
//    int m_currentBufferIndex;
//    //TextureDesc m_desc;
//    //std::vector<VkImage> m_images;
//    //std::vector<VkImageView> m_imageViews;
//    ////VkImageAspectFlags m_imageAspectFlags;
//    ////VkDeviceMemory m_deviceMemory;
//
//    //VkFormat m_vulkanFormat;
//};

class VulkanDepthBuffer
	: public IDepthBuffer
{
public:
    VulkanDepthBuffer();
    Result init(VulkanDevice* deviceContext, uint32_t width, uint32_t height);
    void dispose();
    const SizeI& size() const { return m_size; }
    const VulkanImage* image() const { return &m_image; }
	VkFormat nativeFormat() const { return m_deviceContext->findDepthFormat(); }

private:
    VulkanDevice* m_deviceContext;
    SizeI m_size;
    VulkanImage m_image;
};

class VulkanSamplerState
	: public ISamplerState
{
public:
	VulkanSamplerState();
	Result init(VulkanDevice* deviceContext, const SamplerStateData& desc);
	virtual void dispose() override;

    VkSampler vulkanSampler() const { return m_sampler; }

private:
	VulkanDevice* m_deviceContext;
	VkSampler m_sampler;
};

class VulkanShaderPass
    : public IShaderPass
    , public IVulkanInFlightResource
{
public:
    VulkanShaderPass();
    Result init(VulkanDevice* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag);
    void dispose();
    virtual IShaderDescriptorTable* descriptorTable() const;
    virtual void onBind() override { RefObjectHelper::retain(this); }
    virtual void onUnBind() override { RefObjectHelper::release(this); }

    VkShaderModule vulkanVertShaderModule() const { return m_vertShaderModule; }
    VkShaderModule vulkanFragShaderModule() const { return m_fragShaderModule; }
    const std::string& vertEntryPointName() const { return m_vertEntryPointName; }
    const std::string& fragEntryPointName() const { return m_fragEntryPointName; }
    VkPipelineLayout vulkanPipelineLayout() const { return m_pipelineLayout; }
    const std::array<VkDescriptorSetLayout, 3>& descriptorSetLayouts() const { return m_descriptorSetLayouts; }

    const std::vector<VkWriteDescriptorSet>& submitDescriptorWriteInfo(VulkanCommandBuffer* commandBuffer, const std::array<VkDescriptorSet, DescriptorType_Count>& descriptorSets);

    // CommandBuffer に対するインターフェイス
    Ref<VulkanDescriptorSetsPool> getDescriptorSetsPool();
    void releaseDescriptorSetsPool(VulkanDescriptorSetsPool* pool);
    //VulkanDescriptorSetsPool* recodingPool = nullptr; // CommandBuffer に対する、いわゆる UserData のイメージ

    const VkWriteDescriptorSet& witeInfo(int index) const { return m_descriptorWriteInfo[index]; }

private:
    VulkanDevice* m_deviceContext;
    VkShaderModule m_vertShaderModule;
    VkShaderModule m_fragShaderModule;
    std::string m_vertEntryPointName;
    std::string m_fragEntryPointName;
    VkPipelineLayout m_pipelineLayout;
    std::array<VkDescriptorSetLayout, 3> m_descriptorSetLayouts;
    std::vector<Ref<VulkanDescriptorSetsPool>> m_descriptorSetsPools;

    std::vector<VkWriteDescriptorSet> m_descriptorWriteInfo;
    std::vector<VkDescriptorBufferInfo> m_bufferDescriptorBufferInfo;
    std::vector<VkDescriptorImageInfo> m_textureDescripterImageInfo;
    std::vector<VkDescriptorImageInfo> m_samplerDescripterImageInfo;

    Ref<VulkanShaderDescriptorTable> m_descriptorTable;
};

class VulkanShaderDescriptorTable
    : public IShaderDescriptorTable
{
public:
    struct UniformBufferInfo
    {
        int descriptorWriteInfoIndex = -1;  // index of VulkanShaderPass::m_descriptorWriteInfo
        int bindingIndex = 0;
        ShaderDescriptorBufferView bufferView;
    };
    struct ImageBufferInfo
    {
        int descriptorWriteInfoIndex = -1;  // index of VulkanShaderPass::m_descriptorWriteInfo
        //int descriptorImageInfoIndex;
        int bindingIndex = 0;
        VkDescriptorImageInfo imageInfo;
        Ref<VulkanTexture> texture;
        Ref<VulkanSamplerState> samplerState;
    };

    VulkanShaderDescriptorTable();
    bool init(VulkanDevice* deviceContext, const VulkanShaderPass* ownerPass, const DescriptorLayout* descriptorLayout);
    void dispose() override;
    void setData(const ShaderDescriptorTableUpdateInfo* data);

    const std::vector<UniformBufferInfo>& uniforms() const { return m_uniforms; }
    const std::vector<ImageBufferInfo>& textures() const { return m_textures; }
    const std::vector<ImageBufferInfo>& samplers() const { return m_samplers; }

private:

    std::vector<UniformBufferInfo> m_uniforms;  // 'b' register
    std::vector<ImageBufferInfo> m_textures;    // 't' register
    std::vector<ImageBufferInfo> m_samplers;    // 's' register
};











class VulkanNativeGraphicsInterface : public IVulkanNativeGraphicsInterface
{
public:
	VulkanNativeGraphicsInterface(VulkanDevice* device)
		: m_device(device)
        , m_context(nullptr)
	{}

    void setContext(VulkanGraphicsContext* context) { m_context = context; }

	virtual GraphicsAPI getGraphicsAPI() const override { return GraphicsAPI::Vulkan; }
	virtual VkInstance getInstance() const override { return m_device->vulkanInstance(); }
	virtual VkPhysicalDevice getPhysicalDevice() const override { return m_device->vulkanPhysicalDevice(); }
	virtual VkDevice getDevice() const override { return m_device->vulkanDevice(); }
	virtual VkQueue getGraphicsQueue() const override { return m_device->m_graphicsQueue; }
	virtual uint32_t getGraphicsQueueFamilyIndex() const override { return m_device->m_graphicsQueueFamilyIndex; }
    virtual VkCommandBuffer getRecordingCommandBuffer() const { return m_context->recodingCommandBuffer()->vulkanCommandBuffer(); }

private:
	VulkanDevice* m_device;
    VulkanGraphicsContext* m_context;
};

} // namespace detail
} // namespace ln
