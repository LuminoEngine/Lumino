
#include "Internal.hpp"
#include "../Engine/RenderingCommandList.hpp"
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>

namespace ln {

//==============================================================================
// VertexLayout

Ref<VertexLayout> VertexLayout::create()
{
    return makeObject<VertexLayout>();
}

VertexLayout::VertexLayout()
    : m_deviceObj(nullptr)
    , m_vertexElements()
    , m_modified(true)
{
}

VertexLayout::~VertexLayout()
{
}

void VertexLayout::init()
{
    GraphicsResource::init();
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
    GraphicsResource::onDispose(explicitDisposing);
}

void VertexLayout::addElement(int streamIndex, VertexElementType type, VertexElementUsage usage, int usageIndex)
{
    if (LN_REQUIRE(streamIndex >= 0)) return;
    if (LN_REQUIRE(usageIndex >= 0)) return;

    VertexElement e;
    e.StreamIndex = streamIndex;
    e.Type = type;
    e.Usage = usage;
    e.UsageIndex = usageIndex;
    m_vertexElements.add(e);
}

detail::IVertexDeclaration* VertexLayout::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
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
