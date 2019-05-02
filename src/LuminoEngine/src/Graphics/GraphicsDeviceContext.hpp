#pragma once
#include <LuminoCore/Base/Result.hpp>
#include <LuminoEngine/Graphics/Common.hpp>
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Shader/Common.hpp>
#include "../Shader/UnifiedShader.hpp"

namespace ln {
struct SizeI;
struct Color;

namespace detail {
class PlatformWindow;
class IGraphicsContext;
class ISwapChain;
class IVertexDeclaration;
class IVertexBuffer;
class IIndexBuffer;
class ITexture;
class IDepthBuffer;
class ISamplerState;
class IShaderPass;
class IShaderUniformBuffer;
class IShaderUniform;
class IShaderSamplerBuffer;

//static const int MaxRenderTargets = 4;
//static const int MaxVertexStreams = 4;

enum class DeviceResourceType
{
    VertexBuffer,
    IndexBuffer,
};

enum class DeviceTextureType
{
	Texture2D,
	Texture3D,
	RenderTarget,
};

struct GraphicsDeviceCaps
{
	UnifiedShaderTriple requestedShaderTriple;
};

struct DevicePipelineState
{
	BlendStateDesc blendState;
	RasterizerStateDesc rasterizerState;
	DepthStencilStateDesc depthStencilState;
	IVertexDeclaration* vertexDeclaration = nullptr;
	PrimitiveTopology topology = PrimitiveTopology::TriangleList;
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
};

enum GraphicsContextStateDirtyFlags
{
    GraphicsContextStateDirtyFlags_None = 0x0000,
    GraphicsContextStateDirtyFlags_PipelineState = 0x0001,
    GraphicsContextStateDirtyFlags_FrameBuffers = 0x0002,
    GraphicsContextStateDirtyFlags_RegionRects = 0x0004,
    GraphicsContextStateDirtyFlags_Primitives = 0x0008,
    GraphicsContextStateDirtyFlags_ShaderPass = 0x0010,
    GraphicsContextStateDirtyFlags_All = 0xFFFF,
};

enum GraphicsContextSubmitSource
{
    GraphicsContextSubmitSource_Clear,
    GraphicsContextSubmitSource_Draw,
};

struct ShaderVertexInputAttribute
{
	VertexElementUsage usage;
	uint32_t index;
	uint32_t layoutLocation;
};

using ShaderVertexInputAttributeTable = std::vector<ShaderVertexInputAttribute>;

struct ShaderPassCreateInfo
{
    const byte_t* vsCode;
    size_t vsCodeLen;
    const byte_t* psCode;
    size_t psCodeLen;
    const char* vsEntryPointName;
    const char* psEntryPointName;
    //const ShaderVertexInputAttributeTable* attributeTable;
    const DescriptorLayout* descriptorLayout;
};

class IGraphicsDeviceObject
    : public RefObject
{
public:

protected:
    IGraphicsDeviceObject();
    virtual ~IGraphicsDeviceObject();
    //virtual void finalize();
public:
    virtual void dispose();	// (複数回の呼び出しに備えること)

private:
    bool m_disposed;
	//friend class IGraphicsDevice;
};

class IGraphicsResource
    : public IGraphicsDeviceObject
{
public:
    virtual DeviceResourceType resourceType() const = 0;

protected:
    virtual ~IGraphicsResource();
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
	void enterMainThread();
	void leaveMainThread();
	void enterRenderState();
	void leaveRenderState();

	Ref<ISwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize);
	Ref<IVertexDeclaration> createVertexDeclaration(const VertexElement* elements, int elementsCount);
	Ref<IVertexBuffer> createVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData = nullptr);
	Ref<IIndexBuffer> createIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData = nullptr);
	Ref<ITexture> createTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData = nullptr);
	Ref<ITexture> createTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData = nullptr);
	Ref<ITexture> createRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap);
    Ref<ITexture> createWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight);
	Ref<IDepthBuffer> createDepthBuffer(uint32_t width, uint32_t height);
	Ref<ISamplerState> createSamplerState(const SamplerStateData& desc);
	Ref<IShaderPass> createShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag);

    virtual IGraphicsContext* getGraphicsContext() const = 0;


    // utility
    Ref<IShaderPass> createShaderPassFromUnifiedShaderPass(const UnifiedShader* unifiedShader, UnifiedShader::PassId passId, DiagnosticsManager* diag);

protected:
	virtual void onGetCaps(GraphicsDeviceCaps* outCaps) = 0;
	virtual void onEnterMainThread() = 0;
	virtual void onLeaveMainThread() = 0;
	virtual void onSaveExternalRenderState() = 0;
	virtual void onRestoreExternalRenderState() = 0;
	virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) = 0;
	virtual Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) = 0;
	virtual Ref<IVertexBuffer> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) = 0;
	virtual Ref<IIndexBuffer> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) = 0;
	virtual Ref<ITexture> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) = 0;
	virtual Ref<ITexture> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) = 0;
	virtual Ref<ITexture> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap) = 0;
    virtual Ref<ITexture> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) = 0;
	virtual Ref<IDepthBuffer> onCreateDepthBuffer(uint32_t width, uint32_t height) = 0;
	virtual Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) = 0;
	virtual Ref<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) = 0;

/////////
	//virtual void onBeginCommandRecoding() = 0;
	//virtual void onEndCommandRecoding() = 0;
	//virtual void onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState) = 0;
	//virtual void onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer) = 0;
	//virtual void onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize) = 0;
	//virtual void onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer) = 0;
	//virtual void onUpdateShaderPass(IShaderPass* newPass) = 0;
 //   virtual void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource) = 0;

 //   virtual void* onMapResource(IGraphicsResource* resource) = 0;
 //   virtual void onUnmapResource(IGraphicsResource* resource) = 0;
 //   virtual void onSetSubData(IGraphicsResource* resource, size_t offset, const void* data, size_t length) = 0;
 //   virtual void onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
 //   virtual void onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

	//virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) = 0;
	//virtual void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount) = 0;
	//virtual void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount) = 0;
 //   virtual void onFlushCommandBuffer(ITexture* affectRendreTarget) = 0;

	//virtual void onPresent(ISwapChain* swapChain) = 0;
/////////

public:	// TODO:
	void collectGarbageObjects();

	IGraphicsContext* m_graphicsContext;
	GraphicsDeviceCaps m_caps;
	std::vector<Ref<IGraphicsDeviceObject>> m_aliveObjects;
};

class IGraphicsContext
    : public RefObject
{
public:

    /////////
    void begin();
    void end();
    void setBlendState(const BlendStateDesc& value);
    void setRasterizerState(const RasterizerStateDesc& value);
    void setDepthStencilState(const DepthStencilStateDesc& value);
    void setColorBuffer(int index, ITexture* value);
    void setDepthBuffer(IDepthBuffer* value);
    void setViewportRect(const RectI& value);
    void setScissorRect(const RectI& value);
    void setVertexDeclaration(IVertexDeclaration* value);
    void setVertexBuffer(int streamIndex, IVertexBuffer* value);
    void setIndexBuffer(IIndexBuffer* value);
    void setShaderPass(IShaderPass* value);
    void setPrimitiveTopology(PrimitiveTopology value);

	IVertexDeclaration* vertexDeclaration() const { return m_staging.pipelineState.vertexDeclaration; }
	IVertexBuffer* vertexBuffer(int streamIndex) const { return m_staging.primitive.vertexBuffers[streamIndex]; }
	IIndexBuffer* indexBuffer() const { return m_staging.primitive.indexBuffer; }
	PrimitiveTopology primitiveTopology() const { return m_staging.pipelineState.topology; }

    // write only
    void* map(IGraphicsResource* resource);
    void unmap(IGraphicsResource* resource);
    void setSubData(IGraphicsResource* resource, size_t offset, const void* data, size_t length);
    void setSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize);
    void setSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize);

    void clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil);
    void drawPrimitive(int startVertex, int primitiveCount);
    void drawPrimitiveIndexed(int startIndex, int primitiveCount);
    void flushCommandBuffer(ITexture* affectRendreTarget);  // 呼ぶ前に end しておくこと

    void present(ISwapChain* swapChain);

    /////////

    IGraphicsDevice* device() const { return m_device; }

public:	// TODO:
	IGraphicsContext();
    virtual ~IGraphicsContext() = default;
	Result init(IGraphicsDevice* owner);

	virtual void onBeginCommandRecoding() = 0;
	virtual void onEndCommandRecoding() = 0;
	virtual void onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState) = 0;
	virtual void onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer) = 0;
	virtual void onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize) = 0;
	virtual void onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer) = 0;
	virtual void onUpdateShaderPass(IShaderPass* newPass) = 0;
	virtual void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource) = 0;

	virtual void* onMapResource(IGraphicsResource* resource) = 0;
	virtual void onUnmapResource(IGraphicsResource* resource) = 0;
	virtual void onSetSubData(IGraphicsResource* resource, size_t offset, const void* data, size_t length) = 0;
	virtual void onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
	virtual void onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

	virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) = 0;
	virtual void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount) = 0;
	virtual void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount) = 0;
	virtual void onFlushCommandBuffer(ITexture* affectRendreTarget) = 0;

	virtual void onPresent(ISwapChain* swapChain) = 0;

	uint32_t stagingStateDirtyFlags() const { return m_stateDirtyFlags; }
	const GraphicsContextState& stagingState() const { return m_staging; }
	const GraphicsContextState& committedState() const { return m_committed; }

private:
    void commitStatus(GraphicsContextSubmitSource submitSource);
    void endCommit();

	IGraphicsDevice* m_device;
    uint32_t m_stateDirtyFlags;
    GraphicsContextState m_staging;
    GraphicsContextState m_committed;
};

class ISwapChain
	: public IGraphicsDeviceObject
{
public:
	ISwapChain();
    
    // 次のフレーム描画を開始できるようにデバイスに要求し、描画先となるカラーバッファのインデックスを取得する。
    virtual void acquireNextImage(int* outImageIndex) = 0;

	virtual ITexture* getRenderTarget(int imageIndex) const = 0;


	virtual Result resizeBackbuffer(uint32_t width, uint32_t height) = 0;

protected:
	virtual ~ISwapChain() = default;
};


class IVertexDeclaration
	: public IGraphicsDeviceObject
{
protected:
	IVertexDeclaration();
	virtual ~IVertexDeclaration() = default;
};


class IVertexBuffer
	: public IGraphicsResource
{
public:
    virtual DeviceResourceType resourceType() const { return DeviceResourceType::VertexBuffer; }
	virtual size_t getBytesSize() = 0;
	virtual GraphicsResourceUsage usage() const = 0;
	//virtual void setSubData(size_t offset, const void* data, size_t length) = 0;

protected:
	IVertexBuffer();
	virtual ~IVertexBuffer() = default;
};


class IIndexBuffer
	: public IGraphicsResource
{
public:
    virtual DeviceResourceType resourceType() const { return DeviceResourceType::IndexBuffer; }
	virtual size_t getBytesSize() = 0;
	virtual GraphicsResourceUsage usage() const = 0;
	//virtual void setSubData(size_t offset, const void* data, size_t length) = 0;

protected:
	IIndexBuffer();
	virtual ~IIndexBuffer() = default;
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
	virtual void readData(void* outData) = 0;


	//void Clear(const Color& color);

	//void setSubData(const PointI& point, RawBitmap* bitmap);



	/// テクスチャの種類の取得
	//virtual TextureType getTextureType() const = 0;

	/// サーフェイスフォーマットの取得
	//virtual TextureFormat getTextureFormat() const = 0;


	///// 実際のサイズの取得 (デバイス依存により2のべき乗倍に拡張されたサイズ)
	//virtual const SizeI& getRealSize() const = 0;

	///// サンプラステートの設定
	////virtual void setSamplerState(const SamplerState& state) = 0;

	// データ転送 (TODO:部分更新は未実装…)
	// data に渡されるイメージデータは上下が反転している状態。
	//virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
	//virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

	//virtual void setSubData3D(const BoxI& box, const void* data, size_t dataBytes) = 0;

	//// (得られるデータは上下反転)
	//virtual void getData(const RectI& rect, void* outData) = 0;

	// 得られるデータは上下反転していない。レイアウトは RGBA
	//virtual void readData(void* outData) {}


	///// ロック (バッファは上下反転)
	//virtual RawBitmap* lock() = 0;

	///// アンロック
	//virtual void unlock() = 0;

	bool mipmap() const { return m_mipmap; }

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
	//virtual int getUniformCount() const = 0;
	//virtual IShaderUniform* getUniform(int index) const = 0;
	//virtual void setUniformValue(int index, const void* data, size_t size) = 0;

	virtual int getUniformBufferCount() const = 0;
	virtual IShaderUniformBuffer* getUniformBuffer(int index) const = 0;

	virtual IShaderSamplerBuffer* samplerBuffer() const = 0;

protected:
	IShaderPass();
	virtual ~IShaderPass() = default;
};

class IShaderUniformBuffer
	: public IGraphicsDeviceObject
{
public:
	virtual const std::string& name() const = 0;
	virtual int getUniformCount() const = 0;
	virtual IShaderUniform* getUniform(int index) const = 0;
	virtual size_t bufferSize() const = 0;
	virtual void setData(const void* data, size_t size) = 0;

protected:
	IShaderUniformBuffer();
	virtual ~IShaderUniformBuffer() = default;
};

class IShaderUniform
	: public IGraphicsDeviceObject
{
public:
	virtual const ShaderUniformTypeDesc& desc() const = 0;
	virtual const std::string& name() const = 0;

protected:
	IShaderUniform();
	virtual ~IShaderUniform() = default;
};

class IShaderSamplerBuffer
	: public IGraphicsDeviceObject
{
public:
	virtual int registerCount() const = 0;
	virtual const std::string& getTextureRegisterName(int registerIndex) const = 0;
	//virtual const std::string& getSamplerRegisterName(int registerIndex) const = 0;
	virtual void setTexture(int registerIndex, ITexture* texture) = 0;
	virtual void setSamplerState(int registerIndex, ISamplerState* state) = 0;

protected:
	IShaderSamplerBuffer();
	virtual ~IShaderSamplerBuffer() = default;
};

} // namespace detail
} // namespace ln

