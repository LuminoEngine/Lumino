
#pragma once

#include "../Internal.h"
#include "../../include/Lumino/Graphics/VertexBuffer.h"
#include "../../include/Lumino/Graphics/GraphicsManager.h"

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// VertexBuffer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VertexBuffer* VertexBuffer::Create(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage)
{
	return Create(Internal::Manager, vertexElements, elementsCount, vertexCount, data, usage);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VertexBuffer* VertexBuffer::Create(GraphicsManager* manager, const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage)
{
	LN_THROW(manager != NULL, ArgumentException);
	RefPtr<Device::IVertexBuffer> deviceObj(
		manager->GetGraphicsDevice()->GetDeviceObject()->CreateVertexBuffer(vertexElements, elementsCount, vertexCount, data, usage));
	return LN_NEW VertexBuffer(deviceObj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VertexBuffer::VertexBuffer(Device::IVertexBuffer* deviceObj)
	: m_deviceObj(deviceObj)
{
	LN_SAFE_ADDREF(m_deviceObj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
	LN_SAFE_RELEASE(m_deviceObj);
}


} // namespace Graphics
} // namespace Lumino
