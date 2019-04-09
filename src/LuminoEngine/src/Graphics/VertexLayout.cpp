
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "../Engine/RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>

namespace ln {

//==============================================================================
// VertexLayout

Ref<VertexLayout> VertexLayout::create()
{
	return newObject<VertexLayout>();
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

	for (int i = 0; i < count; ++i)
	{
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

detail::IVertexDeclaration* VertexLayout::resolveRHIObject()
{
	if (m_modified)
	{
		m_deviceObj = manager()->deviceContext()->createVertexDeclaration(&m_vertexElements[0], m_vertexElements.size());
		m_modified = false;
	}

	return m_deviceObj;
}

void VertexLayout::onChangeDevice(detail::IGraphicsDevice* device)
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
