
#include "Internal.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {

//=============================================================================
// IGraphicsDeviceContext

IGraphicsDeviceContext::IGraphicsDeviceContext()
{
}

void IGraphicsDeviceContext::initialize()
{
}

void IGraphicsDeviceContext::dispose()
{
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

Ref<ITexture> IGraphicsDeviceContext::createRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
	return onCreateRenderTarget(width, height, requestFormat, mipmap);
}

Ref<IShaderPass> IGraphicsDeviceContext::createShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* fsCodeLen, int psCodeLen, ShaderCompilationDiag* diag)
{
	diag->level = ShaderCompilationResultLevel::Success;
	diag->message.clear();

	auto pass = onCreateShaderPass(vsCode, vsCodeLen, fsCodeLen, psCodeLen, diag);

	if (!diag->message.empty()) {
		LN_LOG_VERBOSE << diag->message;
	}

	return pass;
}

void IGraphicsDeviceContext::setColorBuffer(int index, ITexture* value)
{
	m_staging.renderTargets[index] = value;
}

void IGraphicsDeviceContext::setDepthBuffer(IDepthBuffer* value)
{
	LN_NOTIMPLEMENTED();
	m_staging.depthBuffer = value;
}

void IGraphicsDeviceContext::setVertexDeclaration(IVertexDeclaration* value)
{
	m_staging.vertexDeclaration = value;
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
	m_staging.shaderPass = value;
}

void IGraphicsDeviceContext::clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	commitStatus();
	onClearBuffers(flags, color, z, stencil);
}

void IGraphicsDeviceContext::drawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
	commitStatus();
	onDrawPrimitive(primitive, startVertex, primitiveCount);
}

void IGraphicsDeviceContext::drawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
	commitStatus();
	onDrawPrimitiveIndexed(primitive, startIndex, primitiveCount);
}

void IGraphicsDeviceContext::present(ISwapChain* swapChain)
{
	onPresent(swapChain);
}

void IGraphicsDeviceContext::commitStatus()
{
	if (LN_REQUIRE(m_staging.renderTargets[0])) return;

	// TODO: modified check

	onUpdateFrameBuffers(m_staging.renderTargets.data(), m_staging.renderTargets.size(), nullptr);

	onUpdatePrimitiveData(m_staging.vertexDeclaration, m_staging.vertexBuffers.data(), m_staging.vertexBuffers.size(), m_staging.indexBuffer);
	// TODO: Shadr
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
		LN_LOG_ERROR << "object [" << this << "] is not disposed";
	}
}

void IGraphicsDeviceObject::dispose()
{
	m_disposed = true;
}


} // namespace detail
} // namespace ln

