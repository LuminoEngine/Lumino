
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/GraphicsCommandBuffer.hpp>

namespace ln {

//==============================================================================
// GraphicsCommandBuffer

GraphicsCommandBuffer::GraphicsCommandBuffer()
    : m_manager(nullptr)
    , m_rhiObject()
{
}

GraphicsCommandBuffer::~GraphicsCommandBuffer()
{
}

void GraphicsCommandBuffer::init()
{
    Object::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
}

void GraphicsCommandBuffer::onDispose(bool explicitDisposing)
{
    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    Object::onDispose(explicitDisposing);
}

void GraphicsCommandBuffer::onChangeDevice(detail::IGraphicsDevice* device)
{
}

detail::ICommandList* GraphicsCommandBuffer::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
	return nullptr;
}

} // namespace ln
