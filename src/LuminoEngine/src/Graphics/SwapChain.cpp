
#include "Internal.hpp"
#include <Lumino/Graphics/SwapChain.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {

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
	m_rhiObject.safeRelease();
}

} // namespace ln

