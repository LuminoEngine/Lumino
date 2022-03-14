#pragma once
#include <LuminoCore/Base/Result.hpp>
#include <LuminoEngine/Graphics/Common.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoShaderCompiler/Common.hpp>
#include "../../../ShaderCompiler/src/UnifiedShader.hpp"
#include "RHIObject.hpp"
#include "RHIResource.hpp"

namespace ln {
struct SizeI;
struct Color;
class INativeGraphicsInterface;
class PlatformWindow;

namespace detail {
class RHIDeviceObject;
class IGraphicsDevice;
class ISwapChain;
class ICommandList;
class ICommandQueue;
class IRenderPass;
class IVertexDeclaration;
class ISamplerState;
class IShaderPass;
class IPipeline;
class IDescriptorPool;
class IDescriptor;
class NativeRenderPassCache;
class NativePipelineCache;
class RHIBitmap;
class RHIResource;
class RHIProfiler;

struct GraphicsDeviceCaps {
    GraphicsAPI graphicsAPI;
    UnifiedShaderTriple requestedShaderTriple;
    bool imageLayoytVFlip = false;
    size_t uniformBufferOffsetAlignment;
};

// obsolete
struct DevicePipelineState {
    IVertexDeclaration* vertexDeclaration = nullptr;
    BlendStateDesc blendState;
    RasterizerStateDesc rasterizerState;
    DepthStencilStateDesc depthStencilState;
    PrimitiveTopology topology = PrimitiveTopology::TriangleList;
};

struct DevicePipelineStateDesc {
    BlendStateDesc blendState;
    RasterizerStateDesc rasterizerState;
    DepthStencilStateDesc depthStencilState;
    PrimitiveTopology topology = PrimitiveTopology::TriangleList;
    IVertexDeclaration* vertexDeclaration = nullptr;
    IShaderPass* shaderPass = nullptr;
    IRenderPass* renderPass = nullptr;
};

struct DeviceFramebufferState {
    std::array<RHIResource*, MaxMultiRenderTargets> renderTargets = {};
    RHIResource* depthBuffer = nullptr;
};

struct DeviceRegionRectsState {
    RectI viewportRect;
    RectI scissorRect;
};

struct DevicePrimitiveState {
    std::array<RHIResource*, MaxVertexStreams> vertexBuffers = {};
    RHIResource* indexBuffer = nullptr;
};

struct GraphicsContextState {
    DevicePipelineState pipelineState;
    DeviceFramebufferState framebufferState;
    DeviceRegionRectsState regionRects;
    DevicePrimitiveState primitive;
    IShaderPass* shaderPass = nullptr;
    IDescriptor* descriptor = nullptr;
};

enum GraphicsContextStateDirtyFlags {
    GraphicsContextStateDirtyFlags_None = 0x0000,
    GraphicsContextStateDirtyFlags_PipelineState = 0x0001,
    GraphicsContextStateDirtyFlags_FrameBuffers = 0x0002,
    GraphicsContextStateDirtyFlags_RegionRects = 0x0004,
    GraphicsContextStateDirtyFlags_Primitives = 0x0008,
    GraphicsContextStateDirtyFlags_ShaderPass = 0x0010,
    GraphicsContextStateDirtyFlags_Descriptor = 0x0020,
    GraphicsContextStateDirtyFlags_All = 0xFFFF,
};

enum GraphicsContextSubmitSource {
    GraphicsContextSubmitSource_Clear,
    GraphicsContextSubmitSource_Draw,
    GraphicsContextSubmitSource_Dispatch,
    GraphicsContextSubmitSource_Extension,
};

struct ShaderVertexInputAttribute {
    VertexElementUsage usage;
    uint32_t index;
    uint32_t layoutLocation;
};

using ShaderVertexInputAttributeTable = std::vector<ShaderVertexInputAttribute>;

struct ShaderDescriptorBufferView {
    RHIResource* buffer;
    size_t offset;
    //size_t size;
};

struct ShaderDescriptorCombinedSampler {
    RHIResource* texture;
    ISamplerState* stamplerState;
};

struct ShaderDescriptorTableUpdateItem {
    RHIDeviceObject* object; // UniformBuffer, Texture, VertexBuffer, etc..
    ISamplerState* stamplerState;
    size_t offset; // UniformBuffer offset;
};

struct ShaderDescriptorTableUpdateInfo {
    static const int MaxElements = 32;

    // 各要素番号は DataIndex。detail::DescriptorLayout の各メンバと一致する。BindingIndex ではない点に注意。
    std::array<ShaderDescriptorTableUpdateItem, MaxElements> uniforms = {};
    std::array<ShaderDescriptorTableUpdateItem, MaxElements> resources = {};
    std::array<ShaderDescriptorTableUpdateItem, MaxElements> samplers = {};
    std::array<ShaderDescriptorTableUpdateItem, MaxElements> storages = {};
};

struct ShaderPassCreateInfo {
    const char* name;
    const byte_t* vsCode;
    size_t vsCodeLen;
    const byte_t* psCode;
    size_t psCodeLen;
    const byte_t* csCode;
    size_t csCodeLen;
    const char* vsEntryPointName;
    const char* psEntryPointName;
    const char* csEntryPointName;
    const DescriptorLayout* descriptorLayout;
    const std::vector<VertexInputAttribute>* attributes;
};

class IGraphicsHelper {
public:
    static AttributeUsage ElementUsageToAttributeUsage(VertexElementUsage value);
    static VertexElementUsage AttributeUsageToElementUsage(AttributeUsage value);
};

class IGraphicsDevice
    : public RefObject {
public:
    IGraphicsDevice();
    virtual ~IGraphicsDevice();

    void init();
    virtual void dispose();
    const GraphicsDeviceCaps& caps() { return m_caps; }
    void refreshCaps();

    Ref<ISwapChain> createSwapChain(PlatformWindow* window, const SizeI& backbufferSize);
    Ref<ICommandList> createCommandList();
    Ref<IRenderPass> createRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil);
    Ref<IPipeline> createPipeline(const DevicePipelineStateDesc& state);
    Ref<IVertexDeclaration> createVertexDeclaration(const VertexElement* elements, int elementsCount);
    Ref<RHIResource> createVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData = nullptr);
    Ref<RHIResource> createIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData = nullptr);
    Ref<RHIResource> createTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData = nullptr);
    Ref<RHIResource> createTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData = nullptr);
    Ref<RHIResource> createRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa);
    Ref<RHIResource> createWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight);
    Ref<RHIResource> createDepthBuffer(uint32_t width, uint32_t height);
    Ref<ISamplerState> createSamplerState(const SamplerStateData& desc);
    Ref<IShaderPass> createShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag);
    Ref<RHIResource> createUniformBuffer(uint32_t size);
    Ref<IDescriptorPool> createDescriptorPool(IShaderPass* shaderPass);
    void releaseObject(RHIDeviceObject* obj) {}

    void submitCommandBuffer(ICommandList* context, RHIResource* affectRendreTarget); // 呼ぶ前に end しておくこと

    virtual INativeGraphicsInterface* getNativeInterface() const = 0;
    virtual ICommandQueue* getGraphicsCommandQueue() = 0;
    virtual ICommandQueue* getComputeCommandQueue() = 0;

    // utility
    Ref<IShaderPass> createShaderPassFromUnifiedShaderPass(const UnifiedShader* unifiedShader, UnifiedShader::PassId passId, const std::string& name, DiagnosticsManager* diag);

    const std::unique_ptr<NativeRenderPassCache>& renderPassCache() const { return m_renderPassCache; }
    const std::unique_ptr<NativePipelineCache>& pipelineCache() const { return m_pipelineCache; }
    const std::unique_ptr<RHIProfiler>& profiler() const { return m_profiler; }

protected:
    virtual void onGetCaps(GraphicsDeviceCaps* outCaps) = 0;
    virtual Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) = 0;
    virtual Ref<ICommandList> onCreateCommandList() = 0;
    virtual Ref<IRenderPass> onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) = 0;
    virtual Ref<IPipeline> onCreatePipeline(const DevicePipelineStateDesc& state) = 0;
    virtual Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) = 0;
    virtual Ref<RHIResource> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) = 0;
    virtual Ref<RHIResource> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) = 0;
    virtual Ref<RHIResource> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) = 0;
    virtual Ref<RHIResource> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) = 0;
    virtual Ref<RHIResource> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) = 0;
    virtual Ref<RHIResource> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) = 0;
    virtual Ref<RHIResource> onCreateDepthBuffer(uint32_t width, uint32_t height) = 0;
    virtual Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) = 0;
    virtual Ref<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) = 0;
    virtual Ref<RHIResource> onCreateUniformBuffer(uint32_t size) = 0;
    virtual Ref<IDescriptorPool> onCreateDescriptorPool(IShaderPass* shaderPass) = 0;
    virtual void onSubmitCommandBuffer(ICommandList* context, RHIResource* affectRendreTarget) = 0;

public: // TODO:
    GraphicsDeviceCaps m_caps;
    std::unique_ptr<NativeRenderPassCache> m_renderPassCache;
    std::unique_ptr<NativePipelineCache> m_pipelineCache;

private:
    std::unique_ptr<RHIProfiler> m_profiler;

public:
    int32_t m_objectNextId; // TODO: 暫定対策。Hash が使えるとよい
};

class ICommandList
    : public RHIDeviceObject {
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
    void setVertexBuffer(int streamIndex, RHIResource* value);
    void setIndexBuffer(RHIResource* value);
    void setShaderPass(IShaderPass* value);
    void setDescriptor(IDescriptor* value);
    void setPrimitiveTopology(PrimitiveTopology value);

    // write only
    void setSubData(RHIResource* resource, size_t offset, const void* data, size_t length);
    void setSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize);
    void setSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize);

    void dispatch(int groupCountX, int groupCountY, int groupCountZ);
    void clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil);
    void drawPrimitive(int startVertex, int primitiveCount);
    void drawPrimitiveIndexed(int startIndex, int primitiveCount, int instanceCount, int vertexOffset);
    void drawExtension(INativeGraphicsExtension* extension);

    /////////

    virtual void wait() = 0;

    IRenderPass* currentRenderPass() const { return m_currentRenderPass; }

public: // TODO:
    ICommandList();
    virtual ~ICommandList();
    Result init(IGraphicsDevice* owner);
    void dispose() override;

    virtual void onSaveExternalRenderState() = 0;
    virtual void onRestoreExternalRenderState() = 0;
    virtual void onBeginCommandRecoding() = 0;
    virtual void onEndCommandRecoding() = 0;
    virtual void onBeginRenderPass(IRenderPass* renderPass) = 0;
    virtual void onEndRenderPass(IRenderPass* renderPass) = 0;
    virtual void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) = 0;

    virtual void onSetSubData(RHIResource* resource, size_t offset, const void* data, size_t length) = 0;
    virtual void onSetSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
    virtual void onSetSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

    virtual void onDispatch(const GraphicsContextState& state, IPipeline* pipeline, int groupCountX, int groupCountY, int groupCountZ) = 0;
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

    uint32_t m_stateDirtyFlags;
    GraphicsContextState m_staging;
    GraphicsContextState m_committed;
    IRenderPass* m_currentRenderPass;
    std::vector<Ref<IRenderPass>> m_renderPasses; // 描画中の delete を防ぐため参照を持っておく
    std::vector<Ref<RHIDeviceObject>> m_inflightResources;
};

class ISwapChain
    : public RHIDeviceObject {
public:
    ISwapChain();

    virtual uint32_t getBackbufferCount() = 0;

    // 次のフレーム描画を開始できるようにデバイスに要求し、描画先となるカラーバッファのインデックスを取得する。
    virtual void acquireNextImage(int* outImageIndex) = 0;

    virtual RHIResource* getRenderTarget(int imageIndex) const = 0;

    virtual Result resizeBackbuffer(uint32_t width, uint32_t height) = 0;

    virtual void present() = 0;

protected:
    virtual ~ISwapChain();
};

// OpenGL の場合は、現在のコンテキストに対してただ glFlush するだけ。Compute は非対応。
class ICommandQueue
    : public RHIDeviceObject {
public:
    virtual Result submit(ICommandList* commandList) = 0;

protected:
    virtual ~ICommandQueue() = default;
};

// Note: Framebuffer も兼ねる。Vulkan では分けることで subpass を実現するが、Metal や DX12 では無いし、そこまで最適化する必要も今はない。
// 性質上 RenderTarget と DepthBuffer を持つことになるが、派生では参照カウントをインクリメントしないように注意すること。
// RenderPass をキャッシュから削除できなくなる。
class IRenderPass
    : public RHIDeviceObject {
public:
    uint64_t cacheKeyHash = 0;

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

    bool containsRenderTarget(RHIResource* renderTarget) const {
        return std::find(m_renderTargets.begin(), m_renderTargets.end(), renderTarget) != m_renderTargets.end();
    }

    bool containsDepthBuffer(RHIResource* depthBuffer) const {
        return m_depthBuffer == depthBuffer;
    }

    bool isMultisample() const { return m_isMultisample; }

    RHIExtent2D viewSize() const;

    virtual void dispose();

    void retainObjects();
    void releaseObjects();

protected:
    virtual ~IRenderPass();

protected:
    std::array<RHIResource*, MaxMultiRenderTargets> m_renderTargets;
    RHIResource* m_depthBuffer;
    bool m_isMultisample = false;

    // TODO: init 用意した方がいい気がする
    friend class IGraphicsDevice;
};

class IVertexDeclaration
    : public RHIDeviceObject {
public:
    uint64_t m_hash;

    uint64_t hash() const { return m_hash; }
    static uint64_t computeHash(const VertexElement* elements, int count);

    virtual void dispose();

protected:
    IVertexDeclaration();
    virtual ~IVertexDeclaration();
    bool init(const VertexElement* elements, int count);

private:
    friend class IGraphicsDevice;
};

class ISamplerState
    : public RHIDeviceObject {
public:
protected:
    ISamplerState();
    virtual ~ISamplerState();
};

class IShaderPass
    : public RHIDeviceObject {
public:
    const std::vector<VertexInputAttribute>& attributes() const { return m_attributes; }
    const VertexInputAttribute* findAttribute(VertexElementUsage usage, int usageIndex) const;
    void dispose() override;

protected:
    IShaderPass();
    virtual ~IShaderPass();
    bool init(const ShaderPassCreateInfo& createInfo);

private:
    //IGraphicsDevice* m_device = nullptr;
    std::string m_name;
    std::vector<VertexInputAttribute> m_attributes;

    friend class IGraphicsDevice;
};

class IPipeline
    : public RHIDeviceObject {
public:
    uint64_t cacheKeyHash = 0;

    virtual void dispose();
    const IVertexDeclaration* vertexLayout() const { return m_sourceVertexLayout; }
    const IRenderPass* renderPass() const { return m_sourceRenderPass; }
    const IShaderPass* shaderPass() const { return m_sourceShaderPass; }

protected:
    virtual ~IPipeline();

private:
    const IVertexDeclaration* m_sourceVertexLayout = nullptr;
    const IRenderPass* m_sourceRenderPass = nullptr;
    const IShaderPass* m_sourceShaderPass = nullptr;

    // TODO: init 用意した方がいい気がする
    friend class IGraphicsDevice;
    friend class NativePipelineCache;
};

// API ごとの Descriptor の実データ。
// Allocate に必要な情報をラップ (UniformBuffer,Texture,Sampler,それらをまとめるDescriptorSetなど)するインターフェイス。VkDescriptorSetPool 相当。
//
// 単純に考えるなら CommandList ごとに超巨大なバッファをひとつ作って使えばよいのだが、
// Vulkan では VkDescriptorSetPool の作成時に必要な要素 (UBO はいくつ？Samplerはいくつ？など) を決めておかなければならない。
// そのため最もメモリ効率良く確保するには、CommandList と ShaderPass(のもつLayout) をキーとして VkDescriptorSetPool を作る必要がある。
class IDescriptorPool
    : public RHIDeviceObject {
public:
    //virtual void dispose() = 0;
    virtual void reset() = 0;
    virtual Result allocate(IDescriptor** outDescriptor) = 0;

protected:
    virtual ~IDescriptorPool();
};

class IDescriptor
    : public RefObject {
public:
    // CombinedSampler の場合、resources() へ Texture と SamplerState を同時にセットすることで、ペアを示す。
    struct Reference {
        Ref<RHIDeviceObject> object;
        Ref<ISamplerState> samplerState;
        size_t offset; // UniformBuffer offset;
    };
    using ReferenceList = std::array<Reference, ShaderDescriptorTableUpdateInfo::MaxElements>;

    void setData(const ShaderDescriptorTableUpdateInfo& data);
    const ReferenceList& buffers() const { return m_buffers; }
    const ReferenceList& resources() const { return m_resources; }
    const ReferenceList& samplers() const { return m_samplers; }
    const ReferenceList& storages() const { return m_storages; }

    const Reference& bufferSlot(LayoutSlotIndex i) const { return m_buffers[i.i]; }

protected:
    virtual ~IDescriptor();
    virtual void onUpdateData(const ShaderDescriptorTableUpdateInfo& data) = 0;
    void reset();

private:
    // CommandList 実行中の解放を防ぐため、参照を保持する
    ReferenceList m_buffers = {};
    ReferenceList m_resources = {};
    ReferenceList m_samplers = {};
    ReferenceList m_storages = {};
};

} // namespace detail
} // namespace ln

#include "RHIBitmap.hpp"
#include "RHIObjectCache.hpp"
