
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/GraphicsCommandBuffer.hpp>

namespace ln {

//==============================================================================
// GraphicsCommandBuffer

GraphicsCommandBuffer::GraphicsCommandBuffer()
    : m_rhiObject()
{
}

GraphicsCommandBuffer::~GraphicsCommandBuffer()
{
}

void GraphicsCommandBuffer::init()
{
    GraphicsResource::init();
}

void GraphicsCommandBuffer::onDispose(bool explicitDisposing)
{
    GraphicsResource::onDispose(explicitDisposing);
}

void GraphicsCommandBuffer::onChangeDevice(detail::IGraphicsDevice* device)
{
}

detail::ICommandList* GraphicsCommandBuffer::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
	return nullptr;
}

} // namespace ln
