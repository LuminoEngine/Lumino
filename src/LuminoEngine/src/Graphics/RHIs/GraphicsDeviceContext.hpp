#pragma once
#include <LuminoCore/Base/Result.hpp>
#include <LuminoEngine/Graphics/Common.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Shader/Common.hpp>
#include "../../Shader/UnifiedShader.hpp"

namespace ln {
struct SizeI;
struct Color;
class INativeGraphicsInterface;

namespace detail {
class PlatformWindow;
class ISwapChain;
class ICommandList;
class ICommandQueue;
class IRenderPass;
class IVertexDeclaration;
class IVertexBuffer;
class IIndexBuffer;
class IUniformBuffer;
class ITexture;
class IDepthBuffer;
class ISamplerState;
class IShaderPass;
class IPipeline;
class IDescriptorPool;
class IDescriptor;
class NativeRenderPassCache;
class NativePipelineCache;
class RHIBitmap;

enum class DeviceResourceType
{
    VertexBuffer,
    IndexBuffer,
	UniformBuffer,
};

enum class DeviceTextureType
{
	Texture2D,
	Texture3D,
	RenderTarget,
};

struct GraphicsDeviceCaps
{
	GraphicsAPI graphicsAPI;
	UnifiedShaderTriple requestedShaderTriple;
	bool imageLayoytVFlip = false;
	size_t uniformBufferOffsetAlignment;
};

// obsolete
struct DevicePipelineState
{
	IVertexDeclaration* vertexDeclaration = nullptr;
	BlendStateDesc blendState;
	RasterizerStateDesc rasterizerState;
	DepthStencilStateDesc depthStencilState;
	PrimitiveTopology topology = PrimitiveTopology::TriangleList;
};

struct DevicePipelineStateDesc
{
	BlendStateDesc blendState;
	RasterizerStateDesc rasterizerState;
	DepthStencilStateDesc depthStencilState;
	PrimitiveTopology topology = PrimitiveTopology::TriangleList;
	IVertexDeclaration* vertexDeclaration = nullptr;
	IShaderPass* shaderPass = nullptr;
	IRenderPass* renderPass = nullptr;
};

struct DeviceFramebufferState
{
	std::array<ITexture*, MaxMultiRenderTargets> renderTargets = {};
	IDepthBuffer* depthBuffer = nullptr;
};

struct DeviceRegionRectsState
{
	RectI viewportRect;
	RectI scissorRect;
};

struct DevicePrimitiveState
{
	std::array<IVertexBuffer*, MaxVertexStreams> vertexBuffers = {};
	IIndexBuffer* indexBuffer = nullptr;
};

struct GraphicsContextState
{
    DevicePipelineState pipelineState;
    DeviceFramebufferState framebufferState;
    DeviceRegionRectsState regionRects;
    DevicePrimitiveState primitive;
    IShaderPass* shaderPass = nullptr;
	IDescriptor* descriptor = nullptr;
};

enum GraphicsContextStateDirtyFlags
{
    GraphicsContextStateDirtyFlags_None = 0x0000,
    GraphicsContextStateDirtyFlags_PipelineState = 0x0001,
    GraphicsContextStateDirtyFlags_FrameBuffers = 0x0002,
    GraphicsContextStateDirtyFlags_RegionRects = 0x0004,
    GraphicsContextStateDirtyFlags_Primitives = 0x0008,
    GraphicsContextStateDirtyFlags_ShaderPass = 0x0010,
	GraphicsContextStateDirtyFlags_Descriptor = 0x0020,
    GraphicsContextStateDirtyFlags_All = 0xFFFF,
};

enum GraphicsContextSubmitSource
{
    GraphicsContextSubmitSource_Clear,
    GraphicsContextSubmitSource_Draw,
	GraphicsContextSubmitSource_Extension,
};

struct ShaderVertexInputAttribute
{
	VertexElementUsage usage;
	uint32_t index;
	uint32_t layoutLocation;
};

using ShaderVertexInputAttributeTable = std::vector<ShaderVertexInputAttribute>;

struct ShaderDescriptorBufferView
{
	IUniformBuffer* buffer;
	size_t offset;
	//size_t size;
};

struct ShaderDescriptorCombinedSampler
{
	ITexture* texture;
	ISamplerState* stamplerState;
};

struct ShaderDescriptorTableUpdateInfo
{
	static const int MaxElements = 32;

	// 各要素番号は DataIndex。detail::DescriptorLayout の各メンバと一致する。BindingIndex ではない点に注意。
	std::array<ShaderDescriptorBufferView, MaxElements> uniforms = {};
	std::array<ShaderDescriptorCombinedSampler, MaxElements> textures = {};
	std::array<ShaderDescriptorCombinedSampler, MaxElements> samplers = {};
};

struct ShaderPassCreateInfo
{
	const char* name;
    const byte_t* vsCode;
    size_t vsCodeLen;
    const byte_t* psCode;
    size_t psCodeLen;
    const char* vsEntryPointName;
    const char* psEntryPointName;
    const DescriptorLayout* descriptorLayout;
	const std::vector<VertexInputAttribute>* attributes;
};

class IGraphicsHelper
{
public:
	static AttributeUsage ElementUsageToAttributeUsage(VertexElementUsage value);
	static VertexElementUsage AttributeUsageToElementUsage(AttributeUsage value);
	
};


class RHIObject
{
protected:
	RHIObject() = default;

private:
	RHIObject(const RHIObject&) = delete;
	void operator=(const RHIObject&) = delete;
};

template<class T>
using RHIPtr = std::shared_ptr<T>;

template<class T, class... TArgs>
inline RHIPtr<T> makeRHIRef(TArgs&&... args)
{
	return std::make_shared<T>(std::forward<TArgs>(args)...);
}

class IGraphicsDeviceObject
    : public RefObject
{
protected:
    IGraphicsDeviceObject();
    virtual ~IGraphicsDeviceObject();
    virtual void finalize();
    virtual void dispose();	// Prepare for multiple calls

private:
    bool m_disposed;
};

class IGraphicsRHIBuffer
    : public IGraphicsDeviceObject
{
public:
    virtual DeviceResourceType resourceType() const = 0;

protected:
    virtual ~IGraphicsRHIBuffer();
};

class IGraphicsDevice
	: public RefObject
{
public:
	IGraphicsDevice();
	virtual ~IGraphicsDevice() = default;

	void init();
	virtual void dispose();
	const GraphicsDeviceCaps& caps() { return m_caps; }
	void refreshCaps();

	Ref<ISwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize);
	Ref<ICommandList> createCommandList();
	Ref<IRenderPass> createRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil);
	Ref<IPipeline> createPipeline(const DevicePipelineStateDesc& state);
	Ref<IVertexDeclaration> createVertexDeclaration(const VertexElement* elements, int elementsCount);
	Ref<IVertexBuffer> createVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData = nullptr);
	Ref<IIndexBuffer> createIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData = nullptr);
	Ref<ITexture> createTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData = nullptr);
	Ref<ITexture> createTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData = nullptr);
	Ref<ITexture> createRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa);
    Ref<ITexture> createWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight);
	Ref<IDepthBuffer> createDepthBuffer(uint32_t width, uint32_t height);
	Ref<ISamplerState> createSamplerState(const SamplerStateData& desc);
	Ref<IShaderPass> createShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag);
	Ref<IUniformBuffer> createUniformBuffer(uint32_t size);
	Ref<IDescriptorPool> createDescriptorPool(IShaderPass* shaderPass);
    void releaseObject(IGraphicsDeviceObject* obj) {}

	void submitCommandBuffer(ICommandList* context, ITexture* affectRendreTarget);  // 呼ぶ前に end しておくこと

	virtual INativeGraphicsInterface* getNativeInterface() const = 0;
	virtual ICommandQueue* getGraphicsCommandQueue() = 0;
	virtual ICommandQueue* getComputeCommandQueue() = 0;

    // utility
    Ref<IShaderPass> createShaderPassFromUnifiedShaderPass(const UnifiedShader* unifiedShader, UnifiedShader::PassId passId, const std::string& name, DiagnosticsManager* diag);


	const std::unique_ptr<NativeRenderPassCache>& renderPassCache() const { return m_renderPassCache; }
	const std::unique_ptr<NativePipelineCache>& pipelineCache() const { return m_pipelineCache; }

protected:
	virtual void onGetCaps(GraphicsDeviceCaps* outCaps) = 0;
	virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) = 0;
	virtual Ref<ICommandList> onCreateCommandList() = 0;
	virtual Ref<IRenderPass> onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) = 0;
	virtual Ref<IPipeline> onCreatePipeline(const DevicePipelineStateDesc& state) = 0;
	virtual Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) = 0;
	virtual Ref<IVertexBuffer> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) = 0;
	virtual Ref<IIndexBuffer> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) = 0;
	virtual Ref<ITexture> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) = 0;
	virtual Ref<ITexture> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) = 0;
	virtual Ref<ITexture> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) = 0;
    virtual Ref<ITexture> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) = 0;
	virtual Ref<IDepthBuffer> onCreateDepthBuffer(uint32_t width, uint32_t height) = 0;
	virtual Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) = 0;
	virtual Ref<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) = 0;
	virtual Ref<IUniformBuffer> onCreateUniformBuffer(uint32_t size) = 0;
	virtual Ref<IDescriptorPool> onCreateDescriptorPool(IShaderPass* shaderPass) = 0;
	virtual void onSubmitCommandBuffer(ICommandList* context, ITexture* affectRendreTarget) = 0;

public:	// TODO:
	GraphicsDeviceCaps m_caps;
	std::vector<Ref<IGraphicsDeviceObject>> m_aliveObjects;
	std::unique_ptr<NativeRenderPassCache> m_renderPassCache;
	std::unique_ptr<NativePipelineCache> m_pipelineCache;
};

class ICommandList
    : public IGraphicsDeviceObject
{
public:
	// LuminoGraphis を他のフレームワークに組み込むときに、バックエンドが DX9 や OpenGL などステートマシンベースである場合に使用する
	void enterRenderState();
	void leaveRenderState();

    /////////
    void begin();
    void end();
	void beginRenderPass(IRenderPass* value);
	void endRenderPass(IRenderPass* value);
    void setBlendState(const BlendStateDesc& value);
    void setRasterizerState(const RasterizerStateDesc& value);
    void setDepthStencilState(const DepthStencilStateDesc& value);
    void setViewportRect(const RectI& value);
    void setScissorRect(const RectI& value);
    void setVertexDeclaration(IVertexDeclaration* value);
    void setVertexBuffer(int streamIndex, IVertexBuffer* value);
    void setIndexBuffer(IIndexBuffer* value);
    void setShaderPass(IShaderPass* value);
	void setDescriptor(IDescriptor* value);
    void setPrimitiveTopology(PrimitiveTopology value);

    // write only
    void* map(IGraphicsRHIBuffer* resource, uint32_t offset, uint32_t size);
    void unmap(IGraphicsRHIBuffer* resource);
    void setSubData(IGraphicsRHIBuffer* resource, size_t offset, const void* data, size_t length);
    void setSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize);
    void setSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize);

    void clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil);
    void drawPrimitive(int startVertex, int primitiveCount);
    void drawPrimitiveIndexed(int startIndex, int primitiveCount, int instanceCount, int vertexOffset);
	void drawExtension(INativeGraphicsExtension* extension);

    /////////

	virtual void wait() = 0;

    IGraphicsDevice* device() const { return m_device; }
    IRenderPass* currentRenderPass() const { return m_currentRenderPass; }

public:	// TODO:
	ICommandList();
    virtual ~ICommandList() = default;
	Result init(IGraphicsDevice* owner);

	virtual void onSaveExternalRenderState() = 0;
	virtual void onRestoreExternalRenderState() = 0;
	virtual void onBeginCommandRecoding() = 0;
	virtual void onEndCommandRecoding() = 0;
	virtual void onBeginRenderPass(IRenderPass* renderPass) = 0;
	virtual void onEndRenderPass(IRenderPass* renderPass) = 0;
	virtual void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) = 0;

	virtual void* onMapResource(IGraphicsRHIBuffer* resource, uint32_t offset, uint32_t size) = 0;
	virtual void onUnmapResource(IGraphicsRHIBuffer* resource) = 0;
	virtual void onSetSubData(IGraphicsRHIBuffer* resource, size_t offset, const void* data, size_t length) = 0;
	virtual void onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
	virtual void onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

	virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) = 0;
	virtual void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount) = 0;
	virtual void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset) = 0;
	virtual void onDrawExtension(INativeGraphicsExtension* extension) = 0;

	uint32_t stagingStateDirtyFlags() const { return m_stateDirtyFlags; }
	const GraphicsContextState& stagingState() const { return m_staging; }
	const GraphicsContextState& committedState() const { return m_committed; }

private:
    void commitStatus(GraphicsContextSubmitSource submitSource);
    void endCommit(GraphicsContextSubmitSource submitSource);

	IGraphicsDevice* m_device;
    uint32_t m_stateDirtyFlags;
    GraphicsContextState m_staging;
    GraphicsContextState m_committed;
	IRenderPass* m_currentRenderPass;
};

class ISwapChain
	: public IGraphicsDeviceObject
{
public:
	ISwapChain();

	virtual uint32_t getBackbufferCount() = 0;
    
    // 次のフレーム描画を開始できるようにデバイスに要求し、描画先となるカラーバッファのインデックスを取得する。
    virtual void acquireNextImage(int* outImageIndex) = 0;

	virtual ITexture* getRenderTarget(int imageIndex) const = 0;


	virtual Result resizeBackbuffer(uint32_t width, uint32_t height) = 0;

	virtual void present() = 0;

protected:
	virtual ~ISwapChain() = default;
};

// OpenGL の場合は、現在のコンテキストに対してただ glFlush するだけ。Compute は非対応。
class ICommandQueue
	: public IGraphicsDeviceObject
{
public:
	virtual Result submit(ICommandList* commandList) = 0;

protected:
	virtual ~ICommandQueue() = default;
};

// Note: Framebuffer も兼ねる。Vulkan では分けることで subpass を実現するが、Metal や DX12 では無いし、そこまで最適化する必要も今はない。
class IRenderPass
	: public IGraphicsDeviceObject
{
public:
	uint64_t cacheKeyHash = 0;

	//const std::array<Ref<ITexture>, MaxMultiRenderTargets>& renderTargets() const { return m_renderTargets; }

	//const Ref<IDepthBuffer>& depthBuffer() const { return m_depthBuffer; }

	// TODO: init 時に計算してよい
	int32_t getAvailableRenderTargetCount() const {
		int32_t count = static_cast<int>(m_renderTargets.size());
		for (int32_t i = count - 1; i >= 0; i--) {
			if (m_renderTargets[i]) {
				return i + 1;
			}
		}
		return 0;
	}

	bool hasDepthBuffer() const { return m_depthBuffer != nullptr; }

	bool isMultisample() const { return m_isMultisample; }

	virtual void dispose();

protected:
	virtual ~IRenderPass() = default;

protected:
	IGraphicsDevice* m_device = nullptr;
	std::array<Ref<ITexture>, MaxMultiRenderTargets> m_renderTargets;
	Ref<IDepthBuffer> m_depthBuffer;
	bool m_isMultisample = false;

	// TODO: init 用意した方がいい気がする
	friend class IGraphicsDevice;
};

class IVertexDeclaration
	: public IGraphicsDeviceObject
{
public:
	uint64_t m_hash;

	uint64_t hash() const { return m_hash; }
	static uint64_t computeHash(const VertexElement* elements, int count);

	virtual void dispose();

protected:
	IVertexDeclaration();
	virtual ~IVertexDeclaration() = default;
	bool init(const VertexElement* elements, int count);

private:
	IGraphicsDevice* m_device = nullptr;
	friend class IGraphicsDevice;
};


class IVertexBuffer
	: public IGraphicsRHIBuffer
{
public:
    virtual DeviceResourceType resourceType() const { return DeviceResourceType::VertexBuffer; }
	virtual size_t getBytesSize() = 0;
	virtual GraphicsResourceUsage usage() const = 0;
	//virtual void* map() = 0;	// write only. 初期化用. dynamic に書き換えたい場合は CommandList のデータ転送を使用すること.
	//virtual void unmap() = 0;

protected:
	IVertexBuffer();
	virtual ~IVertexBuffer() = default;
};


class IIndexBuffer
	: public IGraphicsRHIBuffer
{
public:
    virtual DeviceResourceType resourceType() const { return DeviceResourceType::IndexBuffer; }
	virtual size_t getBytesSize() = 0;
	virtual GraphicsResourceUsage usage() const = 0;
	//virtual void* map() = 0;	// write only. 初期化用. dynamic に書き換えたい場合は CommandList のデータ転送を使用すること.
	//virtual void unmap() = 0;

protected:
	IIndexBuffer();
	virtual ~IIndexBuffer() = default;
};

class IUniformBuffer
	: public IGraphicsDeviceObject
{
public:
	virtual void* map() = 0;
	virtual void unmap() = 0;

protected:
	virtual ~IUniformBuffer() = default;
};

class ITexture
	: public IGraphicsDeviceObject
{
public:
	virtual DeviceTextureType type() const = 0;

	virtual SizeI realSize() = 0;

	virtual TextureFormat getTextureFormat() const = 0;

	virtual GraphicsResourceUsage usage() const = 0;

	// データは up flow (上下反転)
	virtual RHIPtr<RHIBitmap> readData() = 0;

	bool mipmap() const { return m_mipmap; }

	// RenderTarget のみ
	virtual bool isMultisample() const = 0;

protected:
	ITexture();
	virtual ~ITexture() = default;

private:
	bool m_mipmap;

	friend class IGraphicsDevice;
};


class IDepthBuffer
	: public IGraphicsDeviceObject
{
public:

	virtual int format() const { return 0; }

	virtual bool isMultisample() const = 0;

protected:
	IDepthBuffer();
	virtual ~IDepthBuffer() = default;
};

class ISamplerState
	: public IGraphicsDeviceObject
{
public:

protected:
	ISamplerState();
	virtual ~ISamplerState() = default;
};

class IShaderPass
	: public IGraphicsDeviceObject
{
public:
	const std::vector<VertexInputAttribute>& attributes() const { return m_attributes; }
	const VertexInputAttribute* findAttribute(VertexElementUsage usage, int usageIndex) const;
	void dispose() override;

protected:
	IShaderPass();
	virtual ~IShaderPass() = default;
	bool init(const ShaderPassCreateInfo& createInfo);

private:
	IGraphicsDevice* m_device = nullptr;
	std::string m_name;
	std::vector<VertexInputAttribute> m_attributes;

	friend class IGraphicsDevice;
};

class IPipeline
	: public RefObject
{
public:
	uint64_t cacheKeyHash = 0;

	virtual void dispose();
	const IVertexDeclaration* vertexLayout() const { return m_sourceVertexLayout; }
	const IRenderPass* renderPass() const { return m_sourceRenderPass; }
	const IShaderPass* shaderPass() const { return m_sourceShaderPass; }

protected:
	virtual ~IPipeline() = default;

private:
	const IVertexDeclaration* m_sourceVertexLayout = nullptr;
	const IRenderPass* m_sourceRenderPass = nullptr;
	const IShaderPass* m_sourceShaderPass = nullptr;

	// TODO: init 用意した方がいい気がする
	friend class IGraphicsDevice;
	friend class NativePipelineCache;
};

// API ごとの Descriptor の実データ Allocate (UniformBuffer,Texture,Sampler,それらをまとめるDescriptorSetなど) 
// に必要な情報をラップするインターフェイス。VkDescriptorSetPool 相当。
//
// 単純に考えるなら CommandList ごとに超巨大なバッファをひとつ作って使えばよいのだが、
// Vulkan では VkDescriptorSetPool の作成時に必要な要素 (UBO はいくつ？Samplerはいくつ？など) を決めておかなければならない。
// そのため最もメモリ効率良く確保するには、CommandList と ShaderPass(のもつLayout) をキーとして VkDescriptorSetPool を作る必要がある。
//
// また OpenGL サポート中はそれとの整合をとるためこの Descriptor 周りを不自然にラップしていたため、メンテが難しくなる事態が発生していた。
// OpenGL を切ったので、上位レイヤーで共通化できる部分はそのようにして、少しでも下位レイヤーの複雑さを抑える。
class IDescriptorPool
	: public RefObject
{
public:
	virtual void dispose() = 0;
	virtual void reset() = 0;
	virtual IDescriptor* allocate() = 0;

protected:
	virtual ~IDescriptorPool() = default;
};

class IDescriptor
	: public RefObject
{
public:
	virtual void setData(const ShaderDescriptorTableUpdateInfo& data) = 0;
	//void setUniformBuffer(int index, const ShaderDescriptorBufferView& value) { uniforms[index] = value; }
	//void setTexture(int index, const ShaderDescriptorCombinedSampler& value) { textures[index] = value; }
	//void setSampler(int index, const ShaderDescriptorCombinedSampler& value) { samplers[index] = value; }

protected:
	virtual ~IDescriptor() = default;

	//static const int MaxElements = 32;
	//std::array<ShaderDescriptorBufferView, MaxElements> uniforms = {};
	//std::array<ShaderDescriptorCombinedSampler, MaxElements> textures = {};
	//std::array<ShaderDescriptorCombinedSampler, MaxElements> samplers = {};
};

} // namespace detail
} // namespace ln

#include "RHIBitmap.hpp"
#include "RHIObjectCache.hpp"

