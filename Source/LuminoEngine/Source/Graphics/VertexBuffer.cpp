
#pragma once
#include "../Internal.h"
#include <Lumino/Graphics/VertexBuffer.h>
#include "GraphicsManager.h"
#include "Device/GraphicsDriverInterface.h"
#include "RenderingCommand.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// VertexBuffer
//==============================================================================

////------------------------------------------------------------------------------
//VertexBuffer* VertexBuffer::Create(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage)
//{
//	return Create(GraphicsManager::GetInstance(), vertexElements, elementsCount, vertexCount, data, usage);
//}
//
////------------------------------------------------------------------------------
//VertexBuffer* VertexBuffer::Create(GraphicsManager* manager, const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage)
//{
//	LN_THROW(manager != NULL, ArgumentException);
//	return LN_NEW VertexBuffer(manager, vertexElements, elementsCount, vertexCount, data, usage);
//}

//------------------------------------------------------------------------------
VertexBuffer::VertexBuffer()
	: m_deviceObj(nullptr)
	, m_usage(ResourceUsage::Static)
	, m_pool(GraphicsResourcePool::Managed)	// TODO
	, m_initialUpdate(true)
	, m_locked(false)
	, m_rhiLockedBuffer(nullptr)
{
}

//------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
	LN_SAFE_RELEASE(m_deviceObj);
}

//------------------------------------------------------------------------------
void VertexBuffer::Initialize(detail::GraphicsManager* manager, size_t bufferSize, const void* data, ResourceUsage usage, bool sizeConst)
{
	GraphicsResourceObject::Initialize(manager);
	m_usage = usage;

	if (sizeConst)
	{
		m_deviceObj = m_manager->GetGraphicsDevice()->CreateVertexBuffer(bufferSize, data, usage);
	}
	else
	{
		m_buffer.resize(bufferSize);
	}
}

//------------------------------------------------------------------------------
int VertexBuffer::GetSize() const
{
	return static_cast<int>(m_buffer.size());
}

//------------------------------------------------------------------------------
void VertexBuffer::Reserve(int size)
{
	if (LN_CHECK_STATE(!IsRHIDirect())) return;		// サイズ変更禁止

	size_t newSize = static_cast<size_t>(size);
	if (newSize != m_buffer.capacity())
	{
		m_buffer.reserve(newSize);
	}
}

//------------------------------------------------------------------------------
void VertexBuffer::Resize(int size)
{
	if (LN_CHECK_STATE(!IsRHIDirect())) return;		// サイズ変更禁止

	size_t newSize = static_cast<size_t>(size);
	if (newSize != m_buffer.size())
	{
		m_buffer.resize(newSize);
	}
}

//------------------------------------------------------------------------------
void* VertexBuffer::GetMappedData()
{
	if (m_usage == ResourceUsage::Static)
	{
		// sizeConst で、まだ1度も SetVertexBufferCommand に入っていない場合は直接 Lock で書き換えできる
		if (m_initialUpdate && m_deviceObj != nullptr)
		{
			if (m_rhiLockedBuffer == nullptr)
			{
				m_rhiLockedBuffer = m_deviceObj->Lock();
			}
			m_locked = true;
			return m_rhiLockedBuffer;
		}
	}

	m_locked = true;
	return m_buffer.data();
}


////------------------------------------------------------------------------------
//ByteBuffer* VertexBuffer::GetMappedData()
//{
//	if (m_usage == ResourceUsage::Static)
//	{
//		// まだ1度も SetVertexBufferCommand に入っていない場合は直接 Lock で書き換えできる
//		if (m_initialUpdate) {
//			m_lockedBuffer.Attach(m_deviceObj->Lock(), m_deviceObj->GetByteCount());
//		}
//		else {
//			LN_THROW(0, NotImplementedException);
//		}
//	}
//
//	m_locked = true;
//	return &m_lockedBuffer;
//}

//------------------------------------------------------------------------------
//void VertexBuffer::Unlock()
//{
//}

////------------------------------------------------------------------------------
//void VertexBuffer::Resize(size_t bufferSize)
//{
//	if (!m_initialUpdate)
//	{
//		if (LN_CHECK_STATE(m_usage == ResourceUsage::Dynamic)) return;
//	}
//	m_bufferSize = bufferSize;
//	m_lockedBuffer.Resize(m_bufferSize);
//}

//------------------------------------------------------------------------------
void VertexBuffer::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		// 必要があればデータを保存する
		if (m_pool == GraphicsResourcePool::Managed)
		{
			m_buffer.resize(m_deviceObj->GetByteCount());
			memcpy(m_buffer.data(), m_deviceObj->Lock(), m_buffer.size());
			m_deviceObj->Unlock();
		}

		// オブジェクト破棄
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		// 復帰後は次の ResolveDeviceObject() で新しい RHI オブジェクトにセットされる
		m_locked = true;
	}
}

//------------------------------------------------------------------------------
Driver::IVertexBuffer* VertexBuffer::ResolveDeviceObject()
{
	if (m_locked)
	{
		if (IsRHIDirect())
		{
			m_deviceObj->Unlock();
		}
		else
		{
			if (m_deviceObj == nullptr || m_deviceObj->GetByteCount() < m_buffer.size())
			{
				LN_SAFE_RELEASE(m_deviceObj);
				m_deviceObj = m_manager->GetGraphicsDevice()->CreateVertexBuffer(m_buffer.size(), m_buffer.data(), m_usage);
			}
			else
			{
				RenderBulkData data(m_buffer.data(), m_buffer.size());
				Driver::IVertexBuffer* deviceObj = m_deviceObj;
				LN_ENQUEUE_RENDER_COMMAND_2(
					VertexBuffer_SetSubData, m_manager,
					RenderBulkData, data,
					RefPtr<Driver::IVertexBuffer>, deviceObj,
					{
						deviceObj->SetSubData(0, data.GetData(), data.GetSize());
					});
			}
		}
	}

	m_initialUpdate = false;
	m_locked = false;
	return m_deviceObj;
}

//------------------------------------------------------------------------------
//bool VertexBuffer::VerifyInitialUpdate()
//{
//	if (!m_initialUpdate)
//	{
//		if (LN_CHECK_STATE(m_usage == ResourceUsage::Dynamic)) return false;
//	}
//	else
//	{
//		if (LN_CHECK_STATE(m_deviceObj == nullptr)) return false;
//	}
//	return true;
//}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
