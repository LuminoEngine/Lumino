
#pragma once
#include "../Internal.h"
#include <Lumino/Graphics/VertexDeclaration.h>
#include "GraphicsManager.h"
#include "Device/GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// VertexDeclaration
//==============================================================================

//------------------------------------------------------------------------------
VertexDeclarationPtr VertexDeclaration::create()
{
	auto ptr = VertexDeclarationPtr::makeRef();
	ptr->initialize(detail::GraphicsManager::getInstance());
	return ptr;
}

//------------------------------------------------------------------------------
VertexDeclaration::VertexDeclaration()
	: m_deviceObj(nullptr)
	, m_vertexElements()
	, m_modified(true)
{
}

//------------------------------------------------------------------------------
VertexDeclaration::~VertexDeclaration()
{
}

//------------------------------------------------------------------------------
bool VertexDeclaration::initialize(detail::GraphicsManager* manager)
{
	return GraphicsResourceObject::initialize();
}

//------------------------------------------------------------------------------
bool VertexDeclaration::initialize(detail::GraphicsManager* manager, const VertexElement* elements, int count)
{
	LN_BASE_INITIALIZE(GraphicsResourceObject);

	for (int i = 0; i < count; ++i)
	{
		m_vertexElements.add(elements[i]);
	}
	return true;
}

//------------------------------------------------------------------------------
void VertexDeclaration::Dispose()
{
	m_deviceObj.safeRelease();
	GraphicsResourceObject::Dispose();
}

//------------------------------------------------------------------------------
void VertexDeclaration::addVertexElement(int streamIndex, VertexElementType type, VertexElementUsage usage, int usageIndex)
{
	if (LN_CHECK_ARG(streamIndex >= 0)) return;
	if (LN_CHECK_ARG(usageIndex >= 0)) return;

	VertexElement e;
	e.StreamIndex = streamIndex;
	e.Type = type;
	e.Usage = usage;
	e.UsageIndex = usageIndex;
	m_vertexElements.add(e);
}

//------------------------------------------------------------------------------
void VertexDeclaration::tryUpdateResource()
{
	if (m_modified)
	{
		m_deviceObj.attach(m_manager->getGraphicsDevice()->createVertexDeclaration(&m_vertexElements[0], m_vertexElements.getCount()), false);
		m_modified = false;
	}
}

//------------------------------------------------------------------------------
Driver::IVertexDeclaration* VertexDeclaration::getDeviceObject()
{
	tryUpdateResource();
	return m_deviceObj;
}

//------------------------------------------------------------------------------
void VertexDeclaration::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		// 破棄
		m_deviceObj.safeRelease();
	}
	else
	{
		assert(m_deviceObj == NULL);

		// 作り直す
		m_deviceObj.attach(m_manager->getGraphicsDevice()->createVertexDeclaration(&m_vertexElements[0], m_vertexElements.getCount()), false);
		m_modified = false;
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
