#include "Internal.hpp"
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/GPU/CommandQueue.hpp>
#include <LuminoEngine/GPU/Graphics.hpp>
#include <LuminoEngine/GraphicsRHI/GraphicsDeviceContext.hpp>

namespace ln {

//==============================================================================
// Graphics

GraphicsAPI Graphics::activeGraphicsAPI()
{
	return detail::GraphicsManager::instance()->deviceContext()->caps().graphicsAPI;
}
//
//CommandQueue* Graphics::graphicsQueue()
//{
//	return detail::GraphicsManager::instance()->graphicsQueue();
//}
//
//CommandQueue* Graphics::computeQueue()
//{
//	return detail::GraphicsManager::instance()->computeQueue();
//}

int Graphics::registerExtension(INativeGraphicsExtension* extension)
{
	return detail::GraphicsManager::instance()->registerExtension(extension);
}

void Graphics::unregisterExtension(INativeGraphicsExtension* extension)
{
	return detail::GraphicsManager::instance()->unregisterExtension(extension);
}

} // namespace ln
