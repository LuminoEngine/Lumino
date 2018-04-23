
#include "Internal.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {

//=============================================================================
// GraphicsDeviceContext

GraphicsDeviceContext::GraphicsDeviceContext()
{
}

void GraphicsDeviceContext::initialize()
{
}

void GraphicsDeviceContext::dispose()
{
}

Ref<ISwapChain> GraphicsDeviceContext::createSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	return onCreateSwapChain(window, backbufferSize);
}

void GraphicsDeviceContext::clearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	onClearBuffers(flags, color, z, stencil);
}

} // namespace detail
} // namespace ln

