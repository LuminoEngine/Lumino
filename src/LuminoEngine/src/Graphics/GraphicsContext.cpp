
#include "Internal.hpp"
#include <Lumino/Graphics/GraphicsContext.hpp>
#include <Lumino/Graphics/VertexDeclaration.hpp>
#include <Lumino/Graphics/VertexBuffer.hpp>
#include <Lumino/Graphics/IndexBuffer.hpp>
#include <Lumino/Graphics/Texture.hpp>
#include <Lumino/Shader/Shader.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include "RenderingCommandList.hpp"

namespace ln {

//==============================================================================
// SwapChain

SwapChain::SwapChain()
	: m_rhiObject(nullptr)
	, m_colorBuffer(nullptr)
{
}

SwapChain::~SwapChain()
{
}

void SwapChain::initialize(detail::PlatformWindow* window, const SizeI& backbufferSize)
{
	m_rhiObject = detail::EngineDomain::graphicsManager()->deviceContext()->createSwapChain(window, backbufferSize);
	m_colorBuffer = newObject<RenderTargetTexture>(m_rhiObject->getColorBuffer());
}

void SwapChain::dispose()
{
	m_rhiObject = nullptr;
}

RenderTargetTexture* SwapChain::colorBuffer() const
{
	return m_colorBuffer;
}

void SwapChain::wait()
{
	// TODO
}

detail::ISwapChain* SwapChain::resolveRHIObject() const
{
	return m_rhiObject;
}

//==============================================================================
// GraphicsContext

GraphicsContext::GraphicsContext()
	: m_device(nullptr)
{
}

GraphicsContext::~GraphicsContext()
{
}

void GraphicsContext::initialize(detail::IGraphicsDeviceContext* device)
{
	m_manager = detail::EngineDomain::graphicsManager();
	m_device = device;
}

void GraphicsContext::dispose()
{
}

void GraphicsContext::setColorBuffer(int index, RenderTargetTexture* value)
{
	detail::ITexture* rhiObject = (value) ? value->resolveRHIObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_3(
		GraphicsContext_setDepthBuffer, m_manager,
		detail::IGraphicsDeviceContext*, m_device,
		int, index,
		detail::ITexture*, rhiObject,
		{
			m_device->setColorBuffer(index, rhiObject);
		});
}

void GraphicsContext::setDepthBuffer(DepthBuffer* value)
{
	detail::IDepthBuffer* rhiObject = (value) ? value->resolveRHIObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_2(
		GraphicsContext_setDepthBuffer, m_manager,
		detail::IGraphicsDeviceContext*, m_device,
		detail::IDepthBuffer*, rhiObject,
		{
			m_device->setDepthBuffer(rhiObject);
		});
}

void GraphicsContext::setVertexDeclaration(VertexDeclaration* value)
{
	detail::IVertexDeclaration* rhiObject = (value) ? value->resolveRHIObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_2(
		GraphicsContext_setVertexDeclaration, m_manager,
		detail::IGraphicsDeviceContext*, m_device,
		detail::IVertexDeclaration*, rhiObject,
		{
			m_device->setVertexDeclaration(rhiObject);
		});
}

void GraphicsContext::setVertexBuffer(int streamIndex, VertexBuffer* value)
{
	detail::IVertexBuffer* rhiObject = (value) ? value->resolveRHIObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_3(
		GraphicsContext_setVertexBuffer, m_manager,
		detail::IGraphicsDeviceContext*, m_device,
		int, streamIndex,
		detail::IVertexBuffer*, rhiObject,
		{
			m_device->setVertexBuffer(streamIndex, rhiObject);
		});
}

void GraphicsContext::setIndexBuffer(IndexBuffer* value)
{
	detail::IIndexBuffer* rhiObject = (value) ? value->resolveRHIObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_2(
		GraphicsContext_setIndexBuffer, m_manager,
		detail::IGraphicsDeviceContext*, m_device,
		detail::IIndexBuffer*, rhiObject,
		{
			m_device->setIndexBuffer(rhiObject);
		});
}

void GraphicsContext::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	// TODO: threading
	m_device->clearBuffers(flags, color, z, stencil);
}

void GraphicsContext::drawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
	LN_ENQUEUE_RENDER_COMMAND_4(
		GraphicsContext_setIndexBuffer, m_manager,
		detail::IGraphicsDeviceContext*, m_device,
		PrimitiveType, primitive,
		int, startVertex,
		int, primitiveCount,
		{
			m_device->drawPrimitive(primitive, startVertex, primitiveCount);
		});
}

void GraphicsContext::drawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
	LN_ENQUEUE_RENDER_COMMAND_4(
		GraphicsContext_setIndexBuffer, m_manager,
		detail::IGraphicsDeviceContext*, m_device,
		PrimitiveType, primitive,
		int, startIndex,
		int, primitiveCount,
		{
			m_device->drawPrimitiveIndexed(primitive, startIndex, primitiveCount);
		});
}

void GraphicsContext::setShaderPass(ShaderPass* pass)
{
	// TODO: threading
	if (pass)
	{
		pass->commit();
	}
}

void GraphicsContext::present(SwapChain* swapChain)
{
	if (LN_REQUIRE(swapChain)) return;

	// TODO: threading
	m_device->present(swapChain->resolveRHIObject());
}

} // namespace ln

