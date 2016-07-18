
#pragma once
#include "../Internal.h"
#include <Lumino/Graphics/VertexBuffer.h>
#include "GraphicsManager.h"
#include "Device/GraphicsDriverInterface.h"

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
	, m_bufferSize(0)
	, m_usage(DeviceResourceUsage_Static)
	, m_pool(GraphicsResourcePool::Managed)	// TODO
	, m_initialUpdate(true)
{
}

//------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
	LN_SAFE_RELEASE(m_deviceObj);
}

//------------------------------------------------------------------------------
void VertexBuffer::Initialize(GraphicsManager* manager, size_t bufferSize, const void* data, DeviceResourceUsage usage)
{
	GraphicsResourceObject::Initialize(manager);

	m_bufferSize = bufferSize;
	m_usage = usage;
	m_deviceObj = m_manager->GetGraphicsDevice()->CreateVertexBuffer(m_bufferSize, data, usage);
}

//------------------------------------------------------------------------------
ByteBuffer* VertexBuffer::Lock()
{
	// まだ1度も SetVertexBufferCommand に入っていない場合は直接 Lock で書き換えできる
	if (m_initialUpdate) {
		m_lockedBuffer.Attach(m_deviceObj->Lock(), m_deviceObj->GetByteCount());
	}
	else {
		LN_THROW(0, NotImplementedException);
	}
	return &m_lockedBuffer;
}

//------------------------------------------------------------------------------
void VertexBuffer::Unlock()
{
	if (m_initialUpdate) {
		m_deviceObj->Unlock();
	}
	else {
		LN_THROW(0, NotImplementedException);
	}
}

//------------------------------------------------------------------------------
void VertexBuffer::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL)
	{
		// 必要があればデータを保存する
		if (m_pool == GraphicsResourcePool::Managed)
		{
			m_lockedBuffer.Attach(m_deviceObj->Lock(), m_deviceObj->GetByteCount());
			m_deviceObj->Unlock();
		}

		// オブジェクト破棄
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		assert(m_deviceObj == NULL);

		// 保存したデータはあるか？
		const void* data = NULL;
		if (m_pool == GraphicsResourcePool::Managed) {
			data = m_lockedBuffer.GetConstData();
		}

		// オブジェクトを作り直す
		m_deviceObj = m_manager->GetGraphicsDevice()->CreateVertexBuffer(m_bufferSize, data, m_usage);
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
