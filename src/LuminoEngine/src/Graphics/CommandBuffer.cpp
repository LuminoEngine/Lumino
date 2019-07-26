
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/CommandBuffer.hpp>

namespace ln {

//==============================================================================
// CommandBuffer

CommandBuffer::CommandBuffer()
    : m_rhiObject()
{
}

CommandBuffer::~CommandBuffer()
{
}

void CommandBuffer::init()
{
    GraphicsResource::init();
}

void CommandBuffer::onDispose(bool explicitDisposing)
{
    GraphicsResource::onDispose(explicitDisposing);
}

void CommandBuffer::onChangeDevice(detail::IGraphicsDevice* device)
{
}

detail::ICommandList* CommandBuffer::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
	return nullptr;
}

} // namespace ln
