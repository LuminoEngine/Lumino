
#pragma once
#include "VulkanHelper.hpp"
#include <LuminoGraphics/GraphicsExtensionVulkan.hpp>

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
class VulkanTexture;
class VulkanRenderTarget;
class VulkanDepthBuffer;
//class VulkanShaderDescriptorTable;
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
    VkSampleCountFlagBits msaaSamples() const { return m_msaaSamples; }
    //const Ref<VulkanSingleFrameAllocatorPageManager>& uniformBufferSingleFrameAllocator() const { return m_uniformBufferSingleFrameAllocator; }
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
	Ref<RHIResource> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
	Ref<RHIResource> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
	Ref<RHIResource> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
	Ref<RHIResource> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
	Ref<RHIResource> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) override;
    Ref<RHIResource> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) override { LN_NOTIMPLEMENTED(); return nullptr; }
    Ref<RHIResource> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
	Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) override;
	Ref<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) override;
    Ref<RHIResource> onCreateUniformBuffer(uint32_t size) override;
    Ref<IDescriptorPool> onCreateDescriptorPool(IShaderPass* shaderPass) override;
	void onSubmitCommandBuffer(ICommandList* context, RHIResource* affectRendreTarget) override;
	ICommandQueue* getGraphicsCommandQueue() override;
	ICommandQueue* getComputeCommandQueue() override;

public: // TODO:
    struct PhysicalDeviceInfo
    {
        VkPhysicalDevice device;
        VkPhysicalDeviceMemoryProperties memoryProperty;
        VkPhysicalDeviceProperties deviceProperty;
    };

    struct ImageFormatProperty
    {
        VkFormat format;
        VkFormatProperties props;
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

    std::vector<const char*> m_availableValidationLayers;
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_device;
    uint32_t m_graphicsQueueFamilyIndex;
	VkQueue m_graphicsQueue;
    VkCommandPool m_commandPool;
    //VulkanAllocator m_allocator;
    //Ref<VulkanSingleFrameAllocatorPageManager> m_uniformBufferSingleFrameAllocator;
    Ref<VulkanSingleFrameAllocatorPageManager> m_transferBufferSingleFrameAllocator;


    //VulkanRenderPassCache m_renderPassCache;
    //VulkanFramebufferCache m_framebufferCache;
    //VulkanPipelineCache m_pipelineCache;

	//Ref<VulkanGraphicsContext> m_graphicsContext;
	std::unique_ptr<VulkanNativeGraphicsInterface> m_nativeInterface;

    std::vector<VkQueueFamilyProperties> m_queueFamilyProps;
    std::vector<ImageFormatProperty> m_imageFormatProperties;
    VkPhysicalDeviceMemoryProperties m_deviceMemoryProperties;
    std::vector<PhysicalDeviceInfo> m_physicalDeviceInfos;
    int32_t m_activePhysicalDeviceInfoIndex;
    VkSampleCountFlagBits m_msaaSamples;
    bool m_enableValidationLayers;
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
    RHIResource* getRenderTarget(int imageIndex) const override;
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
    bool containsMultisampleTarget() const { return m_containsMultisampleTarget; }

private:
	VulkanDevice* m_device;
	VkRenderPass m_nativeRenderPass;
	Ref<VulkanFramebuffer2> m_framebuffer;
	ClearFlags m_clearFlags;
	Color m_clearColor;
	float m_clearDepth;
	uint8_t m_clearStencil;
    bool m_containsMultisampleTarget;
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
	const std::array<VulkanRenderTarget*, MaxMultiRenderTargets>& renderTargets() const { return m_renderTargets; }
	const VulkanDepthBuffer* depthBuffer() const { return m_depthBuffer; }

private:
	VulkanDevice* m_device;
	VulkanRenderPass2* m_ownerRenderPass;
	VkFramebuffer m_framebuffer;
	std::array<VulkanRenderTarget*, MaxMultiRenderTargets> m_renderTargets;
	VulkanDepthBuffer* m_depthBuffer;

    VkFramebufferCreateInfo ff;
    VulkanRenderTarget* _sizeBase;
    const VulkanImage* _sizeBaseImg;
    VkImage _baseImg;
    int32_t _baseId = 0; 
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
    Result createGraphicsPipeline(const DevicePipelineStateDesc& state);
    Result createComputePipeline(const DevicePipelineStateDesc& state);

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
    uint32_t maxStreamCount() const { return m_maxStreamCount; }
    const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions() const { return m_bindings; }
    const std::vector<AttributeDescriptionSource>& vertexAttributeDescriptionSources() const { return m_attributeSources; }
    const VulkanVertexDeclaration::AttributeDescriptionSource* findAttributeDescriptionSource(AttributeUsage usage, int usageIndex) const;

private:

    std::vector<VertexElement> m_elements;
    uint32_t m_maxStreamCount;
    std::vector<VkVertexInputBindingDescription> m_bindings;
    std::vector<AttributeDescriptionSource> m_attributeSources;
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










class VulkanNativeGraphicsInterface : public IVulkanNativeGraphicsInterface
{
public:
    VulkanNativeGraphicsInterface(VulkanDevice* device);
    void setContext(VulkanGraphicsContext* context);

    virtual GraphicsAPI getGraphicsAPI() const override;
    virtual VkInstance getInstance() const override;
    virtual VkPhysicalDevice getPhysicalDevice() const override;
    virtual VkDevice getDevice() const override;
    virtual VkQueue getGraphicsQueue() const override;
    virtual uint32_t getGraphicsQueueFamilyIndex() const override;
    virtual VkCommandBuffer getRecordingCommandBuffer() const;

private:
	VulkanDevice* m_device;
    VulkanGraphicsContext* m_context;
};

} // namespace detail
} // namespace ln
