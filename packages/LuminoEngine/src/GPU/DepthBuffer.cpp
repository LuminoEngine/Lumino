#include "Internal.hpp"
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/GraphicsDeviceContext.hpp>
#include "RenderTargetTextureCache.hpp"
#include <LuminoEngine/GPU/detail/GraphicsObjectRegistry.hpp>
#include <LuminoEngine/GPU/SwapChain.hpp>
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/GPU/DepthBuffer.hpp>
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
    //, m_rhiObject()
    , m_size() {
}

DepthBuffer::~DepthBuffer() {
}

void DepthBuffer::init(int width, int height) {
    Object::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
    detail::GraphicsResourceInternal::manager(this)->profiler()->addDepthBuffer(this);
    m_manager->resourceRegistry()->registerObject(this);

    m_size.width = width;
    m_size.height = height;
    //m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createDepthBuffer(width, height);
}

void DepthBuffer::onDispose(bool explicitDisposing) {
    //m_rhiObject = nullptr;

    if (m_manager) {
        m_manager->resourceRegistry()->unregisterObject(this);
        detail::GraphicsResourceInternal::manager(this)->profiler()->removeDepthBuffer(this);
        detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    }
    Object::onDispose(explicitDisposing);
}

void DepthBuffer::onChangeDevice(detail::IGraphicsDevice* device) {
    if (!device) {
        //m_rhiObject = nullptr;
    }
    else {
        //m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createDepthBuffer(m_size.width, m_size.height);
    }
}

detail::RHIResource* DepthBuffer::resolveRHIObject(GraphicsCommandList* context, bool* outModified) {
    GraphicsContext* graphicsContext = context->graphicsContext();
    detail::RHIResource* rhiObject = static_cast<detail::RHIResource*>(graphicsContext->rhiResourceRegistry()->get(this));
    *outModified = false;
    if (!rhiObject) {
        detail::IGraphicsDevice* device = graphicsContext->rhiDevice();
        Ref<detail::RHIResource> ref = device->createDepthBuffer(m_size.width, m_size.height);
        graphicsContext->rhiResourceRegistry()->registerObject(this, ref);
        rhiObject = ref;
        *outModified = true;
    }

    if (LN_ENSURE(rhiObject)) return nullptr;
    return rhiObject;
}

} // namespace ln
