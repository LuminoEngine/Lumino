
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

void IGraphicsDeviceContext::setBlendState(const BlendStateDesc& value)
{
	m_staging.pipelineState.blendState = value;
}

void IGraphicsDeviceContext::setRasterizerState(const RasterizerStateDesc& value)
{
	m_staging.pipelineState.rasterizerState = value;
}

void IGraphicsDeviceContext::setDepthStencilState(const DepthStencilStateDesc& value)
{
	m_staging.pipelineState.depthStencilState = value;
}

void IGraphicsDeviceContext::setColorBuffer(int index, ITexture* value)
{
	m_staging.framebufferState.renderTargets[index] = value;
}

void IGraphicsDeviceContext::setDepthBuffer(IDepthBuffer* value)
{
	m_staging.framebufferState.depthBuffer = value;
}

void IGraphicsDeviceContext::setViewportRect(const RectI& value)
{
	m_staging.viewportRect = value;
}

void IGraphicsDeviceContext::setScissorRect(const RectI& value)
{
	m_staging.scissorRect = value;
}

void IGraphicsDeviceContext::setVertexDeclaration(IVertexDeclaration* value)
{
	m_staging.pipelineState.vertexDeclaration = value;
}

void IGraphicsDeviceContext::setVertexBuffer(int streamIndex, IVertexBuffer* value)
{
	m_staging.vertexBuffers[streamIndex] = value;
}

void IGraphicsDeviceContext::setIndexBuffer(IIndexBuffer* value)
{
	m_staging.indexBuffer = value;
}

void IGraphicsDeviceContext::setShaderPass(IShaderPass* value)
{
	m_staging.pipelineState.shaderPass = value;
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

void IGraphicsDeviceContext::present(ISwapChain* swapChain)
{
	onPresent(swapChain);
}

void IGraphicsDeviceContext::commitStatus()
{
	if (LN_REQUIRE(m_staging.framebufferState.renderTargets[0])) return;

	// TODO: modified check

	onUpdatePipelineState(m_staging.pipelineState.blendState, m_staging.pipelineState.rasterizerState, m_staging.pipelineState.depthStencilState);

	onUpdateShaderPass(m_staging.pipelineState.shaderPass);

	onUpdateFrameBuffers(m_staging.framebufferState.renderTargets.data(), m_staging.framebufferState.renderTargets.size(), m_staging.framebufferState.depthBuffer);

	onUpdateRegionRects(m_staging.viewportRect, m_staging.scissorRect, m_staging.framebufferState.renderTargets[0]->realSize());

	onUpdatePrimitiveData(m_staging.pipelineState.vertexDeclaration, m_staging.vertexBuffers.data(), m_staging.vertexBuffers.size(), m_staging.indexBuffer);
	
    m_committed = m_staging;
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

