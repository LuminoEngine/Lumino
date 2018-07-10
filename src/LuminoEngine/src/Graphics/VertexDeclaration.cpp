
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <Lumino/Graphics/VertexDeclaration.hpp>

namespace ln {

//==============================================================================
// VertexDeclaration

Ref<VertexDeclaration> VertexDeclaration::create()
{
	return newObject<VertexDeclaration>();
}

VertexDeclaration::VertexDeclaration()
	: m_deviceObj(nullptr)
	, m_vertexElements()
	, m_modified(true)
{
}

VertexDeclaration::~VertexDeclaration()
{
}

void VertexDeclaration::initialize()
{
	GraphicsResource::initialize();
}

void VertexDeclaration::initialize(const VertexElement* elements, int count)
{
	initialize();

	for (int i = 0; i < count; ++i)
	{
		m_vertexElements.add(elements[i]);
	}
}

void VertexDeclaration::dispose()
{
	m_deviceObj = nullptr;

	GraphicsResource::dispose();
}

void VertexDeclaration::addVertexElement(int streamIndex, VertexElementType type, VertexElementUsage usage, int usageIndex)
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

detail::IVertexDeclaration* VertexDeclaration::resolveRHIObject()
{
	if (m_modified)
	{
		m_deviceObj = manager()->deviceContext()->createVertexDeclaration(&m_vertexElements[0], m_vertexElements.size());
		m_modified = false;
	}

	return m_deviceObj;
}

void VertexDeclaration::onChangeDevice(detail::IGraphicsDeviceContext* device)
{
	if (device)
	{
		m_deviceObj = manager()->deviceContext()->createVertexDeclaration(&m_vertexElements[0], m_vertexElements.size());
		m_modified = false;
	}
	else
	{
		m_deviceObj.reset();
	}
}

} // namespace ln
