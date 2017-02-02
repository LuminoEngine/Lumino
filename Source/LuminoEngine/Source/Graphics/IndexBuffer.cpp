
#include "../Internal.h"
#include <Lumino/Graphics/IndexBuffer.h>
#include "GraphicsManager.h"
#include "Device/GraphicsDriverInterface.h"
#include "RenderingCommand.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

////------------------------------------------------------------------------------
//IndexBuffer* IndexBuffer::Create(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
//{
//	return Create(GraphicsManager::GetInstance(), indexCount, initialData, format, usage);
//}
//
////------------------------------------------------------------------------------
//IndexBuffer* IndexBuffer::Create(GraphicsManager* manager, int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
//{
//	LN_THROW(manager != NULL, ArgumentException);
//	return LN_NEW IndexBuffer(manager, indexCount, initialData, format, usage);
//}

//------------------------------------------------------------------------------
IndexBuffer::IndexBuffer()
	: m_deviceObj(nullptr)
	, m_indexCount(0)
	, m_format(IndexBufferFormat_UInt16)
	, m_usage(ResourceUsage::Static)
	, m_pool(GraphicsResourcePool::Managed)	// TODO
	, m_initialUpdate(true)
	, m_locked(false)
{

}

//------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
	m_deviceObj.SafeRelease();
}

//------------------------------------------------------------------------------
void IndexBuffer::Initialize(detail::GraphicsManager* manager, int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage)
{
	m_indexCount = indexCount;
	m_format = format;
	m_usage = usage;

	GraphicsResourceObject::Initialize(manager);


	m_deviceObj.Attach(m_manager->GetGraphicsDevice()->CreateIndexBuffer(m_indexCount, initialData, m_format, m_usage), false);

	if (m_usage == ResourceUsage::Dynamic)
	{
		int stride = (format == IndexBufferFormat_UInt16) ? 2 : 4;
		m_lockedBuffer.Resize(m_indexCount * stride);
	}
}

//------------------------------------------------------------------------------
Driver::IIndexBuffer* IndexBuffer::ResolveDeviceObject()
{
	if (m_locked)
	{
		if (m_usage == ResourceUsage::Static)
		{
			// まだ1度も SetVertexBufferCommand に入っていない場合は直接書き換えできる
			if (m_initialUpdate)
			{
				m_deviceObj->SetSubData(0, m_lockedBuffer.GetConstData(), m_lockedBuffer.GetSize());
			}
			else
			{
				LN_THROW(0, NotImplementedException);
			}

			//if (m_initialUpdate) {
			//	m_deviceObj->Unlock();
			//}
			//else {
			//	LN_THROW(0, NotImplementedException);
			//}
		}
		else
		{
			if (m_deviceObj->GetByteCount() < m_lockedBuffer.GetSize())
			{
				m_deviceObj.Attach(m_manager->GetGraphicsDevice()->CreateIndexBuffer(m_indexCount, m_lockedBuffer.GetConstData(), m_format, m_usage), false);
			}
			else
			{
				RenderBulkData data(m_lockedBuffer.GetConstData(), m_lockedBuffer.GetSize());
				Driver::IIndexBuffer* deviceObj = m_deviceObj;
				LN_ENQUEUE_RENDER_COMMAND_2(
					VertexBuffer_SetSubData, m_manager,
					RenderBulkData, data,
					RefPtr<Driver::IIndexBuffer>, deviceObj,
					{
						deviceObj->SetSubData(0, data.GetData(), data.GetSize());
					});
			}
		}
	}

	return m_deviceObj;
}

//------------------------------------------------------------------------------
int IndexBuffer::GetIndexStride() const
{
	if (m_format == IndexBufferFormat_UInt16) {
		return 2;
	}
	else if (m_format == IndexBufferFormat_UInt32) {
		return 4;
	}
	assert(0);
	return 0;
}

//------------------------------------------------------------------------------
ByteBuffer* IndexBuffer::GetMappedData()
{
	if (m_usage == ResourceUsage::Static)
	{
		if (m_lockedBuffer.GetSize() == 0)
		{
			m_lockedBuffer.Alloc(GetIndexStride() * m_indexCount);
		}
		// まだ1度も SetVertexBufferCommand に入っていない場合は直接 Lock で書き換えできる
		//if (m_initialUpdate) {
		//	void* buffer;
		//	size_t size;
		//	m_deviceObj->Lock(&buffer, &size);
		//	m_lockedBuffer.Attach(buffer, size);
		//}
		//else {
		//	LN_THROW(0, NotImplementedException);
		//}
	}

	m_locked = true;
	return &m_lockedBuffer;
}

//------------------------------------------------------------------------------
//void IndexBuffer::Unlock()
//{
//
//}

//------------------------------------------------------------------------------
void IndexBuffer::Resize(int indexCount, IndexBufferFormat format)
{
	if (!m_initialUpdate)
	{
		LN_CHECK_STATE(m_usage == ResourceUsage::Dynamic);
	}
	m_indexCount = indexCount;
	m_format = format;
	int stride = (format == IndexBufferFormat_UInt16) ? 2 : 4;
	m_lockedBuffer.Resize(indexCount * stride);
}

//------------------------------------------------------------------------------
//void IndexBuffer::SetSubData(uint32_t offsetBytes, void* data, uint32_t dataBytes)
//{
//
//}

//------------------------------------------------------------------------------
void IndexBuffer::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL)
	{
		// 必要があればデータを保存する
		if (m_pool == GraphicsResourcePool::Managed)
		{
			void* buffer;
			size_t size;
			m_deviceObj->Lock(&buffer, &size);
			m_lockedBuffer.Attach(buffer, size);
			m_deviceObj->Unlock();
		}

		// オブジェクト破棄
		m_deviceObj.SafeRelease();
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
		m_deviceObj.Attach(m_manager->GetGraphicsDevice()->CreateIndexBuffer(m_indexCount, data, m_format, m_usage), false);
	}
}


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
