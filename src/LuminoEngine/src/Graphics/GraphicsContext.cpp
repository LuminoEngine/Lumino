
#include "Internal.hpp"
#include <Lumino/Graphics/GraphicsContext.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {

//==============================================================================
// SwapChain

SwapChain::SwapChain()
{
}

SwapChain::~SwapChain()
{
}

void SwapChain::initialize(detail::PlatformWindow* window, const SizeI& backbufferSize)
{
	m_rhiObject = detail::EngineDomain::graphicsManager()->deviceContext()->createSwapChain(window, backbufferSize);
}

void SwapChain::dispose()
{
	m_rhiObject.reset();
}

detail::ISwapChain* SwapChain::resolveRHIObject() const
{
	return m_rhiObject;
}

//==============================================================================
// GraphicsContext

void GraphicsContext::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	// TODO: threading
	m_rhiObject->clearBuffers(flags, color, z, stencil);
}

void GraphicsContext::present(SwapChain* swapChain)
{
	if (LN_REQUIRE(swapChain)) return;

	// TODO: threading
	m_rhiObject->present(swapChain->resolveRHIObject());
}

GraphicsContext::GraphicsContext()
	: m_rhiObject(nullptr)
{
}

GraphicsContext::~GraphicsContext()
{
}

void GraphicsContext::initialize(detail::IGraphicsDeviceContext* device)
{
	m_rhiObject = device;
}

void GraphicsContext::dispose()
{
}

} // namespace ln

