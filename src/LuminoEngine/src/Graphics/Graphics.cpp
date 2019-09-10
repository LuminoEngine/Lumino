
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include <LuminoEngine/Graphics/CommandQueue.hpp>
#include <LuminoEngine/Graphics/Graphics.hpp>

namespace ln {

//==============================================================================
// Graphics

CommandQueue* Graphics::graphicsQueue()
{
	return detail::EngineDomain::graphicsManager()->graphicsQueue();
}

CommandQueue* Graphics::computeQueue()
{
	return detail::EngineDomain::graphicsManager()->computeQueue();
}

} // namespace ln
