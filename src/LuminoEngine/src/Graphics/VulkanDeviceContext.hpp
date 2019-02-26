
#pragma once
#include "VulkanHelper.hpp"

struct GLFWwindow;

struct QueueFamilyIndices {
	ln::Optional<uint32_t> graphicsFamily;
	ln::Optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.hasValue() && presentFamily.hasValue();
	}
};

namespace ln {
namespace detail {
class VulkanRenderTarget;

class VulkanDeviceContext
	: public IGraphicsDeviceContext
{
public:
	struct Settings
	{
		PlatformWindow* mainWindow = nullptr;
	};

	VulkanDeviceContext();
	bool init(const Settings& settings);
	virtual void dispose() override;

    VkInstance vulkanInstance() const { return m_instance; }
    VkPhysicalDevice vulkanPhysicalDevice() const { return m_physicalDevice; }
    VkDevice vulkanDevice() const { return m_device; }
    const VkAllocationCallbacks* vulkanAllocator() const { return nullptr; }// return m_allocator.vulkanAllocator();

    Result findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t* outType);

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
	virtual Ref<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) override;
	virtual void onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState) override;
	virtual void onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer) override;
	virtual void onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize) override;
	virtual void onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer) override;
	virtual void onUpdateShaderPass(IShaderPass* newPass) override;
	virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
	virtual void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount) override;
	virtual void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount) override;
	virtual void onPresent(ISwapChain* swapChain) override;

public: // TODO:
    struct PhysicalDeviceInfo
    {
        VkPhysicalDevice device;
        VkPhysicalDeviceMemoryProperties memoryProperty;
        VkPhysicalDeviceProperties deviceProperty;
    };

    Result createInstance();
    Result setupDebugMessenger();
	Result createSurface();
    Result pickPhysicalDevice();
	Result createLogicalDevice();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	GLFWwindow* m_mainWindow;
	VkSurfaceKHR m_mainSurface;

    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_device;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
    VkCommandPool m_commandPool;
    //VulkanAllocator m_allocator;

    std::vector<PhysicalDeviceInfo> m_physicalDeviceInfos;
};

class VulkanSwapChain
	: public ISwapChain
{
public:
	VulkanSwapChain();
	bool init();
	virtual ITexture* getColorBuffer() const;

	Ref<VulkanRenderTarget> m_colorBuffer;
};

class VulkanVertexBuffer
    : public IVertexBuffer
{
public:
    VulkanVertexBuffer();
    Result init(VulkanDeviceContext* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
    virtual void dispose() override;
    virtual size_t getBytesSize() override { return m_buffer.size(); }
    virtual GraphicsResourceUsage usage() const override { return m_usage; }
    virtual void setSubData(size_t offset, const void* data, size_t length) override { return m_buffer.setData(offset, data, length); }
    virtual void* map() override { return m_buffer.map(); }
    virtual void unmap() override { m_buffer.unmap(); }

    VkBuffer vulkanBuffer() const { return m_buffer.vulkanBuffer(); }
    VkDeviceMemory vulkanDeviceMemory() const { return m_buffer.vulkanBufferMemory(); }

private:
    VulkanBuffer m_buffer;
    GraphicsResourceUsage m_usage;
};

class VulkanRenderTarget
	: public ITexture
{
public:
	VulkanRenderTarget() {}
	virtual DeviceTextureType type() const { return DeviceTextureType::RenderTarget; }
	virtual SizeI realSize() { return SizeI(); }
	virtual TextureFormat getTextureFormat() const {return TextureFormat::RGBA32; }
	virtual void readData(void* outData) {}
	virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) {}
	virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) {}

protected:
};

class VulkanDepthBuffer
	: public IDepthBuffer
{
public:
	VulkanDepthBuffer() {}
};


} // namespace detail
} // namespace ln
