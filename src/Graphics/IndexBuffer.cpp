
#pragma once

#include "../Internal.h"
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/GraphicsManager.h>

namespace Lumino
{
namespace Graphics
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IndexBuffer* IndexBuffer::Create(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
{
	return Create(Internal::Manager, indexCount, initialData, format, usage);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IndexBuffer* IndexBuffer::Create(GraphicsManager* manager, int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
{
	LN_THROW(manager != NULL, ArgumentException);
	RefPtr<Device::IIndexBuffer> deviceObj(
		manager->GetGraphicsDevice()->GetDeviceObject()->CreateIndexBuffer(indexCount, initialData, format, usage));
	return LN_NEW IndexBuffer(deviceObj, indexCount, format);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IndexBuffer::IndexBuffer(Device::IIndexBuffer* deviceObj, int indexCount, IndexBufferFormat format)
	: m_deviceObj(deviceObj)
	, m_initialUpdate(true)
{
	LN_SAFE_ADDREF(m_deviceObj);

	int stride = (format == IndexBufferFormat_UInt16) ? 2 : 4;
	m_data.Attach(LN_NEW ByteBuffer(stride * indexCount));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
	LN_SAFE_RELEASE(m_deviceObj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ByteBuffer* IndexBuffer::Lock()
{
	// まだ1度も SetVertexBufferCommand に入っていない場合は直接 Lock で書き換えできる
	if (m_initialUpdate) {
		void* buffer;
		size_t size;
		m_deviceObj->Lock(&buffer, &size);
		m_lockedBuffer.Attach(buffer, size);
	}
	else {
		LN_THROW(0, NotImplementedException);
	}
	return &m_lockedBuffer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IndexBuffer::Unlock()
{
	if (m_initialUpdate) {
		m_deviceObj->Unlock();
	}
	else {
		LN_THROW(0, NotImplementedException);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void IndexBuffer::SetSubData(uint32_t offsetBytes, void* data, uint32_t dataBytes)
//{
//
//}


} // namespace Graphics
} // namespace Lumino
