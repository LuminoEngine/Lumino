
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include <LuminoEngine/Graphics/CommandQueue.hpp>
#include <LuminoEngine/Graphics/Graphics.hpp>
#include "RHIs/GraphicsDeviceContext.hpp"

namespace ln {

//==============================================================================
// Graphics

GraphicsAPI Graphics::activeGraphicsAPI()
{
	return detail::EngineDomain::graphicsManager()->deviceContext()->caps().graphicsAPI;
}

CommandQueue* Graphics::graphicsQueue()
{
	return detail::EngineDomain::graphicsManager()->graphicsQueue();
}

CommandQueue* Graphics::computeQueue()
{
	return detail::EngineDomain::graphicsManager()->computeQueue();
}

int Graphics::registerExtension(INativeGraphicsExtension* extension)
{
	return detail::EngineDomain::graphicsManager()->registerExtension(extension);
}

void Graphics::unregisterExtension(INativeGraphicsExtension* extension)
{
	return detail::EngineDomain::graphicsManager()->unregisterExtension(extension);
}

} // namespace ln
