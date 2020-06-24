
#include "Internal.hpp"
#include "MixHash.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {


//=============================================================================
// IGraphicsDeviceObject

AttributeUsage IGraphicsHelper::ElementUsageToAttributeUsage(VertexElementUsage value)
{
	static struct
	{
		VertexElementUsage v1;
		AttributeUsage v2;
	}
	s_conversionTable[] =
	{
		{ VertexElementUsage::Unknown, AttributeUsage_Unknown },
		{ VertexElementUsage::Position, AttributeUsage_Position },
		{ VertexElementUsage::Normal, AttributeUsage_Normal },
		{ VertexElementUsage::Color, AttributeUsage_Color },
		{ VertexElementUsage::TexCoord, AttributeUsage_TexCoord},
		{ VertexElementUsage::PointSize, AttributeUsage_Unknown },
		{ VertexElementUsage::Tangent, AttributeUsage_Tangent },
		{ VertexElementUsage::Binormal, AttributeUsage_Binormal,},
		{ VertexElementUsage::BlendIndices, AttributeUsage_BlendIndices },
		{ VertexElementUsage::BlendWeight, AttributeUsage_BlendWeight },
	};
	assert(s_conversionTable[(int)value].v1 == value);
	return s_conversionTable[(int)value].v2;
}

VertexElementUsage IGraphicsHelper::AttributeUsageToElementUsage(AttributeUsage value)
{
	static struct
	{
		AttributeUsage v1;
		VertexElementUsage v2;
	}
	s_conversionTable[] =
	{
		{ AttributeUsage_Unknown, VertexElementUsage::Unknown },
		{ AttributeUsage_Position, VertexElementUsage::Position },
		{ AttributeUsage_BlendIndices, VertexElementUsage::BlendIndices },
		{ AttributeUsage_BlendWeight, VertexElementUsage::BlendWeight },
		{ AttributeUsage_Normal, VertexElementUsage::Normal },
		{ AttributeUsage_TexCoord, VertexElementUsage::TexCoord },
		{ AttributeUsage_Tangent, VertexElementUsage::Tangent },
		{ AttributeUsage_Binormal, VertexElementUsage::Binormal },
		{ AttributeUsage_Color, VertexElementUsage::Color },
	};
	assert(s_conversionTable[(int)value].v1 == value);
	return s_conversionTable[(int)value].v2;
}

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

void IGraphicsDeviceObject::finalize()
{
#ifdef LN_DEBUG
    bool d = m_disposed;
    dispose();
    if (!d) {
        LN_CHECK(m_disposed);
    }
#else
    dispose();
#endif
}

void IGraphicsDeviceObject::dispose()
{
    m_disposed = true;
}

//=============================================================================
// IGraphicsRHIBuffer

IGraphicsRHIBuffer::~IGraphicsRHIBuffer()
{
}

//=============================================================================
// IGraphicsDevice

IGraphicsDevice::IGraphicsDevice()
	: m_renderPassCache(std::make_unique<NativeRenderPassCache>(this))
	, m_pipelineCache(std::make_unique<NativePipelineCache>(this))
{
}

void IGraphicsDevice::init()
{
}

void IGraphicsDevice::dispose()
{
	m_pipelineCache->clear();
	m_renderPassCache->clear();

	//for (auto& obj : m_aliveObjects) {
	//	obj->dispose();
	//}
	m_aliveObjects.clear();
}

void IGraphicsDevice::refreshCaps()
{
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

Ref<ICommandList> IGraphicsDevice::createCommandList()
{
	return onCreateCommandList();
}

Ref<IRenderPass> IGraphicsDevice::createRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil)
{
	Ref<IRenderPass> ptr = onCreateRenderPass(buffers, clearFlags, clearColor, clearDepth, clearStencil);
	if (ptr) {
		ptr->m_device = this;
		m_aliveObjects.push_back(ptr);

		// Preserve dependent object references
		for (uint32_t i = 0; i < buffers.renderTargets.size(); i++) {
			ptr->m_renderTargets[i] = buffers.renderTargets[i];
		}
		ptr->m_depthBuffer = buffers.depthBuffer;
	}
	return ptr;
}

Ref<IPipeline> IGraphicsDevice::createPipeline(const DevicePipelineStateDesc& state)
{
	Ref<IPipeline> ptr = onCreatePipeline(state);
	if (ptr) {
		ptr->m_sourceVertexLayout = state.vertexDeclaration;
		ptr->m_sourceRenderPass = state.renderPass;
		ptr->m_sourceShaderPass = state.shaderPass;
	}
	return ptr;
}

Ref<IVertexDeclaration> IGraphicsDevice::createVertexDeclaration(const VertexElement* elements, int elementsCount)
{
	Ref<IVertexDeclaration> ptr = onCreateVertexDeclaration(elements, elementsCount);
	if (ptr) {
		ptr->m_device = this;
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
		ptr->m_device = this;
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

void IGraphicsDevice::flushCommandBuffer(ICommandList* context, ITexture* affectRendreTarget)
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

//void IGraphicsDevice::collectGarbageObjects()
//{
//	for (int i = m_aliveObjects.size() - 1; i >= 0; i--)
//	{
//		if (RefObjectHelper::getReferenceCount(m_aliveObjects[i]) <= 1) {
//			m_aliveObjects[i]->dispose();
//			m_aliveObjects.erase(m_aliveObjects.begin() + i);
//		}
//	}
//}

//=============================================================================
// ICommandList

ICommandList::ICommandList()
	: m_device(nullptr)
    , m_stateDirtyFlags(GraphicsContextStateDirtyFlags_All)
    , m_staging()
    , m_committed()
	, m_currentRenderPass(nullptr)
{
}

Result ICommandList::init(IGraphicsDevice* owner)
{
	m_device = owner;
	return true;
}

void ICommandList::enterRenderState()
{
	onSaveExternalRenderState();
}

void ICommandList::leaveRenderState()
{
	onRestoreExternalRenderState();
}

void ICommandList::begin()
{
    m_stateDirtyFlags = GraphicsContextStateDirtyFlags_All;
    onBeginCommandRecoding();
}

void ICommandList::end()
{
    onEndCommandRecoding();
}

void ICommandList::beginRenderPass(IRenderPass* value)
{
	if (LN_REQUIRE(!m_currentRenderPass)) return;
	m_currentRenderPass = value;
	onBeginRenderPass(value);
}

void ICommandList::endRenderPass(IRenderPass* value)
{
	if (LN_REQUIRE(m_currentRenderPass && m_currentRenderPass == value)) return;
	onEndRenderPass(value);
	m_currentRenderPass = nullptr;
}

void ICommandList::setBlendState(const BlendStateDesc& value)
{
    m_staging.pipelineState.blendState = value;
    m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
}

void ICommandList::setRasterizerState(const RasterizerStateDesc& value)
{
    m_staging.pipelineState.rasterizerState = value;
    m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
}

void ICommandList::setDepthStencilState(const DepthStencilStateDesc& value)
{
    m_staging.pipelineState.depthStencilState = value;
    m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
}

//void ICommandList::setColorBuffer(int index, ITexture* value)
//{
//    if (m_staging.framebufferState.renderTargets[index] != value) {
//        m_staging.framebufferState.renderTargets[index] = value;
//        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_FrameBuffers;
//    }
//}
//
//void ICommandList::setDepthBuffer(IDepthBuffer* value)
//{
//    if (m_staging.framebufferState.depthBuffer != value) {
//        m_staging.framebufferState.depthBuffer = value;
//        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_FrameBuffers;
//    }
//}

void ICommandList::setViewportRect(const RectI& value)
{
    if (m_staging.regionRects.viewportRect != value) {
        m_staging.regionRects.viewportRect = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_RegionRects;
    }
}

void ICommandList::setScissorRect(const RectI& value)
{
    if (m_staging.regionRects.scissorRect != value) {
        m_staging.regionRects.scissorRect = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_RegionRects;
    }
}

void ICommandList::setVertexDeclaration(IVertexDeclaration* value)
{
    if (m_staging.pipelineState.vertexDeclaration != value) {
        m_staging.pipelineState.vertexDeclaration = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_PipelineState;
    }
}

void ICommandList::setVertexBuffer(int streamIndex, IVertexBuffer* value)
{
    if (m_staging.primitive.vertexBuffers[streamIndex] != value) {
        m_staging.primitive.vertexBuffers[streamIndex] = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_Primitives;
    }
}

void ICommandList::setIndexBuffer(IIndexBuffer* value)
{
    if (m_staging.primitive.indexBuffer != value) {
        m_staging.primitive.indexBuffer = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_Primitives;
    }
}

void ICommandList::setShaderPass(IShaderPass* value)
{
    if (m_staging.shaderPass != value) {
        m_staging.shaderPass = value;
        m_stateDirtyFlags |= GraphicsContextStateDirtyFlags_ShaderPass;
    }
}

void ICommandList::setPrimitiveTopology(PrimitiveTopology value)
{
    m_staging.pipelineState.topology = value;
}
//
//void ICommandList::setRenderPass(IRenderPass* value)
//{
//	m_staging.renderPass = value;
//}

void* ICommandList::map(IGraphicsRHIBuffer* resource, uint32_t offset, uint32_t size)
{
    return onMapResource(resource, offset, size);
}

void ICommandList::unmap(IGraphicsRHIBuffer* resource)
{
    onUnmapResource(resource);
}

void ICommandList::setSubData(IGraphicsRHIBuffer* resource, size_t offset, const void* data, size_t length)
{
    onSetSubData(resource, offset, data, length);
}

void ICommandList::setSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize)
{
    onSetSubData2D(resource, x, y, width, height, data, dataSize);
}

void ICommandList::setSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    onSetSubData3D(resource, x, y, z, width, height, depth, data, dataSize);
}

void ICommandList::setDescriptorTableData(IShaderDescriptorTable* resource, const ShaderDescriptorTableUpdateInfo* data)
{
	onSetDescriptorTableData(resource, data);
}

void ICommandList::clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
    commitStatus(GraphicsContextSubmitSource_Clear);
    onClearBuffers(flags, color, z, stencil);
    endCommit(GraphicsContextSubmitSource_Clear);
}

void ICommandList::drawPrimitive(int startVertex, int primitiveCount)
{
    commitStatus(GraphicsContextSubmitSource_Draw);
    onDrawPrimitive(m_staging.pipelineState.topology, startVertex, primitiveCount);
    endCommit(GraphicsContextSubmitSource_Draw);
}

void ICommandList::drawPrimitiveIndexed(int startIndex, int primitiveCount, int instanceCount)
{
    commitStatus(GraphicsContextSubmitSource_Draw);
    onDrawPrimitiveIndexed(m_staging.pipelineState.topology, startIndex, primitiveCount, instanceCount);
    endCommit(GraphicsContextSubmitSource_Draw);
}

void ICommandList::drawExtension(INativeGraphicsExtension* extension)
{
	commitStatus(GraphicsContextSubmitSource_Extension);
	onDrawExtension(extension);
	endCommit(GraphicsContextSubmitSource_Extension);
}

void ICommandList::commitStatus(GraphicsContextSubmitSource submitSource)
{
    //if (LN_REQUIRE(m_staging.framebufferState.renderTargets[0])) return;
    //if (LN_REQUIRE(m_staging.pipelineState.vertexDeclaration)) return;


	if (m_staging.shaderPass
        && m_staging.pipelineState.vertexDeclaration && m_currentRenderPass // extention 描画時
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
		IPipeline* pipeline = m_device->pipelineCache()->findOrCreate(state);

		onSubmitStatus(m_staging, m_stateDirtyFlags, submitSource, pipeline);
	}
	else {
        // clear や、extension 呼び出し。pipelineState を独自で作らない。
		onSubmitStatus(m_staging, m_stateDirtyFlags, submitSource, nullptr);
	}
}

void ICommandList::endCommit(GraphicsContextSubmitSource submitSource)
{
	if (submitSource == GraphicsContextSubmitSource_Extension) {
		// 次回の描画時にすべて設定しなおす
		m_stateDirtyFlags = GraphicsContextStateDirtyFlags_All;
	}
	else {
		m_committed = m_staging;
		m_stateDirtyFlags = GraphicsContextStateDirtyFlags_None;
	}
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
		m_device->pipelineCache()->invalidate(this);
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

bool IVertexDeclaration::init(const VertexElement* elements, int count)
{
	//m_elements.resize(count);
	//for (int i = 0; i < count; i++) m_elements[i] = elements[i];
	return true;
}

void IVertexDeclaration::dispose()
{
	if (m_device) {
		m_device->pipelineCache()->invalidate(this);
		m_device = nullptr;
	}

	IGraphicsDeviceObject::dispose();
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

//const VertexElement* IVertexDeclaration::findElement(AttributeUsage usage, int usageIndex) const
//{
//	// TODO: これ線形探索じゃなくて、map 作った方がいいかも。
//	// usage の種類は固定だし、usageIndex も最大 16 あれば十分だし、byte 型 8x16 くらいの Matrix で足りる。
//	auto u = IGraphicsHelper::AttributeUsageToElementUsage(usage);
//	for (auto& e : m_elements) {
//		if (e.Usage == u && e.UsageIndex == usageIndex) {
//			return &e;
//		}
//	}
//	return nullptr;
//}

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

void IShaderPass::dispose()
{
	if (m_device) {
		m_device->pipelineCache()->invalidate(this);
		m_device = nullptr;
	}
	IGraphicsDeviceObject::dispose();
}

bool IShaderPass::init(const ShaderPassCreateInfo& createInfo)
{
	m_attributes = *(createInfo.attributes);
	return true;
}

const VertexInputAttribute* IShaderPass::findAttribute(VertexElementUsage usage, int usageIndex) const
{
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
	//IGraphicsDeviceObject::dispose();
}

//=============================================================================
// NativeRenderPassCache

NativeRenderPassCache::NativeRenderPassCache(IGraphicsDevice* device)
	: m_device(device)
	, m_hashMap()
{
	assert(m_device);
}

void NativeRenderPassCache::clear()
{
	for (auto& pair : m_hashMap) {
		pair.second.value->dispose();
	}
	m_hashMap.clear();
	DiagnosticsManager::activeDiagnostics()->setCounterValue(ProfilingItem::Graphics_RenderPassCount, m_hashMap.size());
}

IRenderPass* NativeRenderPassCache::findOrCreate(const FindKey& key)
{
	uint64_t hash = computeHash(key);
	auto itr = m_hashMap.find(hash);
	if (itr != m_hashMap.end()) {
		itr->second.referenceCount++;
		return itr->second.value;
	}
	else {
		 DeviceFramebufferState buffers;
		size_t i = 0;
		for (; i < key.renderTargets.size(); i++) {
			if (!key.renderTargets[i]) break;
			buffers.renderTargets[i] = key.renderTargets[i];
		}
		buffers.depthBuffer = key.depthBuffer;

		auto renderPass = m_device->createRenderPass(buffers, key.clearFlags, key.clearColor, key.clearDepth, key.clearStencil);
		if (!renderPass) {
			return nullptr;
		}

		m_hashMap.insert({ hash, { 1, renderPass } });
		DiagnosticsManager::activeDiagnostics()->setCounterValue(ProfilingItem::Graphics_RenderPassCount, m_hashMap.size());
		return renderPass;
	}
}

void NativeRenderPassCache::release(IRenderPass* value)
{
	if (value) {
		auto itr = m_hashMap.find(value->cacheKeyHash);
		if (itr != m_hashMap.end()) {
			itr->second.referenceCount--;
			if (itr->second.referenceCount <= 0) {
				// TODO: 削除しない。すぐに削除されても、また同じパラメータで使うかもしれない。
				// 後々最大数の設定は必要だろう。その時も、次回 create するときに空きが無ければ消すようにしたい。
				//m_hashMap.erase(itr);
			}
		}
	}
}

uint64_t NativeRenderPassCache::computeHash(const FindKey& key)
{
	MixHash hash;
	for (size_t i = 0; i < key.renderTargets.size(); i++) {
		// TODO: Format だけでもいいかも。Vulkan はそうだった。
		hash.add(key.renderTargets[i]);
	}
	hash.add(key.depthBuffer);  // TODO: Format だけでもいいかも。Vulkan はそうだった。
	hash.add(key.clearFlags);   // TODO: 以下、一つの構造体にまとめれば少し高速化できそう
    hash.add(key.clearColor);
    hash.add(key.clearDepth);
    hash.add(key.clearStencil);
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

void NativePipelineCache::clear()
{
	for (auto& pair : m_hashMap) {
		pair.second->dispose();
	}
	m_hashMap.clear();
	m_device = nullptr;
}

IPipeline* NativePipelineCache::findOrCreate(const FindKey& key)
{
	if (LN_REQUIRE(m_device)) return nullptr;

	uint64_t hash = computeHash(key);
	auto itr = m_hashMap.find(hash);
	if (itr != m_hashMap.end()) {
		return itr->second;
	}
	else {
		auto pipeline = m_device->createPipeline(key);
		if (!pipeline) {
			return nullptr;
		}

		m_hashMap.insert({ hash, pipeline });
		return pipeline;
	}
}

void NativePipelineCache::invalidate(IVertexDeclaration* value)
{
	for (auto itr = m_hashMap.begin(); itr != m_hashMap.end(); ) {
		if (itr->second->m_sourceVertexLayout == value) {
			itr->second->dispose();
			itr = m_hashMap.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void NativePipelineCache::invalidate(IRenderPass* value)
{
	for (auto itr = m_hashMap.begin(); itr != m_hashMap.end(); ) {
		if (itr->second->m_sourceRenderPass == value) {
			itr->second->dispose();
			itr = m_hashMap.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void NativePipelineCache::invalidate(IShaderPass* value)
{
	for (auto itr = m_hashMap.begin(); itr != m_hashMap.end(); ) {
		if (itr->second->m_sourceShaderPass == value) {
			itr->second->dispose();
			itr = m_hashMap.erase(itr);
		}
		else {
			++itr;
		}
	}
}

//void NativePipelineCache::invalidate(IPipeline* value)
//{
//	if (value) {
//		m_hashMap.erase(value->cacheKeyHash);
//	}
//}

uint64_t NativePipelineCache::computeHash(const FindKey& key)
{
	uint64_t vertexDeclarationHash = (key.vertexDeclaration) ? key.vertexDeclaration->hash() : 0;

	MixHash hash;
	hash.add(key.blendState);
	hash.add(key.rasterizerState);
	hash.add(key.depthStencilState);
	hash.add(key.topology);
	hash.add(vertexDeclarationHash);
	hash.add(key.shaderPass);
	hash.add(key.renderPass);
	return hash.value();


	//uint64_t vertexDeclarationHash = (key.state.pipelineState.vertexDeclaration) ? key.state.pipelineState.vertexDeclaration->hash() : 0;

	//MixHash hash;
	//hash.add(key.state.pipelineState.blendState);
	//hash.add(key.state.pipelineState.rasterizerState);
	//hash.add(key.state.pipelineState.depthStencilState);
	//hash.add(key.state.pipelineState.topology);
	//hash.add(vertexDeclarationHash);
	//hash.add(key.state.shaderPass);
	//hash.add(key.renderPass);
	//return hash.value();
}

} // namespace detail
} // namespace ln

