
#include "../Internal.h"
#include <Lumino/Graphics/IndexBuffer.h>
#include "GraphicsManager.h"
#include "Device/GraphicsDriverInterface.h"

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
{
	
}

//------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
	LN_SAFE_RELEASE(m_deviceObj);
}

//------------------------------------------------------------------------------
void IndexBuffer::Initialize(detail::GraphicsManager* manager, int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage)
{
	m_indexCount = indexCount;
	m_format = format;
	m_usage = usage;

	GraphicsResourceObject::Initialize(manager);

	int stride = (format == IndexBufferFormat_UInt16) ? 2 : 4;
	m_data = ByteBuffer(stride * indexCount);

	m_deviceObj = m_manager->GetGraphicsDevice()->CreateIndexBuffer(m_indexCount, initialData, m_format, m_usage);
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
ByteBuffer* IndexBuffer::Lock()
{
	if (m_lockedBuffer.GetSize() == 0 || m_usage == ResourceUsage::Static)
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

	return &m_lockedBuffer;
}

//------------------------------------------------------------------------------
void IndexBuffer::Unlock()
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
		m_deviceObj = m_manager->GetGraphicsDevice()->CreateIndexBuffer(m_indexCount, data, m_format, m_usage);
	}
}


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
