
#include "Internal.hpp"
#include <LuminoEngine/Base/MixHash.hpp>
#include "GraphicsDeviceContext.hpp"
#include "RHIObjectCache.hpp"
#include "RHIProfiler.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RHIDeviceObject

AttributeUsage IGraphicsHelper::ElementUsageToAttributeUsage(VertexElementUsage value) {
    static struct
    {
        VertexElementUsage v1;
        AttributeUsage v2;
    } s_conversionTable[] = {
        { VertexElementUsage::Unknown, AttributeUsage_Unknown },
        { VertexElementUsage::Position, AttributeUsage_Position },
        { VertexElementUsage::Normal, AttributeUsage_Normal },
        { VertexElementUsage::Color, AttributeUsage_Color },
        { VertexElementUsage::TexCoord, AttributeUsage_TexCoord },
        { VertexElementUsage::PointSize, AttributeUsage_Unknown },
        { VertexElementUsage::Tangent, AttributeUsage_Tangent },
        { VertexElementUsage::Binormal, AttributeUsage_Binormal },
        { VertexElementUsage::BlendIndices, AttributeUsage_BlendIndices },
        { VertexElementUsage::BlendWeight, AttributeUsage_BlendWeight },
        { VertexElementUsage::InstanceID, AttributeUsage_InstanceID },
    };
    assert(s_conversionTable[(int)value].v1 == value);
    return s_conversionTable[(int)value].v2;
}

VertexElementUsage IGraphicsHelper::AttributeUsageToElementUsage(AttributeUsage value) {
    static struct
    {
        AttributeUsage v1;
        VertexElementUsage v2;
    } s_conversionTable[] = {
        { AttributeUsage_Unknown, VertexElementUsage::Unknown },
        { AttributeUsage_Position, VertexElementUsage::Position },
        { AttributeUsage_BlendIndices, VertexElementUsage::BlendIndices },
        { AttributeUsage_BlendWeight, VertexElementUsage::BlendWeight },
        { AttributeUsage_Normal, VertexElementUsage::Normal },
        { AttributeUsage_TexCoord, VertexElementUsage::TexCoord },
        { AttributeUsage_Tangent, VertexElementUsage::Tangent },
        { AttributeUsage_Binormal, VertexElementUsage::Binormal },
        { AttributeUsage_Color, VertexElementUsage::Color },
        { AttributeUsage_InstanceID, VertexElementUsage::InstanceID },
    };
    assert(s_conversionTable[(int)value].v1 == value);
    return s_conversionTable[(int)value].v2;
}

//==============================================================================
// IGraphicsDevice

IGraphicsDevice::IGraphicsDevice()
    : m_renderPassCache(std::make_unique<NativeRenderPassCache>(this))
    , m_pipelineCache(std::make_unique<NativePipelineCache>(this))
    , m_profiler(std::make_unique<RHIProfiler>())
    , m_objectNextId(1) {
}

IGraphicsDevice::~IGraphicsDevice() {
}

void IGraphicsDevice::init() {
}

void IGraphicsDevice::dispose() {
    m_pipelineCache->clear();
    m_renderPassCache->clear();
}

void IGraphicsDevice::refreshCaps() {
    onGetCaps(&m_caps);
}

Ref<ISwapChain> IGraphicsDevice::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize) {
    Ref<ISwapChain> ptr = onCreateSwapChain(window, backbufferSize);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addSwapChain(ptr);
    }
    return ptr;
}

Ref<ICommandList> IGraphicsDevice::createCommandList() {
    Ref<ICommandList> ptr = onCreateCommandList();
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addCommandList(ptr);
    }
    return ptr;
}

Ref<IRenderPass> IGraphicsDevice::createRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) {
    Ref<IRenderPass> ptr = onCreateRenderPass(buffers, clearFlags, clearColor, clearDepth, clearStencil);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;

        // Preserve dependent object references
        for (uint32_t i = 0; i < buffers.renderTargets.size(); i++) {
            ptr->m_renderTargets[i] = buffers.renderTargets[i];
        }
        ptr->m_depthBuffer = buffers.depthBuffer;

        // 検証。MSAA 有効なら、RenderPass にアタッチされているすべての RenderTarget と DepthBuffer は
        // MSAA 有効 (同一サンプルレベル) でなければならない。
        // https://docs.microsoft.com/en-us/windows/win32/api/dxgicommon/ns-dxgicommon-dxgi_sample_desc
        //#ifdef LN_DEBUG
        int RTCount = 0;
        int multisampleRTCount = 0;
        for (int i = 0; i < MaxMultiRenderTargets; i++) {
            if (ptr->m_renderTargets[i]) {
                RTCount++;
                if (ptr->m_renderTargets[i]->isMultisample()) {
                    multisampleRTCount++;
                }
            }
        }

        if (multisampleRTCount > 0) {
            assert(multisampleRTCount == RTCount);
            if (ptr->m_depthBuffer) {
                assert(ptr->m_depthBuffer->isMultisample());
            }
            ptr->m_isMultisample = true;
        }
        //#endif
        m_profiler->addRenderPass(ptr);
    }
    return ptr;
}

Ref<IPipeline> IGraphicsDevice::createPipeline(const DevicePipelineStateDesc& state) {
    Ref<IPipeline> ptr = onCreatePipeline(state);
    if (ptr) {
        ptr->m_sourceVertexLayout = state.vertexDeclaration;
        ptr->m_sourceRenderPass = state.renderPass;
        ptr->m_sourceShaderPass = state.shaderPass;
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addPipelineState(ptr);
    }
    return ptr;
}

Ref<IVertexDeclaration> IGraphicsDevice::createVertexDeclaration(const VertexElement* elements, int elementsCount) {
    Ref<IVertexDeclaration> ptr = onCreateVertexDeclaration(elements, elementsCount);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        ptr->m_hash = IVertexDeclaration::computeHash(elements, elementsCount);
        m_profiler->addVertexLayout(ptr);
    }
    return ptr;
}

Ref<RHIResource> IGraphicsDevice::createVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) {
    Ref<RHIResource> ptr = onCreateVertexBuffer(usage, bufferSize, initialData);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addVertexBuffer(ptr);
    }
    return ptr;
}

Ref<RHIResource> IGraphicsDevice::createIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) {
    Ref<RHIResource> ptr = onCreateIndexBuffer(usage, format, indexCount, initialData);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addIndexBuffer(ptr);
    }
    return ptr;
}

Ref<RHIResource> IGraphicsDevice::createTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    Ref<RHIResource> ptr = onCreateTexture2D(usage, width, height, requestFormat, mipmap, initialData);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addTexture2D(ptr);
    }
    return ptr;
}

Ref<RHIResource> IGraphicsDevice::createTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<RHIResource> IGraphicsDevice::createRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) {
    Ref<RHIResource> ptr = onCreateRenderTarget(width, height, requestFormat, mipmap, msaa);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addRenderTarget(ptr);
    }
    return ptr;
}

Ref<RHIResource> IGraphicsDevice::createWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) {
    Ref<RHIResource> ptr = onCreateWrappedRenderTarget(nativeObject, hintWidth, hintHeight);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addRenderTarget(ptr);
    }
    return ptr;
}

Ref<RHIResource> IGraphicsDevice::createDepthBuffer(uint32_t width, uint32_t height) {
    Ref<RHIResource> ptr = onCreateDepthBuffer(width, height);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addDepthBuffer(ptr);
    }
    return ptr;
}

Ref<ISamplerState> IGraphicsDevice::createSamplerState(const SamplerStateData& desc) {
    Ref<ISamplerState> ptr = onCreateSamplerState(desc);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addSamplerState(ptr);
    }
    return ptr;
}

Ref<IShaderPass> IGraphicsDevice::createShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) {
    // Verification
    {
        if (createInfo.csCode) {
            if (LN_REQUIRE(createInfo.csCode)) return nullptr;
            if (LN_REQUIRE(createInfo.csCodeLen > 0)) return nullptr;
        }
        else {
            if (LN_REQUIRE(createInfo.vsCode)) return nullptr;
            if (LN_REQUIRE(createInfo.vsCodeLen > 0)) return nullptr;
            if (LN_REQUIRE(createInfo.psCode)) return nullptr;
            if (LN_REQUIRE(createInfo.psCodeLen > 0)) return nullptr;
        }
    }

    diag->level = ShaderCompilationResultLevel::Success;
    diag->message.clear();

    Ref<IShaderPass> ptr = onCreateShaderPass(createInfo, diag);

    if (!diag->message.empty()) {
        LN_LOG_VERBOSE(diag->message);
    }

    if (ptr) {
        ptr->m_name = createInfo.name;
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addShaderPass(ptr);
    }
    return ptr;
}

Ref<RHIResource> IGraphicsDevice::createUniformBuffer(uint32_t size) {
    Ref<RHIResource> ptr = onCreateUniformBuffer(size);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addUniformBuffer(ptr);
    }
    return ptr;
}

Ref<IDescriptorPool> IGraphicsDevice::createDescriptorPool(IShaderPass* shaderPass) {
    Ref<IDescriptorPool> ptr = onCreateDescriptorPool(shaderPass);
    if (ptr) {
        ptr->m_device = this;
        ptr->m_objectId = m_objectNextId++;
        m_profiler->addDescriptorPool(ptr);
    }
    return ptr;
}

void IGraphicsDevice::submitCommandBuffer(ICommandList* context, RHIResource* affectRendreTarget) {
    onSubmitCommandBuffer(context, affectRendreTarget);
}

Ref<IShaderPass> IGraphicsDevice::createShaderPassFromUnifiedShaderPass(const UnifiedShader* unifiedShader, UnifiedShader::PassId passId, const std::string& name, DiagnosticsManager* diag) {
    LN_DCHECK(unifiedShader);
    LN_DCHECK(diag);
    auto& triple = caps().requestedShaderTriple;

    detail::CodeContainerId vscodeId = unifiedShader->vertexShader(passId);
    detail::CodeContainerId pscodeId = unifiedShader->pixelShader(passId);
    detail::CodeContainerId cscodeId = unifiedShader->computeShader(passId);

    const char* vsEntryPointName = nullptr;
    const char* psEntryPointName = nullptr;
    const char* csEntryPointName = nullptr;
    const detail::USCodeInfo* vscode = nullptr;
    const detail::USCodeInfo* pscode = nullptr;
    const detail::USCodeInfo* cscode = nullptr;
    if (vscodeId) {
        auto* contaier = unifiedShader->codeContainer(vscodeId);
        vsEntryPointName = contaier->entryPointName.c_str();
        vscode = contaier->findCode(triple);
    }
    if (pscodeId) {
        auto* contaier = unifiedShader->codeContainer(pscodeId);
        psEntryPointName = contaier->entryPointName.c_str();
        pscode = contaier->findCode(triple);
    }
    if (cscodeId) {
        auto* contaier = unifiedShader->codeContainer(cscodeId);
        csEntryPointName = contaier->entryPointName.c_str();
        cscode = contaier->findCode(triple);
    }

    detail::ShaderPassCreateInfo createInfo = {
        name.c_str(),
        (vscode) ? vscode->code.data() : nullptr,
        (vscode) ? vscode->code.size() : 0,
        (pscode) ? pscode->code.data() : nullptr,
        (pscode) ? pscode->code.size() : 0,
        (cscode) ? cscode->code.data() : nullptr,
        (cscode) ? cscode->code.size() : 0,
        vsEntryPointName,
        psEntryPointName,
        csEntryPointName,
        &unifiedShader->descriptorLayout(passId),
        &unifiedShader->attributes(passId),
    };

    ShaderCompilationDiag sdiag;
    Ref<detail::IShaderPass> pass = createShaderPass(createInfo, &sdiag);

    if (sdiag.level == ShaderCompilationResultLevel::Error) {
        diag->reportError(String::fromStdString(sdiag.message));
    }
    else if (sdiag.level == ShaderCompilationResultLevel::Warning) {
        diag->reportWarning(String::fromStdString(sdiag.message));
    }

    return pass;
}

//==============================================================================
// ICommandList

ICommandList::ICommandList()
    : m_stateDirtyFlags(GraphicsContextStateDirtyFlags_All)
    , m_staging()
    , m_committed()
    , m_currentRenderPass(nullptr) {
}

ICommandList::~ICommandList() {
    if (IGraphicsDevice* d = device()) {
        d->profiler()->removeCommandList(this);
    }
}

Result ICommandList::init(IGraphicsDevice* owner) {
    return ok();
}

void ICommandList::dispose() {
    for (IRenderPass* renderPass : m_renderPasses) {
        renderPass->releaseObjects();
    }
    m_renderPasses.clear();
    RHIDeviceObject::dispose();
}

void ICommandList::enterRenderState() {
    onSaveExternalRenderState();
}

void ICommandList::leaveRenderState() {
    onRestoreExternalRenderState();
}

void ICommandList::begin() {
    for (IRenderPass* renderPass : m_renderPasses) {
        renderPass->releaseObjects();
    }
    m_renderPasses.clear();
    m_inflightResources.clear();

    m_stateDirtyFlags = GraphicsContextStateDirtyFlags_All;
    onBeginCommandRecoding();
}

void ICommandList::end() {
    onEndCommandRecoding();
}

void ICommandList::beginRenderPass(IRenderPass* value) {
    if (LN_REQUIRE(!m_currentRenderPass)) return;
    m_currentRenderPass = value;
    m_renderPasses.push_back(value);
    value->retainObjects();
    onBeginRenderPass(value);
}

void ICommandList::endRenderPass(IRenderPass* value) {
    if (LN_REQUIRE(m_currentRenderPass && m_currentRenderPass == value)) return;
    onEndRenderPass(value);
    m_currentRenderPass = nullptr;
}

void ICommandList::setBlendState(const BlendStateDesc& value) {
    m_staging.pipelineState.blendState = value;
    m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
}

void ICommandList::setRasterizerState(const RasterizerStateDesc& value) {
    m_staging.pipelineState.rasterizerState = value;
    m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
}

void ICommandList::setDepthStencilState(const DepthStencilStateDesc& value) {
    m_staging.pipelineState.depthStencilState = value;
    m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
}

void ICommandList::setViewportRect(const RectI& value) {
    if (m_staging.regionRects.viewportRect != value) {
        m_staging.regionRects.viewportRect = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_RegionRects;
    }
}

void ICommandList::setScissorRect(const RectI& value) {
    if (m_staging.regionRects.scissorRect != value) {
        m_staging.regionRects.scissorRect = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_RegionRects;
    }
}

void ICommandList::setVertexDeclaration(IVertexDeclaration* value) {
    if (m_staging.pipelineState.vertexDeclaration != value) {
        m_staging.pipelineState.vertexDeclaration = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
    }
}

void ICommandList::setVertexBuffer(int streamIndex, RHIResource* value) {
    if (m_staging.primitive.vertexBuffers[streamIndex] != value) {
        m_staging.primitive.vertexBuffers[streamIndex] = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_Primitives;
    }
}

void ICommandList::setIndexBuffer(RHIResource* value) {
    if (m_staging.primitive.indexBuffer != value) {
        m_staging.primitive.indexBuffer = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_Primitives;
    }
}

void ICommandList::setShaderPass(IShaderPass* value) {
    if (m_staging.shaderPass != value) {
        m_staging.shaderPass = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_ShaderPass;
    }
}

void ICommandList::setDescriptor(IDescriptor* value) {
    if (m_staging.descriptor != value) {
        m_staging.descriptor = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_Descriptor;
    }
}

void ICommandList::setPrimitiveTopology(PrimitiveTopology value) {
    m_staging.pipelineState.topology = value;
}

void ICommandList::setSubData(RHIResource* resource, size_t offset, const void* data, size_t length) {
    onSetSubData(resource, offset, data, length);
}

void ICommandList::setSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize) {
    onSetSubData2D(resource, x, y, width, height, data, dataSize);
}

void ICommandList::setSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) {
    onSetSubData3D(resource, x, y, z, width, height, depth, data, dataSize);
}

void ICommandList::dispatch(int groupCountX, int groupCountY, int groupCountZ) {
    //commitStatus(GraphicsContextSubmitSource_Dispatch);
    DevicePipelineStateDesc state;
    state.shaderPass = m_staging.shaderPass;
    IPipeline* pipeline = device()->pipelineCache()->findOrCreate(state);
    onDispatch(m_staging, pipeline, groupCountX, groupCountY, groupCountZ);
    //endCommit(GraphicsContextSubmitSource_Dispatch);
}

void ICommandList::clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) {
    commitStatus(GraphicsContextSubmitSource_Clear);
    onClearBuffers(flags, color, z, stencil);
    endCommit(GraphicsContextSubmitSource_Clear);
}

void ICommandList::drawPrimitive(int startVertex, int primitiveCount) {
    commitStatus(GraphicsContextSubmitSource_Draw);
    onDrawPrimitive(m_staging.pipelineState.topology, startVertex, primitiveCount);
    endCommit(GraphicsContextSubmitSource_Draw);
}

void ICommandList::drawPrimitiveIndexed(int startIndex, int primitiveCount, int instanceCount, int vertexOffset) {
    commitStatus(GraphicsContextSubmitSource_Draw);
    onDrawPrimitiveIndexed(m_staging.pipelineState.topology, startIndex, primitiveCount, instanceCount, vertexOffset);
    endCommit(GraphicsContextSubmitSource_Draw);
}

void ICommandList::drawExtension(INativeGraphicsExtension* extension) {
    commitStatus(GraphicsContextSubmitSource_Extension);
    onDrawExtension(extension);
    endCommit(GraphicsContextSubmitSource_Extension);
}

void ICommandList::commitStatus(GraphicsContextSubmitSource submitSource) {
    //if (LN_REQUIRE(m_staging.framebufferState.renderTargets[0])) return;
    //if (LN_REQUIRE(m_staging.pipelineState.vertexDeclaration)) return;

    //if (submitSource == GraphicsContextSubmitSource_Dispatch) {
    //	IPipeline* pipeline = device()->pipelineCache()->findOrCreate(state);

    //}
    //else
    if (m_staging.shaderPass && m_staging.pipelineState.vertexDeclaration && m_currentRenderPass // extention 描画時
    ) {

        // TODO: modified check

        DevicePipelineStateDesc state;
        state.blendState = m_staging.pipelineState.blendState;
        state.rasterizerState = m_staging.pipelineState.rasterizerState;
        state.depthStencilState = m_staging.pipelineState.depthStencilState;
        state.topology = m_staging.pipelineState.topology;
        state.vertexDeclaration = m_staging.pipelineState.vertexDeclaration;
        state.shaderPass = m_staging.shaderPass;
        state.renderPass = m_currentRenderPass;
        IPipeline* pipeline = device()->pipelineCache()->findOrCreate(state);

        // CommandList 実行中のリソース削除を防ぐため、参照カウントを増やしておく
        for (const auto& v : m_staging.primitive.vertexBuffers) {
            m_inflightResources.push_back(v);
        }
        m_inflightResources.push_back(m_staging.primitive.indexBuffer);
        m_inflightResources.push_back(m_staging.pipelineState.vertexDeclaration);
        m_inflightResources.push_back(pipeline);

        onSubmitStatus(m_staging, m_stateDirtyFlags, submitSource, pipeline);
    }
    else {
        // clear や、extension 呼び出し。pipelineState を独自で作らない。
        onSubmitStatus(m_staging, m_stateDirtyFlags, submitSource, nullptr);
    }
}

void ICommandList::endCommit(GraphicsContextSubmitSource submitSource) {
    if (submitSource == GraphicsContextSubmitSource_Extension) {
        // 次回の描画時にすべて設定しなおす
        m_stateDirtyFlags = GraphicsContextStateDirtyFlags_All;
    }
    else {
        m_committed = m_staging;
        m_stateDirtyFlags = GraphicsContextStateDirtyFlags_None;
    }
}

//==============================================================================
// ISwapChain

ISwapChain::ISwapChain() {
    LN_LOG_VERBOSE("ISwapChain [0x{:x}] constructed.", (intptr_t)this);
}

ISwapChain::~ISwapChain() {
    if (IGraphicsDevice* d = device()) {
        d->profiler()->removeSwapChain(this);
    }
}

//==============================================================================
// IRenderPass

IRenderPass::IRenderPass()
    : m_renderTargets{}
    , m_depthBuffer(nullptr)
    , m_isMultisample(false) {
}

IRenderPass::~IRenderPass() {
    if (IGraphicsDevice* d = device()) {
        d->profiler()->removeRenderPass(this);
    }
}

RHIExtent2D IRenderPass::viewSize() const {
    if (m_renderTargets[0]) {
        const RHIExtent3D& size = m_renderTargets[0]->extentSize();
        return RHIExtent2D{ size.width, size.height };
    }
    else {
        return RHIExtent2D{ 0, 0 };
    }
}

void IRenderPass::dispose() {
    if (IGraphicsDevice* d = device()) {
        d->pipelineCache()->invalidate(this);
    }

    RHIDeviceObject::dispose();
}

void IRenderPass::retainObjects() {
    for (RHIResource* renderTarget : m_renderTargets) {
        if (renderTarget) {
            RefObjectHelper::retain(renderTarget);
        }
    }
    if (m_depthBuffer) {
        RefObjectHelper::retain(m_depthBuffer);
    }
}

void IRenderPass::releaseObjects() {
    for (RHIResource* renderTarget : m_renderTargets) {
        if (renderTarget) {
            RefObjectHelper::release(renderTarget);
        }
    }
    if (m_depthBuffer) {
        RefObjectHelper::release(m_depthBuffer);
    }
}

//==============================================================================
// IVertexDeclaration

IVertexDeclaration::IVertexDeclaration() {
    LN_LOG_VERBOSE("IVertexDeclaration [0x{:x}] constructed.", (intptr_t)this);
}

IVertexDeclaration::~IVertexDeclaration() {
    if (IGraphicsDevice* d = device()) {
        d->profiler()->removeVertexLayout(this);
    }
}

bool IVertexDeclaration::init(const VertexElement* elements, int count) {
    //m_elements.resize(count);
    //for (int i = 0; i < count; i++) m_elements[i] = elements[i];
    return true;
}

void IVertexDeclaration::dispose() {
    if (IGraphicsDevice* d = device()) {
        d->pipelineCache()->invalidate(this);
    }

    RHIDeviceObject::dispose();
}

uint64_t IVertexDeclaration::computeHash(const VertexElement* elements, int count) {
    MixHash hash;
    hash.add(count);
    for (int i = 0; i < count; i++) {
        auto& e = elements[i];
        hash.add(e.StreamIndex);
        hash.add(e.Type);
        hash.add(e.Usage);
        hash.add(e.UsageIndex);
    }
    return hash.value();
}

//==============================================================================
// ISamplerState

ISamplerState::ISamplerState() {
    LN_LOG_VERBOSE("ISamplerState [0x{:x}] constructed.", (intptr_t)this);
}

ISamplerState::~ISamplerState() {
    if (IGraphicsDevice* d = device()) {
        d->profiler()->removeSamplerState(this);
    }
}

//==============================================================================
// IShaderPass

IShaderPass::IShaderPass() {
    LN_LOG_VERBOSE("IShaderPass [0x{:x}] constructed.", (intptr_t)this);
}

IShaderPass::~IShaderPass() {
    if (IGraphicsDevice* d = device()) {
        d->profiler()->removeShaderPass(this);
    }
}

void IShaderPass::dispose() {
    if (IGraphicsDevice* d = device()) {
        d->pipelineCache()->invalidate(this);
    }
    RHIDeviceObject::dispose();
}

bool IShaderPass::init(const ShaderPassCreateInfo& createInfo) {
    m_attributes = *(createInfo.attributes);
    return true;
}

const VertexInputAttribute* IShaderPass::findAttribute(VertexElementUsage usage, int usageIndex) const {
    // TODO: これ線形探索じゃなくて、map 作った方がいいかも。
    // usage の種類は固定だし、usageIndex も最大 16 あれば十分だし、byte 型 8x16 くらいの Matrix で足りる。
    auto au = IGraphicsHelper::ElementUsageToAttributeUsage(usage);
    for (auto& a : m_attributes) {
        if (a.usage == au && a.index == usageIndex) {
            return &a;
        }
    }
    return nullptr;
}

////==============================================================================
//// IShaderPass
//
//IShaderUniformBuffer::IShaderUniformBuffer()
//{
//	LN_LOG_VERBOSE << "IShaderUniformBuffer [0x" << this << "] constructed.";
//}
//
////==============================================================================
//// IShaderUniform
//
//IShaderUniform::IShaderUniform()
//{
//	LN_LOG_VERBOSE << "IShaderUniform [0x" << this << "] constructed.";
//}

//==============================================================================
// IShaderSamplerBuffer

//IShaderSamplerBuffer::IShaderSamplerBuffer()
//{
//	LN_LOG_VERBOSE << "IShaderSamplerBuffer [0x" << this << "] constructed.";
//}

//==============================================================================
// IPipeline

IPipeline::~IPipeline() {
    if (IGraphicsDevice* d = device()) {
        d->profiler()->removePipelineState(this);
    }
}

void IPipeline::dispose() {
    RHIDeviceObject::dispose();
}

//==============================================================================
// IDescriptorPool

IDescriptorPool::~IDescriptorPool() {
    if (IGraphicsDevice* d = device()) {
        d->profiler()->removeDescriptorPool(this);
    }
}

//==============================================================================
// IDescriptor

IDescriptor::~IDescriptor() = default;

void IDescriptor::setData(const ShaderDescriptorTableUpdateInfo& data) {
    for (int32_t i = 0; i < ShaderDescriptorTableUpdateInfo::MaxElements; i++) {
        if (LN_ENSURE(!data.uniforms[i].stamplerState)) return;
        m_buffers[i].object = data.uniforms[i].object;
        m_buffers[i].offset = data.uniforms[i].offset;

        m_resources[i].object = data.resources[i].object;
        m_resources[i].samplerState = data.resources[i].stamplerState;

        if (LN_ENSURE(!data.samplers[i].object)) return;
        m_samplers[i].samplerState = data.samplers[i].stamplerState;

        if (LN_ENSURE(!data.storages[i].stamplerState)) return;
        m_storages[i].object = data.storages[i].object;
    }

    onUpdateData(data);
}

void IDescriptor::reset() {
    for (int32_t i = 0; i < ShaderDescriptorTableUpdateInfo::MaxElements; i++) {
        m_buffers[i].object = nullptr;
        m_resources[i].object = nullptr;
        m_resources[i].samplerState = nullptr;
        m_samplers[i].samplerState = nullptr;
        m_storages[i].object = nullptr;
    }
}

} // namespace detail
} // namespace ln
