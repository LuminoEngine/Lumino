#include "Internal.hpp"
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/GraphicsRHI/GraphicsDeviceContext.hpp>
#include <LuminoEngine/GPU/detail/GraphicsObjectRegistry.hpp>
#include <LuminoEngine/GPU/SwapChain.hpp>
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/GPU/VertexLayout.hpp>

namespace ln {

//==============================================================================
// VertexLayout

Ref<VertexLayout> VertexLayout::create() {
    return makeObject_deprecated<VertexLayout>();
}

VertexLayout::VertexLayout()
    : m_manager(nullptr)
    //, m_deviceObj(nullptr)
    , m_vertexElements()
    , m_modified(true) {
}

VertexLayout::~VertexLayout() {
}

void VertexLayout::init() {
    Object::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
    m_manager->resourceRegistry()->registerObject(this);
}

void VertexLayout::init(const VertexElement* elements, int count) {
    init();
    if (LN_REQUIRE(elements)) return;
    if (LN_REQUIRE(count >= 1)) return;

    for (int i = 0; i < count; ++i) {
        m_vertexElements.add(elements[i]);
    }
}

void VertexLayout::onDispose(bool explicitDisposing) {
    //m_deviceObj = nullptr;

    if (m_manager) {
        m_manager->resourceRegistry()->unregisterObject(this);
    }
    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    Object::onDispose(explicitDisposing);
}

void VertexLayout::addElement(int streamIndex, VertexElementType type, VertexElementUsage usage, int usageIndex, VertexInputRate rate) {
    if (LN_REQUIRE(streamIndex >= 0)) return;
    if (LN_REQUIRE(usageIndex >= 0)) return;

    VertexElement e;
    e.StreamIndex = streamIndex;
    e.Type = type;
    e.Usage = usage;
    e.UsageIndex = usageIndex;
    e.rate = rate;
    m_vertexElements.add(e);
}

void VertexLayout::addElement(const VertexElement& element) {
    m_vertexElements.add(element);
}

detail::IVertexDeclaration* VertexLayout::resolveRHIObject(GraphicsCommandList* context, bool* outModified) {
    GraphicsContext* graphicsContext = context->graphicsContext();
    detail::IVertexDeclaration* rhiObject = static_cast<detail::IVertexDeclaration*>(graphicsContext->rhiResourceRegistry()->get(this));
    *outModified = m_modified;

    if (m_modified) {
        detail::IGraphicsDevice* device = graphicsContext->rhiDevice();
        Ref<detail::IVertexDeclaration> ref = device->createVertexDeclaration(&m_vertexElements[0], m_vertexElements.size());
        graphicsContext->rhiResourceRegistry()->registerObject(this, ref);
        rhiObject = ref;
    }

    if (LN_ENSURE(rhiObject)) return nullptr;

    m_modified = false;
    return rhiObject;
}

void VertexLayout::onChangeDevice(detail::IGraphicsDevice* device) {
    if (!device) {
        //m_deviceObj = nullptr;
        m_modified = true;
    }
    else {
        //m_deviceObj = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createVertexDeclaration(&m_vertexElements[0], m_vertexElements.size());
        m_modified = false;
    }
}

} // namespace ln
