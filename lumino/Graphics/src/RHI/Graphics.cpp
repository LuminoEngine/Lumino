#include "Internal.hpp"
#include <LuminoGraphics/detail/GraphicsManager.hpp>
#include <LuminoGraphics/RHI/CommandQueue.hpp>
#include <LuminoGraphics/RHI/Graphics.hpp>
#include "Backend/GraphicsDeviceContext.hpp"

namespace ln {

//==============================================================================
// Graphics

GraphicsAPI Graphics::activeGraphicsAPI()
{
	return detail::GraphicsManager::instance()->deviceContext()->caps().graphicsAPI;
}

CommandQueue* Graphics::graphicsQueue()
{
	return detail::GraphicsManager::instance()->graphicsQueue();
}

CommandQueue* Graphics::computeQueue()
{
	return detail::GraphicsManager::instance()->computeQueue();
}

int Graphics::registerExtension(INativeGraphicsExtension* extension)
{
	return detail::GraphicsManager::instance()->registerExtension(extension);
}

void Graphics::unregisterExtension(INativeGraphicsExtension* extension)
{
	return detail::GraphicsManager::instance()->unregisterExtension(extension);
}

} // namespace ln
