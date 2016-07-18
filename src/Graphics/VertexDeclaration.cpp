
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
VertexDeclarationPtr VertexDeclaration::Create()
{
	auto ptr = VertexDeclarationPtr::MakeRef();
	ptr->Initialize(GraphicsManager::GetInstance());
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
void VertexDeclaration::Initialize(GraphicsManager* manager)
{
	GraphicsResourceObject::Initialize(manager);
}

//------------------------------------------------------------------------------
void VertexDeclaration::Initialize(GraphicsManager* manager, const VertexElement* elements, int count)
{
	GraphicsResourceObject::Initialize(manager);

	for (int i = 0; i < count; ++i)
	{
		m_vertexElements.Add(elements[i]);
	}
}

//------------------------------------------------------------------------------
void VertexDeclaration::AddVertexElement(int streamIndex, VertexElementType type, VertexElementUsage usage, int usageIndex)
{
	LN_CHECK_ARG(streamIndex >= 0);
	LN_CHECK_ARG(usageIndex >= 0);

	VertexElement e;
	e.StreamIndex = streamIndex;
	e.Type = type;
	e.Usage = usage;
	e.UsageIndex = usageIndex;
	m_vertexElements.Add(e);
}

//------------------------------------------------------------------------------
void VertexDeclaration::TryUpdateResource()
{
	if (m_modified)
	{
		m_deviceObj.Attach(m_manager->GetGraphicsDevice()->CreateVertexDeclaration(&m_vertexElements[0], m_vertexElements.GetCount()), false);
		m_modified = false;
	}
}

//------------------------------------------------------------------------------
Driver::IVertexDeclaration* VertexDeclaration::GetDeviceObject()
{
	TryUpdateResource();
	return m_deviceObj;
}

//------------------------------------------------------------------------------
void VertexDeclaration::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		// 破棄
		m_deviceObj.SafeRelease();
	}
	else
	{
		assert(m_deviceObj == NULL);

		// 作り直す
		m_deviceObj.Attach(m_manager->GetGraphicsDevice()->CreateVertexDeclaration(&m_vertexElements[0], m_vertexElements.GetCount()), false);
		m_modified = false;
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
