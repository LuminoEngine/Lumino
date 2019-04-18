
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

void IGraphicsDevice::enterMainThread()
{
}

void IGraphicsDevice::leaveMainThread()
{
}

void IGraphicsDevice::enterRenderState()
{
	onSaveExternalRenderState();
}

void IGraphicsDevice::leaveRenderState()
{
	onRestoreExternalRenderState();
}

Ref<ISwapChain> IGraphicsDevice::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	Ref<ISwapChain> ptr = onCreateSwapChain(window, backbufferSize);
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<IVertexDeclaration> IGraphicsDevice::createVertexDeclaration(const VertexElement* elements, int elementsCount)
{
	Ref<IVertexDeclaration> ptr = onCreateVertexDeclaration(elements, elementsCount);
	if (ptr) {
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

Ref<ITexture> IGraphicsDevice::createTexture2D(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	Ref<ITexture> ptr = onCreateTexture2D(width, height, requestFormat, mipmap, initialData);
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<ITexture> IGraphicsDevice::createTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	Ref<ITexture> ptr = onCreateTexture3D(width, height, depth, requestFormat, mipmap, initialData);
	if (ptr) {
		m_aliveObjects.push_back(ptr);
	}
	return ptr;
}

Ref<ITexture> IGraphicsDevice::createRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
	Ref<ITexture> ptr = onCreateRenderTarget(width, height, requestFormat, mipmap);
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

void* IGraphicsContext::map(IGraphicsResource* resource)
{
    return onMapResource(resource);
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

void IGraphicsContext::flushCommandBuffer(ITexture* affectRendreTarget)
{
    onFlushCommandBuffer(affectRendreTarget);
}

void IGraphicsContext::present(ISwapChain* swapChain)
{
    onPresent(swapChain);
    m_device->collectGarbageObjects();
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
// IVertexDeclaration

IVertexDeclaration::IVertexDeclaration()
{
	LN_LOG_VERBOSE << "IVertexDeclaration [0x" << this << "] constructed.";
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

} // namespace detail
} // namespace ln

