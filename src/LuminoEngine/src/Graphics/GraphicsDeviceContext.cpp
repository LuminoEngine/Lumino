
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
	//onSetShaderPass(pass);
	LN_NOTIMPLEMENTED();
}

void IGraphicsDeviceContext::clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	onClearBuffers(flags, color, z, stencil);
}

void IGraphicsDeviceContext::present(ISwapChain* swapChain)
{
	onPresent(swapChain);
}

} // namespace detail
} // namespace ln

