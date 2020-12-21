
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/GraphicsCommandBuffer.hpp>
#include "SingleFrameAllocator.hpp"

namespace ln {

#if 0
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

    m_singleFrameUniformBufferAllocator = makeRef<detail::SingleFrameUniformBufferAllocator>(m_manager->singleFrameUniformBufferAllocatorPageManager());
}

void GraphicsCommandBuffer::onDispose(bool explicitDisposing)
{
    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    Object::onDispose(explicitDisposing);
}

detail::UniformBufferView GraphicsCommandBuffer::allocateUniformBuffer(size_t size)
{
    return m_singleFrameUniformBufferAllocator->allocate(size);
}

void GraphicsCommandBuffer::onChangeDevice(detail::IGraphicsDevice* device)
{
}

detail::ICommandList* GraphicsCommandBuffer::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
	return nullptr;
}
#endif

} // namespace ln
