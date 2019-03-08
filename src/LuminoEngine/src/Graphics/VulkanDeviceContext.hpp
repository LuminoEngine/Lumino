
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
class VulkanShaderUniformBuffer;
class VulkanLocalShaderSamplerBuffer;

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
    VkCommandPool vulkanCommandPool() const { return m_commandPool; }
     
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
    Result createCommandPool();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBufferImmediately(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void copyBufferToImageImmediately(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    Result transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

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

class VulkanVertexDeclaration
    : public IVertexDeclaration
{
public:
    // 実際の VkVertexInputAttributeDescription を作るための元情報。
    // location は、Pipleline 作成時に m_elements が持っているセマンティクス情報と、ShaderPass が持っているセマンティクス情報とつなげて確定させる。
    struct AttributeDescriptionSource
    {
        uint32_t binding;   // DirectX でいうところの StreamIndex。頂点バッファを複数分けるときに使う。
        VkFormat format;
        uint32_t offset;
    };

    VulkanVertexDeclaration();
    Result init(const VertexElement* elements, int elementsCount);
    virtual void dispose() override;

    const std::vector<VertexElement>& elements() const { return m_elements; }
    const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions() const { return m_bindings; }
    const std::vector<AttributeDescriptionSource>& vertexAttributeDescriptionSources() const { return m_attributeSources; }

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
    Result init(VulkanDeviceContext* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
    virtual void dispose() override;
    virtual size_t getBytesSize() override { return m_buffer.size(); }
    virtual GraphicsResourceUsage usage() const override { return m_usage; }
    virtual void setSubData(size_t offset, const void* data, size_t length) override;
    virtual void* map() override { return m_buffer.map(); }
    virtual void unmap() override { m_buffer.unmap(); }

    VulkanBuffer* buffer() { return &m_buffer; }
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
    Result init(VulkanDeviceContext* deviceContext, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
    virtual void dispose() override;
    
    virtual size_t getBytesSize() override { return m_buffer.size(); }
    virtual GraphicsResourceUsage usage() const override { return m_usage; }
    virtual void setSubData(size_t offset, const void* data, size_t length) override;
    virtual void* map() override { return m_buffer.map(); }
    virtual void unmap() override { m_buffer.unmap(); }

    VulkanBuffer* buffer() { return &m_buffer; }
    VkBuffer vulkanBuffer() const { return m_buffer.vulkanBuffer(); }
    VkDeviceMemory vulkanDeviceMemory() const { return m_buffer.vulkanBufferMemory(); }
    VkIndexType indexType() const { return m_indexType; }

protected:
    VulkanBuffer m_buffer;
    GraphicsResourceUsage m_usage;
    VkIndexType m_indexType;
};

class VulkanTexture
    : public ITexture
{
public:
    virtual const VulkanImage* image() const = 0;

private:
};

class VulkanTexture2D
	: public VulkanTexture
{
public:
	VulkanTexture2D();
	Result init(VulkanDeviceContext* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData);
    virtual void dispose();
	virtual DeviceTextureType type() const { return DeviceTextureType::Texture2D; }
	virtual SizeI realSize() { return m_size; }
    virtual TextureFormat getTextureFormat() const { return m_format; }
	virtual void readData(void* outData) { LN_UNREACHABLE(); }
	virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) { LN_NOTIMPLEMENTED(); }
	virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) { LN_UNREACHABLE(); }

    virtual const VulkanImage* image() const override { return &m_image; }

private:
	VulkanDeviceContext* m_deviceContext;
    VulkanImage m_image;
	SizeI m_size;
	TextureFormat m_format;
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
    VulkanDepthBuffer();
    Result init(VulkanDeviceContext* deviceContext, uint32_t width, uint32_t height);
    void dispose();
    const VulkanImage* image() const { return &m_image; }

private:
    VulkanDeviceContext* m_deviceContext;
    VulkanImage m_image;
};

class VulkanSamplerState
	: public ISamplerState
{
public:
	VulkanSamplerState();
	Result init(VulkanDeviceContext* deviceContext, const SamplerStateData& desc);
	virtual void dispose() override;

    VkSampler vulkanSampler() const { return m_sampler; }

private:
	VulkanDeviceContext* m_deviceContext;
	VkSampler m_sampler;
};

class VulkanShaderPass
    : public IShaderPass
    , public IVulkanInFlightResource
{
public:
    VulkanShaderPass();
    Result init(VulkanDeviceContext* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag);
    void dispose();
    virtual int getUniformBufferCount() const override { return 0; }
    virtual IShaderUniformBuffer* getUniformBuffer(int index) const override { return nullptr; }
    virtual IShaderSamplerBuffer* samplerBuffer() const override;
    virtual void onBind() override { RefObjectHelper::retain(this); }
    virtual void onUnBind() override { RefObjectHelper::release(this); }

    VkShaderModule vulkanVertShaderModule() const { return m_vertShaderModule; }
    VkShaderModule vulkanFragShaderModule() const { return m_fragShaderModule; }
    const std::array<VkDescriptorSetLayout, 3>& descriptorSetLayouts() const { return m_descriptorSetLayouts; }

    const std::vector<VkWriteDescriptorSet>& submitDescriptorWriteInfo(const std::array<VkDescriptorSet, DescriptorType_Count>& descriptorSets);

    // CommandBuffer に対するインターフェイス
    Ref<VulkanDescriptorSetsPool> getDescriptorSetsPool();
    void releaseDescriptorSetsPool(VulkanDescriptorSetsPool* pool);
    Ref<VulkanDescriptorSetsPool> recodingPool = nullptr; // CommandBuffer に対する、いわゆる UserData のイメージ

private:
    VulkanDeviceContext* m_deviceContext;
    VkShaderModule m_vertShaderModule;
    VkShaderModule m_fragShaderModule;
    std::array<VkDescriptorSetLayout, 3> m_descriptorSetLayouts;
	std::vector<Ref<VulkanShaderUniformBuffer>> m_uniformBuffers;
    std::vector<Ref<VulkanDescriptorSetsPool>> m_descriptorSetsPools;
    Ref<VulkanLocalShaderSamplerBuffer> m_localShaderSamplerBuffer;

    std::vector<VkWriteDescriptorSet> m_descriptorWriteInfo;
    std::vector<VkDescriptorBufferInfo> m_bufferDescriptorBufferInfo;
    std::vector<VkDescriptorImageInfo> m_textureDescripterImageInfo;
    std::vector<VkDescriptorImageInfo> m_samplerDescripterImageInfo;
};

class VulkanShaderUniformBuffer
	: public IShaderUniformBuffer
{
public:
	VulkanShaderUniformBuffer();
	Result init(VulkanDeviceContext* deviceContext, const std::string& name, size_t size, const std::vector<ShaderUniformInfo>& members);
	virtual void dispose() override;

	virtual const std::string& name() const override { return m_name; }
	virtual int getUniformCount() const override { return 0; }
	virtual IShaderUniform* getUniform(int index) const override { return nullptr; }
	virtual size_t bufferSize() const override { return m_data.size(); }
	virtual void setData(const void* data, size_t size) override;

    VkBuffer vulkanBuffer() const { return m_uniformBuffer.vulkanBuffer(); }
    uint32_t descriptorWriteInfoIndex = 0;
    uint32_t bindingIndex = 0;

private:
	std::string m_name;
	std::vector<byte_t> m_data;
	std::vector<int> m_bindingNumbers;
    VulkanBuffer m_uniformBuffer;
};


class VulkanLocalShaderSamplerBuffer
    : public IShaderSamplerBuffer
{
public:
    VulkanLocalShaderSamplerBuffer();
    Result init(/*const DescriptorLayout* descriptorLayout*/);
    virtual void dispose() override;

    virtual int registerCount() const override { return m_table.size(); }
    virtual const std::string& getTextureRegisterName(int registerIndex) const override { return m_table[registerIndex].textureRegisterName; }
    virtual const std::string& getSamplerRegisterName(int registerIndex) const override { return m_table[registerIndex].samplerRegisterName; }
    virtual void setTexture(int registerIndex, ITexture* texture) override;
    virtual void setSamplerState(int registerIndex, ISamplerState* state) override;

	void addDescriptor(DescriptorType type, uint32_t bindingIndex, const std::string& name, uint32_t imageInfoIndex, uint32_t writeInfoIndex);
    DescriptorType descriptorType(int registerIndex) const { return m_table[registerIndex].type; }
    const Ref<VulkanTexture>& texture(int registerIndex) const { return m_table[registerIndex].texture; }
    const Ref<VulkanSamplerState>& samplerState(int registerIndex) const { return m_table[registerIndex].samplerState; }
    uint32_t descriptorImageInfoIndex(int registerIndex) const { return m_table[registerIndex].descriptorImageInfoIndex; }
    uint32_t descriptorWriteInfoIndex(int registerIndex) const { return m_table[registerIndex].descriptorWriteInfoIndex; }

private:
    // VkDescriptorImageInfo と同じような情報を持つ。
    struct Entry
    {
        DescriptorType type;
        std::string textureRegisterName;
        std::string samplerRegisterName;
        Ref<VulkanTexture> texture;
        Ref<VulkanSamplerState> samplerState;
        uint32_t descriptorImageInfoIndex;
        uint32_t descriptorWriteInfoIndex;
        uint32_t bindingIndex;
    };

    std::vector<Entry> m_table;
};

} // namespace detail
} // namespace ln
