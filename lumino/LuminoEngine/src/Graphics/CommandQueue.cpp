
#include "Internal.hpp"
#include "RHIs/GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/CommandQueue.hpp>

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
