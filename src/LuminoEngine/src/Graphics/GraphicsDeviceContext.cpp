
#include "Internal.hpp"
#include "MixHash.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {

//=============================================================================
// IGraphicsDeviceContext

IGraphicsDeviceContext::IGraphicsDeviceContext()
{
}

void IGraphicsDeviceContext::init()
{
}

void IGraphicsDeviceContext::dispose()
{
}

void IGraphicsDeviceContext::refreshCaps()
{
	onGetCaps(&m_caps);
}

void IGraphicsDeviceContext::enterMainThread()
{
}

void IGraphicsDeviceContext::leaveMainThread()
{
}

void IGraphicsDeviceContext::enterRenderState()
{
	onSaveExternalRenderState();
}

void IGraphicsDeviceContext::leaveRenderState()
{
	onRestoreExternalRenderState();
}

Ref<ISwapChain> IGraphicsDeviceContext::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	return onCreateSwapChain(window, backbufferSize);
}

Ref<IVertexDeclaration> IGraphicsDeviceContext::createVertexDeclaration(const VertexElement* elements, int elementsCount)
{
	return onCreateVertexDeclaration(elements, elementsCount);
}

Ref<IVertexBuffer> IGraphicsDeviceContext::createVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
	return onCreateVertexBuffer(usage, bufferSize, initialData);
}

Ref<IIndexBuffer> IGraphicsDeviceContext::createIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
	return onCreateIndexBuffer(usage, format, indexCount, initialData);
}

Ref<ITexture> IGraphicsDeviceContext::createTexture2D(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	return onCreateTexture2D(width, height, requestFormat, mipmap, initialData);
}

Ref<ITexture> IGraphicsDeviceContext::createTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	return onCreateTexture3D(width, height, depth, requestFormat, mipmap, initialData);
}

Ref<ITexture> IGraphicsDeviceContext::createRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
	return onCreateRenderTarget(width, height, requestFormat, mipmap);
}

Ref<IDepthBuffer> IGraphicsDeviceContext::createDepthBuffer(uint32_t width, uint32_t height)
{
	return onCreateDepthBuffer(width, height);
}

Ref<ISamplerState> IGraphicsDeviceContext::createSamplerState(const SamplerStateData& desc)
{
	return onCreateSamplerState(desc);
}

Ref<IShaderPass> IGraphicsDeviceContext::createShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
	diag->level = ShaderCompilationResultLevel::Success;
	diag->message.clear();

	auto pass = onCreateShaderPass(createInfo, diag);

	if (!diag->message.empty()) {
		LN_LOG_VERBOSE << diag->message;
	}

	return pass;
}

void IGraphicsDeviceContext::begin()
{
	m_stateDirtyFlags = StateDirtyFlags_All;
	onBeginCommandRecoding();
}

void IGraphicsDeviceContext::end()
{
	onEndCommandRecoding();
}

void IGraphicsDeviceContext::setBlendState(const BlendStateDesc& value)
{
	m_staging.pipelineState.blendState = value;
	m_stateDirtyFlags |= StateDirtyFlags_PipelineState;
}

void IGraphicsDeviceContext::setRasterizerState(const RasterizerStateDesc& value)
{
	m_staging.pipelineState.rasterizerState = value;
	m_stateDirtyFlags |= StateDirtyFlags_PipelineState;
}

void IGraphicsDeviceContext::setDepthStencilState(const DepthStencilStateDesc& value)
{
	m_staging.pipelineState.depthStencilState = value;
	m_stateDirtyFlags |= StateDirtyFlags_PipelineState;
}

void IGraphicsDeviceContext::setColorBuffer(int index, ITexture* value)
{
	if (m_staging.framebufferState.renderTargets[index] != value) {
		m_staging.framebufferState.renderTargets[index] = value;
		m_stateDirtyFlags |= StateDirtyFlags_FrameBuffers;
	}
}

void IGraphicsDeviceContext::setDepthBuffer(IDepthBuffer* value)
{
	if (m_staging.framebufferState.depthBuffer != value) {
		m_staging.framebufferState.depthBuffer = value;
		m_stateDirtyFlags |= StateDirtyFlags_FrameBuffers;
	}
}

void IGraphicsDeviceContext::setViewportRect(const RectI& value)
{
	if (m_staging.regionRects.viewportRect != value) {
		m_staging.regionRects.viewportRect = value;
		m_stateDirtyFlags |= StateDirtyFlags_RegionRects;
	}
}

void IGraphicsDeviceContext::setScissorRect(const RectI& value)
{
	if (m_staging.regionRects.scissorRect != value) {
		m_staging.regionRects.scissorRect = value;
		m_stateDirtyFlags |= StateDirtyFlags_RegionRects;
	}
}

void IGraphicsDeviceContext::setVertexDeclaration(IVertexDeclaration* value)
{
	if (m_staging.pipelineState.vertexDeclaration != value) {
		m_staging.pipelineState.vertexDeclaration = value;
		m_stateDirtyFlags |= StateDirtyFlags_PipelineState;
	}
}

void IGraphicsDeviceContext::setVertexBuffer(int streamIndex, IVertexBuffer* value)
{
	if (m_staging.primitive.vertexBuffers[streamIndex] != value) {
		m_staging.primitive.vertexBuffers[streamIndex] = value;
		m_stateDirtyFlags |= StateDirtyFlags_Primitives;
	}
}

void IGraphicsDeviceContext::setIndexBuffer(IIndexBuffer* value)
{
	if (m_staging.primitive.indexBuffer != value) {
		m_staging.primitive.indexBuffer = value;
		m_stateDirtyFlags |= StateDirtyFlags_Primitives;
	}
}

void IGraphicsDeviceContext::setShaderPass(IShaderPass* value)
{
	if (m_staging.shaderPass != value) {
		m_staging.shaderPass = value;
		m_stateDirtyFlags |= StateDirtyFlags_ShaderPass;
	}
}

void IGraphicsDeviceContext::setPrimitiveTopology(PrimitiveTopology value)
{
	m_staging.pipelineState.topology = value;
}

void IGraphicsDeviceContext::clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	commitStatus();
	onClearBuffers(flags, color, z, stencil);
}

void IGraphicsDeviceContext::drawPrimitive(int startVertex, int primitiveCount)
{
	commitStatus();
	onDrawPrimitive(m_staging.pipelineState.topology, startVertex, primitiveCount);
}

void IGraphicsDeviceContext::drawPrimitiveIndexed(int startIndex, int primitiveCount)
{
	commitStatus();
	onDrawPrimitiveIndexed(m_staging.pipelineState.topology, startIndex, primitiveCount);
}

void IGraphicsDeviceContext::flushCommandBuffer(ITexture* affectRendreTarget)
{
    onFlushCommandBuffer(affectRendreTarget);
}

void IGraphicsDeviceContext::present(ISwapChain* swapChain)
{
	onPresent(swapChain);
}

Ref<IShaderPass> IGraphicsDeviceContext::createShaderPassFromUnifiedShaderPass(const UnifiedShader* unifiedShader, UnifiedShader::PassId passId, DiagnosticsManager* diag)
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

    //auto rhiPass = createRHIShaderPass(
    //    //(vscode) ? vscode->code.data() : nullptr,
    //    //(vscode) ? vscode->code.size() : 0,
    //    //(pscode) ? pscode->code.data() : nullptr,
    //    //(pscode) ? pscode->code.size() : 0,
    //    ,
    //    vscode->refrection,
    //    pscode->refrection,
    //    diag);

    // TODO: ポインタの必要なし
    auto vertexInputAttributeTable = &unifiedShader->attributeSemantics(passId);


    detail::ShaderVertexInputAttributeTable attributeTable;
    if (vertexInputAttributeTable) {
        struct AttributeUsageConvertionItem
        {
            detail::AttributeUsage usage1;
            VertexElementUsage usage2;
        };
        static const AttributeUsageConvertionItem s_AttributeUsageConvertionTable[] = {
            { detail::AttributeUsage_Unknown, VertexElementUsage::Unknown },
            { detail::AttributeUsage_Position, VertexElementUsage::Position },
            { detail::AttributeUsage_BlendIndices, VertexElementUsage::BlendIndices },
            { detail::AttributeUsage_BlendWeight, VertexElementUsage::BlendWeight },
            { detail::AttributeUsage_Normal, VertexElementUsage::Normal },
            { detail::AttributeUsage_TexCoord, VertexElementUsage::TexCoord },
            { detail::AttributeUsage_Tangent, VertexElementUsage::Unknown },	// TODO:
            { detail::AttributeUsage_Binormal, VertexElementUsage::Unknown },	// TODO:
            { detail::AttributeUsage_Color, VertexElementUsage::Color },
        };

        for (size_t i = 0; i < vertexInputAttributeTable->size(); i++) {
            detail::ShaderVertexInputAttribute attr;
            assert(i == s_AttributeUsageConvertionTable[i].usage1);
            attr.usage = s_AttributeUsageConvertionTable[(int)vertexInputAttributeTable->at(i).usage].usage2;
            attr.index = vertexInputAttributeTable->at(i).index;
            attr.layoutLocation = vertexInputAttributeTable->at(i).layoutLocation;
            attributeTable.push_back(attr);
        }
    }

    detail::ShaderPassCreateInfo createInfo =
    {
        (vscode) ? vscode->code.data() : nullptr,
        (vscode) ? vscode->code.size() : 0,
        (pscode) ? pscode->code.data() : nullptr,
        (pscode) ? pscode->code.size() : 0,
        vsEntryPointName,
        psEntryPointName,
        &attributeTable,
        vscode->refrection,
        pscode->refrection,
        &unifiedShader->descriptorLayout(),
    };
    //if (!createInfo.vertexShaderRefrection) {
    //    createInfo.vertexShaderRefrection = makeRef<detail::UnifiedShaderRefrectionInfo>();
    //}
    //if (!createInfo.pixelShaderRefrection) {
    //    createInfo.pixelShaderRefrection = makeRef<detail::UnifiedShaderRefrectionInfo>();
    //}
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

void IGraphicsDeviceContext::commitStatus()
{
	if (LN_REQUIRE(m_staging.framebufferState.renderTargets[0])) return;
    //if (LN_REQUIRE(m_staging.pipelineState.vertexDeclaration)) return;



	// TODO: modified check

	onUpdatePipelineState(m_staging.pipelineState.blendState, m_staging.pipelineState.rasterizerState, m_staging.pipelineState.depthStencilState);

	onUpdateShaderPass(m_staging.shaderPass);

	onUpdateFrameBuffers(m_staging.framebufferState.renderTargets.data(), m_staging.framebufferState.renderTargets.size(), m_staging.framebufferState.depthBuffer);

	onUpdateRegionRects(m_staging.regionRects.viewportRect, m_staging.regionRects.scissorRect, m_staging.framebufferState.renderTargets[0]->realSize());

	onUpdatePrimitiveData(m_staging.pipelineState.vertexDeclaration, m_staging.primitive.vertexBuffers.data(), m_staging.primitive.vertexBuffers.size(), m_staging.primitive.indexBuffer);
	
    onSubmitStatus(m_staging, m_stateDirtyFlags);

    m_committed = m_staging;
	m_stateDirtyFlags = StateDirtyFlags_None;
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
	dispose();
}

void IGraphicsDeviceObject::dispose()
{
	m_disposed = true;
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
// IShaderUniform

IShaderSamplerBuffer::IShaderSamplerBuffer()
{
	LN_LOG_VERBOSE << "IShaderSamplerBuffer [0x" << this << "] constructed.";
}

} // namespace detail
} // namespace ln

