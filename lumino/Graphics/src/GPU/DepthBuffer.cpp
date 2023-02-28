#include "Internal.hpp"
#include <LuminoGraphics/detail/GraphicsManager.hpp>
#include <LuminoGraphicsRHI/GraphicsDeviceContext.hpp>
#include "RenderTargetTextureCache.hpp"
#include <LuminoGraphics/GPU/DepthBuffer.hpp>
#include "GraphicsProfiler.hpp"

namespace ln {

//==============================================================================
// DepthBuffer

Ref<DepthBuffer> DepthBuffer::create(int width, int height) {
    return makeObject_deprecated<DepthBuffer>(width, height);
}

Ref<DepthBuffer> DepthBuffer::getTemporary(int width, int height) {
    return detail::GraphicsManager::instance()->frameBufferCache()->requestDepthBuffer2(SizeI(width, height));
}

void DepthBuffer::releaseTemporary(DepthBuffer* depthBuffer) {
    detail::GraphicsManager::instance()->frameBufferCache()->release(depthBuffer);
}

DepthBuffer::DepthBuffer()
    : m_manager(nullptr)
    , m_rhiObject()
    , m_size() {
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
    detail::GraphicsResourceInternal::manager(this)->profiler()->addDepthBuffer(this);
}

DepthBuffer::~DepthBuffer() {
    detail::GraphicsResourceInternal::manager(this)->profiler()->removeDepthBuffer(this);
    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
}

void DepthBuffer::init(int width, int height) {
    Object::init();

    m_size.width = width;
    m_size.height = height;
    m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createDepthBuffer(width, height);
}

void DepthBuffer::onDispose(bool explicitDisposing) {
    m_rhiObject = nullptr;

    Object::onDispose(explicitDisposing);
}

void DepthBuffer::onChangeDevice(detail::IGraphicsDevice* device) {
    if (!device) {
        m_rhiObject = nullptr;
    }
    else {
        m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createDepthBuffer(m_size.width, m_size.height);
    }
}

detail::RHIResource* DepthBuffer::resolveRHIObject(GraphicsCommandList* context, bool* outModified) {
    *outModified = false;
    return m_rhiObject;
}

} // namespace ln
