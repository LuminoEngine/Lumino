
#include "Internal.hpp"
#include "Backend/GraphicsDeviceContext.hpp"
#include <LuminoGraphics/RHI/CommandQueue.hpp>

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
