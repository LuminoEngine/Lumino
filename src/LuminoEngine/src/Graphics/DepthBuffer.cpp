
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include "RenderTargetTextureCache.hpp"
#include <LuminoEngine/Graphics/DepthBuffer.hpp>

namespace ln {

//==============================================================================
// DepthBuffer

Ref<DepthBuffer> DepthBuffer::create(int width, int height)
{
    return newObject<DepthBuffer>(width, height);
}

Ref<DepthBuffer> DepthBuffer::getTemporary(int width, int height)
{
    return detail::EngineDomain::graphicsManager()->frameBufferCache()->requestDepthBuffer2(SizeI(width, height));
}

void DepthBuffer::releaseTemporary(DepthBuffer* depthBuffer)
{
    detail::EngineDomain::graphicsManager()->frameBufferCache()->release(depthBuffer);
}

DepthBuffer::DepthBuffer()
    : m_rhiObject()
    , m_size()
{
}

DepthBuffer::~DepthBuffer()
{
}

void DepthBuffer::init(int width, int height)
{
    GraphicsResource::init();
    m_size.width = width;
    m_size.height = height;
    m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createDepthBuffer(width, height);
}

void DepthBuffer::onDispose(bool explicitDisposing)
{
    m_rhiObject = nullptr;
    GraphicsResource::onDispose(explicitDisposing);
}

void DepthBuffer::onChangeDevice(detail::IGraphicsDevice* device)
{
    if (!device) {
        m_rhiObject = nullptr;
    } else {
        m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createDepthBuffer(m_size.width, m_size.height);
    }
}

detail::IDepthBuffer* DepthBuffer::resolveRHIObject(bool* outModified)
{
	*outModified = false;
    return m_rhiObject;
}

} // namespace ln
