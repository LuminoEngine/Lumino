
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

void CommandQueue::init(detail::ICommandQueue* rhiObject)
{
    Object::init();
	m_rhiObject = rhiObject;
}

} // namespace ln
