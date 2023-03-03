
#include "Internal.hpp"
#include <LuminoGraphicsRHI/GraphicsDeviceContext.hpp>
#include <LuminoGraphics/GPU/CommandQueue.hpp>

namespace ln {

//==============================================================================
// CommandQueue

CommandQueue::CommandQueue()
    : m_rhiObject()
{
}

void CommandQueue::init(URef<detail::ICommandQueue> rhiObject)
{
    Object::init();
	m_rhiObject = std::move(rhiObject);
}

} // namespace ln
