
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "Backend/GraphicsDeviceContext.hpp"
#include <LuminoGraphics/RHI/VertexLayout.hpp>

namespace ln {

//==============================================================================
// VertexLayout

Ref<VertexLayout> VertexLayout::create()
{
    return makeObject<VertexLayout>();
}

VertexLayout::VertexLayout()
    : m_manager(nullptr)
    , m_deviceObj(nullptr)
    , m_vertexElements()
    , m_modified(true)
{
}

VertexLayout::~VertexLayout()
{
}

void VertexLayout::init()
{
    Object::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
}

void VertexLayout::init(const VertexElement* elements, int count)
{
    init();
    if (LN_REQUIRE(elements)) return;
    if (LN_REQUIRE(count >= 1)) return;

    for (int i = 0; i < count; ++i) {
        m_vertexElements.add(elements[i]);
    }
}

void VertexLayout::onDispose(bool explicitDisposing)
{
    m_deviceObj = nullptr;

    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    Object::onDispose(explicitDisposing);
}

void VertexLayout::addElement(int streamIndex, VertexElementType type, VertexElementUsage usage, int usageIndex, VertexInputRate rate)
{
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

void VertexLayout::addElement(const VertexElement& element)
{
    m_vertexElements.add(element);
}

detail::IVertexDeclaration* VertexLayout::resolveRHIObject(GraphicsCommandList* context, bool* outModified) {
	*outModified = m_modified;

    if (m_modified) {
        m_deviceObj = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createVertexDeclaration(&m_vertexElements[0], m_vertexElements.size());
        m_modified = false;
    }

    return m_deviceObj;
}

void VertexLayout::onChangeDevice(detail::IGraphicsDevice* device)
{
    if (!device) {
        m_deviceObj = nullptr;
    } else {
        m_deviceObj = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createVertexDeclaration(&m_vertexElements[0], m_vertexElements.size());
        m_modified = false;
    }
}

} // namespace ln
