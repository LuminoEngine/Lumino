﻿
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RHIs/GraphicsDeviceContext.hpp"
#include "RenderTargetTextureCache.hpp"
#include <LuminoEngine/Graphics/DepthBuffer.hpp>
#include "GraphicsProfiler.hpp"

namespace ln {

//==============================================================================
// DepthBuffer

Ref<DepthBuffer> DepthBuffer::create(int width, int height)
{
    return makeObject<DepthBuffer>(width, height);
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
    : m_manager(nullptr)
    , m_rhiObject()
    , m_size()
{
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
    detail::GraphicsResourceInternal::manager(this)->profiler()->addDepthBuffer(this);
}

DepthBuffer::~DepthBuffer()
{
    detail::GraphicsResourceInternal::manager(this)->profiler()->removeDepthBuffer(this);
    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
}

void DepthBuffer::init(int width, int height)
{
    Object::init();

    m_size.width = width;
    m_size.height = height;
    m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createDepthBuffer(width, height);
}

void DepthBuffer::onDispose(bool explicitDisposing)
{
    m_rhiObject = nullptr;

    Object::onDispose(explicitDisposing);
}

void DepthBuffer::onChangeDevice(detail::IGraphicsDevice* device)
{
    if (!device) {
        m_rhiObject = nullptr;
    } else {
        m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createDepthBuffer(m_size.width, m_size.height);
    }
}

detail::IDepthBuffer* DepthBuffer::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
	*outModified = false;
    return m_rhiObject;
}

} // namespace ln
