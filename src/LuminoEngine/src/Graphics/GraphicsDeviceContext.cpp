
#include "Internal.hpp"
#include "MixHash.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {

//=============================================================================
// IGraphicsDeviceObject

IGraphicsDeviceObject::IGraphicsDeviceObject()
    : m_disposed(false)
{
}

IGraphicsDeviceObject::~IGraphicsDeviceObject()
{
    if (!m_disposed) {
        LN_LOG_ERROR << "object [0x" << this << "] is not disposed";
    }
}

//void IGraphicsDeviceObject::finalize()
//{
//    dispose();
//}

void IGraphicsDeviceObject::dispose()
{
    m_disposed = true;
}

//=============================================================================
// IGraphicsResource

IGraphicsResource::~IGraphicsResource()
{
}

//=============================================================================
// IGraphicsDevice

IGraphicsDevice::IGraphicsDevice()
	: m_graphicsContext(nullptr)
	, m_renderPassCache(std::make_unique<NativeRenderPassCache>(this))
	, m_pipelineCache(std::make_unique<NativePipelineCache>(this))
{
}

void IGraphicsDevice::init()
{
}

void IGraphicsDevice::dispose()
{
	for (auto& obj : m_aliveObjects) {
		obj->dispose();
	}
	m_aliveObjects.clear();
}

void IGraphicsDevice::refreshCaps()
{
	m_graphicsContext = getGraphicsContext();
	onGetCaps(&m_caps);
}

Ref<ISwapChain> IGraphicsDevice::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	Ref<ISwapChain> ptr = onCreateSwapChain(window, backbufferSize);
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<IRenderPass> IGraphicsDevice::createRenderPass(ITexture** renderTargets, uint32_t renderTargetCount, IDepthBuffer* depthBuffer, ClearFlags clearFlags, const Color& clearColor, float clearZ, uint8_t clearStencil)
{
	Ref<IRenderPass> ptr = onCreateRenderPass(renderTargets, renderTargetCount, depthBuffer, clearFlags, clearColor, clearZ, clearStencil);
	if (ptr) {
		ptr->m_device = this;
		m_aliveObjects.push_back(ptr);

		// Preserve dependent object references
		for (uint32_t i = 0; i < renderTargetCount; i++) {
			ptr->m_renderTargets[i] = renderTargets[i];
		}
		ptr->m_depthBuffer = depthBuffer;
	}
	return ptr;
}

Ref<IVertexDeclaration> IGraphicsDevice::createVertexDeclaration(const VertexElement* elements, int elementsCount)
{
	Ref<IVertexDeclaration> ptr = onCreateVertexDeclaration(elements, elementsCount);
	if (ptr) {
		ptr->m_hash = IVertexDeclaration::computeHash(elements, elementsCount);
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<IVertexBuffer> IGraphicsDevice::createVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
	Ref<IVertexBuffer> ptr = onCreateVertexBuffer(usage, bufferSize, initialData);
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<IIndexBuffer> IGraphicsDevice::createIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
	Ref<IIndexBuffer> ptr = onCreateIndexBuffer(usage, format, indexCount, initialData);
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<ITexture> IGraphicsDevice::createTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	Ref<ITexture> ptr = onCreateTexture2D(usage, width, height, requestFormat, mipmap, initialData);
	ptr->m_mipmap = mipmap;
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<ITexture> IGraphicsDevice::createTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	Ref<ITexture> ptr = onCreateTexture3D(usage, width, height, depth, requestFormat, mipmap, initialData);
	ptr->m_mipmap = mipmap;
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<ITexture> IGraphicsDevice::createRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
	Ref<ITexture> ptr = onCreateRenderTarget(width, height, requestFormat, mipmap);
	ptr->m_mipmap = mipmap;
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<ITexture> IGraphicsDevice::createWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight)
{
    Ref<ITexture> ptr = onCreateWrappedRenderTarget(nativeObject, hintWidth, hintHeight);
    if (ptr) {
        m_aliveObjects.push_back(ptr);
    }
    return ptr;
}

Ref<IDepthBuffer> IGraphicsDevice::createDepthBuffer(uint32_t width, uint32_t height)
{
	Ref<IDepthBuffer> ptr = onCreateDepthBuffer(width, height);
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<ISamplerState> IGraphicsDevice::createSamplerState(const SamplerStateData& desc)
{
	Ref<ISamplerState> ptr = onCreateSamplerState(desc);
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<IShaderPass> IGraphicsDevice::createShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
	diag->level = ShaderCompilationResultLevel::Success;
	diag->message.clear();

	Ref<IShaderPass> ptr = onCreateShaderPass(createInfo, diag);

	if (!diag->message.empty()) {
		LN_LOG_VERBOSE << diag->message;
	}

	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<IPipeline> IGraphicsDevice::createPipeline(IRenderPass* renderPass, const GraphicsContextState& state)
{
	Ref<IPipeline> ptr = onCreatePipeline(renderPass, state);
	if (ptr) {
		ptr->m_device = this;
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<IGraphicsContext> IGraphicsDevice::createGraphicsContext()
{
	return onCreateGraphicsContext();
}

void IGraphicsDevice::flushCommandBuffer(IGraphicsContext* context, ITexture* affectRendreTarget)
{
	onFlushCommandBuffer(context, affectRendreTarget);
}

Ref<IShaderPass> IGraphicsDevice::createShaderPassFromUnifiedShaderPass(const UnifiedShader* unifiedShader, UnifiedShader::PassId passId, DiagnosticsManager* diag)
{
    LN_DCHECK(unifiedShader);
    LN_DCHECK(diag);
    auto& triple = caps().requestedShaderTriple;

    detail::UnifiedShader::CodeContainerId vscodeId = unifiedShader->vertexShader(passId);
    detail::UnifiedShader::CodeContainerId pscodeId = unifiedShader->pixelShader(passId);

    //const std::vector<byte_t>* vscode = nullptr;
    //const std::vector<byte_t>* pscode = nullptr;
    const char* vsEntryPointName = nullptr;
    const char* psEntryPointName = nullptr;
    const detail::UnifiedShader::CodeInfo* vscode = nullptr;
    const detail::UnifiedShader::CodeInfo* pscode = nullptr;
    if (vscodeId) {
        vsEntryPointName = unifiedShader->entryPointName(vscodeId).c_str();
        vscode = unifiedShader->findCode(vscodeId, triple);
    }
    if (pscodeId) {
        psEntryPointName = unifiedShader->entryPointName(pscodeId).c_str();
        pscode = unifiedShader->findCode(pscodeId, triple);
    }

    detail::ShaderPassCreateInfo createInfo =
    {
        (vscode) ? vscode->code.data() : nullptr,
        (vscode) ? vscode->code.size() : 0,
        (pscode) ? pscode->code.data() : nullptr,
        (pscode) ? pscode->code.size() : 0,
        vsEntryPointName,
        psEntryPointName,
        &unifiedShader->descriptorLayout(passId),
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

void IGraphicsDevice::collectGarbageObjects()
{
	for (int i = m_aliveObjects.size() - 1; i >= 0; i--)
	{
		if (RefObjectHelper::getReferenceCount(m_aliveObjects[i]) <= 1) {
			m_aliveObjects[i]->dispose();
			m_aliveObjects.erase(m_aliveObjects.begin() + i);
		}
	}
}

//=============================================================================
// IGraphicsContext

IGraphicsContext::IGraphicsContext()
	: m_device(nullptr)
    , m_stateDirtyFlags(GraphicsContextStateDirtyFlags_All)
    , m_staging()
    , m_committed()
{
}

Result IGraphicsContext::init(IGraphicsDevice* owner)
{
	m_device = owner;
	return true;
}

void IGraphicsContext::enterRenderState()
{
	onSaveExternalRenderState();
}

void IGraphicsContext::leaveRenderState()
{
	onRestoreExternalRenderState();
}

void IGraphicsContext::begin()
{
    m_stateDirtyFlags = GraphicsContextStateDirtyFlags_All;
    onBeginCommandRecoding();
}

void IGraphicsContext::end()
{
    onEndCommandRecoding();
}

void IGraphicsContext::setBlendState(const BlendStateDesc& value)
{
    m_staging.pipelineState.blendState = value;
    m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
}

void IGraphicsContext::setRasterizerState(const RasterizerStateDesc& value)
{
    m_staging.pipelineState.rasterizerState = value;
    m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
}

void IGraphicsContext::setDepthStencilState(const DepthStencilStateDesc& value)
{
    m_staging.pipelineState.depthStencilState = value;
    m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
}

void IGraphicsContext::setColorBuffer(int index, ITexture* value)
{
    if (m_staging.framebufferState.renderTargets[index] != value) {
        m_staging.framebufferState.renderTargets[index] = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_FrameBuffers;
    }
}

void IGraphicsContext::setDepthBuffer(IDepthBuffer* value)
{
    if (m_staging.framebufferState.depthBuffer != value) {
        m_staging.framebufferState.depthBuffer = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_FrameBuffers;
    }
}

void IGraphicsContext::setViewportRect(const RectI& value)
{
    if (m_staging.regionRects.viewportRect != value) {
        m_staging.regionRects.viewportRect = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_RegionRects;
    }
}

void IGraphicsContext::setScissorRect(const RectI& value)
{
    if (m_staging.regionRects.scissorRect != value) {
        m_staging.regionRects.scissorRect = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_RegionRects;
    }
}

void IGraphicsContext::setVertexDeclaration(IVertexDeclaration* value)
{
    if (m_staging.pipelineState.vertexDeclaration != value) {
        m_staging.pipelineState.vertexDeclaration = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
    }
}

void IGraphicsContext::setVertexBuffer(int streamIndex, IVertexBuffer* value)
{
    if (m_staging.primitive.vertexBuffers[streamIndex] != value) {
        m_staging.primitive.vertexBuffers[streamIndex] = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_Primitives;
    }
}

void IGraphicsContext::setIndexBuffer(IIndexBuffer* value)
{
    if (m_staging.primitive.indexBuffer != value) {
        m_staging.primitive.indexBuffer = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_Primitives;
    }
}

void IGraphicsContext::setShaderPass(IShaderPass* value)
{
    if (m_staging.shaderPass != value) {
        m_staging.shaderPass = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_ShaderPass;
    }
}

void IGraphicsContext::setPrimitiveTopology(PrimitiveTopology value)
{
    m_staging.pipelineState.topology = value;
}

void* IGraphicsContext::map(IGraphicsResource* resource, uint32_t offset, uint32_t size)
{
    return onMapResource(resource, offset, size);
}

void IGraphicsContext::unmap(IGraphicsResource* resource)
{
    onUnmapResource(resource);
}

void IGraphicsContext::setSubData(IGraphicsResource* resource, size_t offset, const void* data, size_t length)
{
    onSetSubData(resource, offset, data, length);
}

void IGraphicsContext::setSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize)
{
    onSetSubData2D(resource, x, y, width, height, data, dataSize);
}

void IGraphicsContext::setSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    onSetSubData3D(resource, x, y, z, width, height, depth, data, dataSize);
}

void IGraphicsContext::clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
    commitStatus(GraphicsContextSubmitSource_Clear);
    onClearBuffers(flags, color, z, stencil);
    endCommit();
}

void IGraphicsContext::drawPrimitive(int startVertex, int primitiveCount)
{
    commitStatus(GraphicsContextSubmitSource_Draw);
    onDrawPrimitive(m_staging.pipelineState.topology, startVertex, primitiveCount);
    endCommit();
}

void IGraphicsContext::drawPrimitiveIndexed(int startIndex, int primitiveCount)
{
    commitStatus(GraphicsContextSubmitSource_Draw);
    onDrawPrimitiveIndexed(m_staging.pipelineState.topology, startIndex, primitiveCount);
    endCommit();
}

void IGraphicsContext::commitStatus(GraphicsContextSubmitSource submitSource)
{
    if (LN_REQUIRE(m_staging.framebufferState.renderTargets[0])) return;
    //if (LN_REQUIRE(m_staging.pipelineState.vertexDeclaration)) return;



    // TODO: modified check

    onUpdatePipelineState(m_staging.pipelineState.blendState, m_staging.pipelineState.rasterizerState, m_staging.pipelineState.depthStencilState);

    onUpdateShaderPass(m_staging.shaderPass);

    onUpdateFrameBuffers(m_staging.framebufferState.renderTargets.data(), m_staging.framebufferState.renderTargets.size(), m_staging.framebufferState.depthBuffer);

    onUpdateRegionRects(m_staging.regionRects.viewportRect, m_staging.regionRects.scissorRect, m_staging.framebufferState.renderTargets[0]->realSize());

    onUpdatePrimitiveData(m_staging.pipelineState.vertexDeclaration, m_staging.primitive.vertexBuffers.data(), m_staging.primitive.vertexBuffers.size(), m_staging.primitive.indexBuffer);

    onSubmitStatus(m_staging, m_stateDirtyFlags, submitSource);
}

void IGraphicsContext::endCommit()
{
    m_committed = m_staging;
    m_stateDirtyFlags = GraphicsContextStateDirtyFlags_None;
}


//=============================================================================
// ISwapChain

ISwapChain::ISwapChain()
{
	LN_LOG_VERBOSE << "ISwapChain [0x" << this << "] constructed.";
}

//=============================================================================
// IRenderPass

void IRenderPass::dispose()
{
	if (m_device) {
		m_device->renderPassCache()->invalidate(this);
		m_device = nullptr;
	}

	IGraphicsDeviceObject::dispose();
}

//=============================================================================
// IVertexDeclaration

IVertexDeclaration::IVertexDeclaration()
{
	LN_LOG_VERBOSE << "IVertexDeclaration [0x" << this << "] constructed.";
}

uint64_t IVertexDeclaration::computeHash(const VertexElement* elements, int count)
{
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

//=============================================================================
// IVertexBuffer

IVertexBuffer::IVertexBuffer()
{
	LN_LOG_VERBOSE << "IVertexBuffer [0x" << this << "] constructed.";
}

//=============================================================================
// IIndexBuffer

IIndexBuffer::IIndexBuffer()
{
	LN_LOG_VERBOSE << "IIndexBuffer [0x" << this << "] constructed.";
}

//=============================================================================
// ITexture

ITexture::ITexture()
	: m_mipmap(false)
{
	LN_LOG_VERBOSE << "ITexture [0x" << this << "] constructed.";
}

//=============================================================================
// IDepthBuffer

IDepthBuffer::IDepthBuffer()
{
	LN_LOG_VERBOSE << "IDepthBuffer [0x" << this << "] constructed.";
}

//=============================================================================
// ISamplerState

ISamplerState::ISamplerState()
{
	LN_LOG_VERBOSE << "ISamplerState [0x" << this << "] constructed.";
}

//=============================================================================
// IShaderPass

IShaderPass::IShaderPass()
{
	LN_LOG_VERBOSE << "IShaderPass [0x" << this << "] constructed.";
}

//=============================================================================
// IShaderPass

IShaderUniformBuffer::IShaderUniformBuffer()
{
	LN_LOG_VERBOSE << "IShaderUniformBuffer [0x" << this << "] constructed.";
}

//=============================================================================
// IShaderUniform

IShaderUniform::IShaderUniform()
{
	LN_LOG_VERBOSE << "IShaderUniform [0x" << this << "] constructed.";
}

//=============================================================================
// IShaderSamplerBuffer

IShaderSamplerBuffer::IShaderSamplerBuffer()
{
	LN_LOG_VERBOSE << "IShaderSamplerBuffer [0x" << this << "] constructed.";
}

//=============================================================================
// IPipeline

void IPipeline::dispose()
{
	if (m_device) {
		m_device->pipelineCache()->invalidate(this);
		m_device = nullptr;
	}

	IGraphicsDeviceObject::dispose();
}

//=============================================================================
// NativeRenderPassCache

NativeRenderPassCache::NativeRenderPassCache(IGraphicsDevice* device)
	: m_device(device)
	, m_hashMap()
{
	assert(m_device);
}

IRenderPass* NativeRenderPassCache::findOrCreate(const FindKey& key)
{
	uint64_t hash = computeHash(key);
	auto itr = m_hashMap.find(hash);
	if (itr != m_hashMap.end()) {
		return itr->second;
	}
	else {
		std::array<ITexture*, MaxMultiRenderTargets> renderTargets;
		size_t i = 0;
		for (; i < key.renderTargets.size(); i++) {
			if (!key.renderTargets[i]) break;
			renderTargets[i] = key.renderTargets[i];
		}

		auto renderPass = m_device->createRenderPass(renderTargets.data(), i, key.depthBuffer, key.clearFlags, key.clearColor, key.clearZ, key.clearStencil);
		if (!renderPass) {
			return nullptr;
		}

		m_hashMap.insert({ hash, renderPass });
		return renderPass;
	}
}

void NativeRenderPassCache::invalidate(IRenderPass* value)
{
	if (value) {
		m_hashMap.erase(value->cacheKeyHash);
	}
}

uint64_t NativeRenderPassCache::computeHash(const FindKey& key)
{
	MixHash hash;
	for (size_t i = 0; i < key.renderTargets.size(); i++) {
		// TODO: Format だけでもいいかも。Vulkan はそうだった。
		hash.add(key.renderTargets[i]);
	}
	// TODO: Format だけでもいいかも。Vulkan はそうだった。
	hash.add(key.depthBuffer);
	hash.add(key.clearFlags);
	return hash.value();
}

//=============================================================================
// NativePipelineCache

NativePipelineCache::NativePipelineCache(IGraphicsDevice* device)
	: m_device(device)
	, m_hashMap()
{
	assert(m_device);
}

IPipeline* NativePipelineCache::findOrCreate(const FindKey& key)
{
	uint64_t hash = computeHash(key);
	auto itr = m_hashMap.find(hash);
	if (itr != m_hashMap.end()) {
		return itr->second;
	}
	else {
		auto pipeline = m_device->createPipeline(key.renderPass, key.state);
		if (!pipeline) {
			return nullptr;
		}

		m_hashMap.insert({ hash, pipeline });
		return pipeline;
	}
}

void NativePipelineCache::invalidate(IPipeline* value)
{
	if (value) {
		m_hashMap.erase(value->cacheKeyHash);
	}
}

uint64_t NativePipelineCache::computeHash(const FindKey& key)
{
	uint64_t vertexDeclarationHash = (key.state.pipelineState.vertexDeclaration) ? key.state.pipelineState.vertexDeclaration->hash() : 0;

	MixHash hash;
	hash.add(key.state.pipelineState.blendState);
	hash.add(key.state.pipelineState.rasterizerState);
	hash.add(key.state.pipelineState.depthStencilState);
	hash.add(key.state.pipelineState.topology);
	hash.add(vertexDeclarationHash);
	hash.add(key.state.shaderPass);
	hash.add(key.renderPass);
	return hash.value();
}

} // namespace detail
} // namespace ln

