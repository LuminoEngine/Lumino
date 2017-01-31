
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
	, m_bufferSize(0)
	, m_usage(ResourceUsage::Static)
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
void VertexBuffer::Initialize(detail::GraphicsManager* manager, size_t bufferSize, const void* data, ResourceUsage usage)
{
	GraphicsResourceObject::Initialize(manager);

	m_bufferSize = bufferSize;
	m_usage = usage;
	m_deviceObj = m_manager->GetGraphicsDevice()->CreateVertexBuffer(m_bufferSize, data, usage);

	if (m_usage == ResourceUsage::Dynamic)
	{
		m_lockedBuffer.Resize(m_bufferSize);
	}
}

//------------------------------------------------------------------------------
ByteBuffer* VertexBuffer::Lock()
{
	if (m_usage == ResourceUsage::Static)
	{
		// まだ1度も SetVertexBufferCommand に入っていない場合は直接 Lock で書き換えできる
		if (m_initialUpdate) {
			m_lockedBuffer.Attach(m_deviceObj->Lock(), m_deviceObj->GetByteCount());
		}
		else {
			LN_THROW(0, NotImplementedException);
		}
	}

	return &m_lockedBuffer;
}

//------------------------------------------------------------------------------
void VertexBuffer::Unlock()
{
	if (m_usage == ResourceUsage::Static)
	{
		if (m_initialUpdate) {
			m_deviceObj->Unlock();
		}
		else {
			LN_THROW(0, NotImplementedException);
		}
	}
	else
	{
		if (m_deviceObj->GetByteCount() < m_bufferSize)
		{
			LN_SAFE_RELEASE(m_deviceObj);
			m_deviceObj = m_manager->GetGraphicsDevice()->CreateVertexBuffer(m_bufferSize, m_lockedBuffer.GetConstData(), m_usage);
		}
		else
		{
			RenderBulkData data(m_lockedBuffer.GetConstData(), m_lockedBuffer.GetSize());
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

//------------------------------------------------------------------------------
void VertexBuffer::Resize(size_t bufferSize)
{
	if (!m_initialUpdate)
	{
		LN_CHECK_STATE(m_usage == ResourceUsage::Dynamic);
	}
	m_bufferSize = bufferSize;
	m_lockedBuffer.Resize(m_bufferSize);
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
