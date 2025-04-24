#pragma once
#include <lumino.h>
#include <LuminoCore/Base/Result.hpp>
#include <LuminoEngine/Graphics/Common.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/Common.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/detail/UnifiedShader.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>
#include "Common.hpp"
#include "RHIObject.hpp"
#include "RHIResource.hpp"

namespace ln {
struct SizeI;
struct Color;
class INativeGraphicsInterface;
class PlatformWindow;

namespace detail {

struct GraphicsDeviceProperties {
    LNGraphicsBackend graphicsAPI;
    kokage::UnifiedShaderTriple requestedShaderTriple;
    bool imageLayoytVFlip = false;
    size_t uniformBufferOffsetAlignment;
    kokage::ShaderTarget shaderTarget = kokage::ShaderTarget::ShaderTarget_UNKNOWN;
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

struct SwapChainCreateInfo {
    PlatformWindow* window; // WebGPU で selector を使っている場合は nullptr
    SizeI backbufferSize;
    const char* webgpuCanvasSelectorOrNull;
};

struct RenderPassCreateInfo {
    const DeviceFramebufferState& buffers;
    ClearFlags clearFlags;
    const Color& clearColor;
    float clearDepth;
    uint8_t clearStencil;
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
    const kokage::DescriptorLayout* descriptorLayout;
    const std::vector<kokage::VertexInputAttribute>* attributes;
};

struct ShaderPassCreateInfo2 {
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
    const kokage::TargetBindingLayoutInfo* descriptorLayout;
    const std::vector<kokage::VertexInputAttribute>* attributes;
};

class IGraphicsHelper {
public:
    static kokage::AttributeUsage ElementUsageToAttributeUsage(VertexElementUsage value);
    static VertexElementUsage AttributeUsageToElementUsage(kokage::AttributeUsage value);
};

/**
 *
 * @note 複数の CommandQueue (非同期・並列コンピュートシェーダなど) は未対応。WebGPU が 2023/3/8 時点ではシングルキューしか仕様策定されていない。
 */
class IGraphicsDevice
    : public RefObject {
public:
    IGraphicsDevice();
    virtual ~IGraphicsDevice();

    void init();
    virtual void dispose();
    const GraphicsDeviceProperties& caps() { return m_caps; }
    void refreshCaps();

    Result<Ref<ISwapChain>> createSwapChain(const SwapChainCreateInfo& createInfo);
    Result<Ref<ICommandList>> createCommandList();
    Result<Ref<IRenderPass>> createRenderPass(const RenderPassCreateInfo& createInfo);
    Ref<IPipeline> createPipeline(const DevicePipelineStateDesc& state);
    Ref<IVertexDeclaration> createVertexDeclaration(const VertexElement* elements, int elementsCount);
    Ref<RHIResource> createVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData = nullptr);
    Ref<RHIResource> createIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData = nullptr);
    Ref<RHIResource> createTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData = nullptr);
    Ref<RHIResource> createTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData = nullptr);
    Ref<RHIResource> createRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa);
    Ref<RHIResource> createWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight);
    Ref<RHIResource> createDepthBuffer(uint32_t width, uint32_t height);
    Result<Ref<ISamplerState>> createSamplerState(const SamplerStateData& desc);
    Ref<IShaderPass> createShaderPass(
        const ShaderPassCreateInfo& createInfo,
        const ShaderPassCreateInfo2* createInfo2OrNull,
        ShaderCompilationDiag* diag);
    Ref<RHIResource> createUniformBuffer(uint32_t size);
    Ref<IDescriptorPool> createDescriptorPool(IShaderPass* shaderPass);
    void releaseObject(RHIDeviceObject* obj) {}

    void queueSubmit(ICommandList* context, RHIResource* affectRendreTarget); // 呼ぶ前に end しておくこと
    void queuePresent(ISwapChain* swapChain);

    virtual INativeGraphicsInterface* getNativeInterface() const = 0;
    //virtual ICommandQueue* getGraphicsCommandQueue() = 0;
    //virtual ICommandQueue* getComputeCommandQueue() = 0;

    // utility
    Ref<IShaderPass> createShaderPassFromUnifiedShaderPass(const kokage::UnifiedShader* unifiedShader, kokage::UnifiedShader::PassId passId, const std::string& name, DiagnosticsManager* diag);

    const std::unique_ptr<NativeRenderPassCache>& renderPassCache() const { return m_renderPassCache; }
    const std::unique_ptr<NativePipelineCache>& pipelineCache() const { return m_pipelineCache; }
    const std::unique_ptr<RHIProfiler>& profiler() const { return m_profiler; }

protected:
    virtual void onGetDeviceProperties(GraphicsDeviceProperties* outCaps) = 0;
    virtual Result<Ref<ISwapChain>> onCreateSwapChain(const SwapChainCreateInfo& createInfo) = 0;
    virtual Result<Ref<ICommandList>> onCreateCommandList() = 0;
    virtual Result<Ref<IRenderPass>> onCreateRenderPass(const RenderPassCreateInfo& createInfo) = 0;
    virtual Ref<IPipeline> onCreatePipeline(const DevicePipelineStateDesc& state) = 0;
    virtual Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) = 0;
    virtual Ref<RHIResource> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) = 0;
    virtual Ref<RHIResource> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) = 0;
    virtual Ref<RHIResource> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) = 0;
    virtual Ref<RHIResource> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) = 0;
    virtual Ref<RHIResource> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) = 0;
    virtual Ref<RHIResource> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) = 0;
    virtual Ref<RHIResource> onCreateDepthBuffer(uint32_t width, uint32_t height) = 0;
    virtual Result<Ref<ISamplerState>> onCreateSamplerState(const SamplerStateData& desc) = 0;
    virtual Ref<IShaderPass> onCreateShaderPass(
        const ShaderPassCreateInfo& createInfo,
        const ShaderPassCreateInfo2* createInfo2OrNull,
        ShaderCompilationDiag* diag) = 0;
    virtual Ref<RHIResource> onCreateUniformBuffer(uint32_t size) = 0;
    virtual Ref<IDescriptorPool> onCreateDescriptorPool(IShaderPass* shaderPass) = 0;
    virtual void onQueueSubmit(ICommandList* context, RHIResource* affectRendreTarget) = 0;
    virtual void onQueuePresent(ISwapChain* swapChain) = 0;

public: // TODO:
    GraphicsDeviceProperties m_caps;
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
    MaybeResult begin();
    MaybeResult end();
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
    void drawPrimitive(int startVertex, int primitiveCount, int instanceCount);
    void drawPrimitiveIndexed(int startIndex, int primitiveCount, int instanceCount, int vertexOffset);
    void drawExtension(INativeGraphicsExtension* extension);

    /////////

    virtual void wait() = 0;

    IRenderPass* currentRenderPass() const { return m_currentRenderPass; }

public: // TODO:
    ICommandList();
    virtual ~ICommandList();
    Result_deprecated<> init(IGraphicsDevice* owner);
    void onDestroy() override;

    virtual void onSaveExternalRenderState() = 0;
    virtual void onRestoreExternalRenderState() = 0;
    virtual MaybeResult onBeginCommandRecoding() = 0;
    virtual MaybeResult onEndCommandRecoding() = 0;
    virtual void onBeginRenderPass(IRenderPass* renderPass) = 0;
    virtual void onEndRenderPass(IRenderPass* renderPass) = 0;
    virtual void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) = 0;

    virtual void onSetSubData(RHIResource* resource, size_t offset, const void* data, size_t length) = 0;
    virtual void onSetSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
    virtual void onSetSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

    virtual void onDispatch(const GraphicsContextState& state, IPipeline* pipeline, int groupCountX, int groupCountY, int groupCountZ) = 0;
    virtual void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) = 0;
    virtual void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount, int instanceCount) = 0;
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

    virtual Result_deprecated<> resizeBackbuffer(uint32_t width, uint32_t height) = 0;

protected:
    virtual ~ISwapChain();
};

//// OpenGL の場合は、現在のコンテキストに対してただ glFlush するだけ。Compute は非対応。
//class ICommandQueue
//    : public RHIDeviceObject {
//public:
//    virtual Result_deprecated<> submit(ICommandList* commandList) = 0;
//
//protected:
//    virtual ~ICommandQueue() = default;
//};

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

    virtual RHIExtent2D viewSize() const;

    void onDestroy() override;

    void retainObjects();
    void releaseObjects();

protected:
    IRenderPass();
    virtual ~IRenderPass();

public: // TODO:
    std::array<RHIResource*, MaxMultiRenderTargets> m_renderTargets;
    RHIResource* m_depthBuffer;
    bool m_isMultisample;

    // TODO: init 用意した方がいい気がする
    friend class IGraphicsDevice;
};

class IVertexDeclaration
    : public RHIDeviceObject {
public:
    uint64_t m_hash;

    uint64_t hash() const { return m_hash; }
    static uint64_t computeHash(const VertexElement* elements, int count);

    void onDestroy() override;

protected:
    IVertexDeclaration();
    virtual ~IVertexDeclaration();
    bool init(const VertexElement* elements, int count);

private:
    friend class IGraphicsDevice;
};

class IShaderPass
    : public RHIDeviceObject {
public:
    const std::vector<kokage::VertexInputAttribute>& attributes() const { return m_attributes; }
    const kokage::VertexInputAttribute* findAttribute(VertexElementUsage usage, int usageIndex) const;
    void onDestroy() override;

    
    // CommandBuffer に対するインターフェイス
    Ref<IDescriptorPool> getDescriptorSetsPool();
    void releaseDescriptorSetsPool(IDescriptorPool* pool);


protected:
    IShaderPass();
    virtual ~IShaderPass();
    bool init(const ShaderPassCreateInfo& createInfo);

private:
    //IGraphicsDevice* m_device = nullptr;
    std::string m_name;
    std::vector<kokage::VertexInputAttribute> m_attributes;
    std::vector<Ref<IDescriptorPool>> m_descriptorSetsPools;

    friend class IGraphicsDevice;
};

class IPipeline
    : public RHIDeviceObject {
public:
    uint64_t cacheKeyHash = 0;

    void onDestroy() override;
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

} // namespace detail
} // namespace ln

#include "RHIBitmap.hpp"
#include "RHIObjectCache.hpp"
#include "RHISamplerStateBase.hpp"
#include "RHIDescriptorPoolBase.hpp"
#include "RHIDescriptorBase.hpp"
