
#include "../Internal.h"
#include "GraphicsManager.h"
#include "RenderingCommand.h"
#include "Device/GraphicsDriverInterface.h"
#include <Lumino/Graphics/IndexBuffer.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// IndexBuffer
//==============================================================================
/**
@brief		インデックスバッファを作成します。
@param[in]	indexCount		: インデックスの要素数
@param[in]	initialData		: 初期値として設定するインデックスデータ
@param[in]	format			: インデックスバッファのフォーマット
@param[in]	usage			: インデックスバッファリソースの使用方法
*/
//static IndexBuffer* Create(int indexCount, const void* initialData = NULL, IndexBufferFormat format = IndexBufferFormat_UInt16, DeviceResourceUsage usage = DeviceResourceUsage_Static);

/**
@brief		インデックスバッファを作成します。
@param[in]	manager			: 作成に使用する GraphicsManager
@param[in]	indexCount		: インデックスの要素数
@param[in]	initialData		: 初期値として設定するインデックスデータ
@param[in]	format			: インデックスバッファのフォーマット
@param[in]	usage			: インデックスバッファリソースの使用方法
@details	この関数はデフォルト以外の GraphicsManager を指定して作成する場合に使用します。
*/
//static IndexBuffer* Create(GraphicsManager* manager, int indexCount, const void* initialData = NULL, IndexBufferFormat format = IndexBufferFormat_UInt16, DeviceResourceUsage usage = DeviceResourceUsage_Static);

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
	: m_rhiObject(nullptr)
	, m_format(IndexBufferFormat_UInt16)
	, m_usage(ResourceUsage::Static)
	, m_pool(GraphicsResourcePool::Managed)
	, m_buffer()
	, m_rhiLockedBuffer(nullptr)
	, m_initialUpdate(true)
	, m_locked(false)
{
}

//------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
	m_rhiObject.SafeRelease();
}

//------------------------------------------------------------------------------
void IndexBuffer::Initialize(detail::GraphicsManager* manager, int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage, bool sizeConst)
{
	GraphicsResourceObject::Initialize(manager);
	m_format = format;
	m_usage = usage;

	if (sizeConst)
	{
		m_rhiObject.Attach(m_manager->GetGraphicsDevice()->CreateIndexBuffer(indexCount, initialData, m_format, m_usage), false);
	}
	else
	{
		m_buffer.resize(Utils::GetIndexBufferSize(m_format, indexCount));
	}
}

//------------------------------------------------------------------------------
int IndexBuffer::GetIndexCount() const
{
	return static_cast<int>(m_buffer.size() / Utils::GetIndexStride(m_format));
}

//------------------------------------------------------------------------------
void IndexBuffer::Reserve(int indexCount)
{
	if (LN_CHECK_STATE(!IsRHIDirect())) return;		// サイズ変更禁止

	size_t newSize = static_cast<size_t>(indexCount * GetIndexStride());
	if (newSize != m_buffer.capacity())
	{
		m_buffer.reserve(newSize);
	}
}

//------------------------------------------------------------------------------
void IndexBuffer::Resize(int indexCount)
{
	if (LN_CHECK_STATE(!IsRHIDirect())) return;		// サイズ変更禁止

	UpdateFormat(indexCount);
	size_t newSize = static_cast<size_t>(indexCount* GetIndexStride());
	if (newSize != m_buffer.size())
	{
		m_buffer.resize(newSize);
	}
}

//------------------------------------------------------------------------------
void* IndexBuffer::GetMappedData()
{
	if (m_usage == ResourceUsage::Static)
	{
		// sizeConst で、まだ1度も SetVertexBufferCommand に入っていない場合は直接 Lock で書き換えできる
		if (m_initialUpdate && m_rhiObject != nullptr)
		{
			if (m_rhiLockedBuffer == nullptr)
			{
				size_t lockedSize;
				m_rhiObject->Lock(&m_rhiLockedBuffer, &lockedSize);
			}
			m_locked = true;
			return m_rhiLockedBuffer;
		}
	}

	m_locked = true;
	return m_buffer.data();
}

//------------------------------------------------------------------------------
void* IndexBuffer::RequestMappedData(int indexCount)
{
	if (GetIndexCount() < indexCount)
	{
		Resize(indexCount);
	}
	return GetMappedData();
}

//------------------------------------------------------------------------------
void IndexBuffer::Clear()
{
	m_buffer.clear();
	m_locked = true;
}

//------------------------------------------------------------------------------
void IndexBuffer::SetIndex(int index, int vertexIndex)
{
	void* indexBuffer = GetMappedData();

	if (m_format == IndexBufferFormat_UInt16)
	{
		uint16_t* i = (uint16_t*)indexBuffer;
		i[index] = vertexIndex;
	}
	else if (m_format == IndexBufferFormat_UInt32)
	{
		uint32_t* i = (uint32_t*)indexBuffer;
		i[index] = vertexIndex;
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

//------------------------------------------------------------------------------
Driver::IIndexBuffer* IndexBuffer::ResolveRHIObject()
{
	if (m_locked)
	{
		if (IsRHIDirect())
		{
			m_rhiObject->Unlock();
		}
		else
		{
			if (m_rhiObject == nullptr || m_rhiObject->GetByteCount() != m_buffer.size())
			{
				m_rhiObject.Attach(m_manager->GetGraphicsDevice()->CreateIndexBuffer(GetIndexCount(), m_buffer.data(), m_format, m_usage), false);
			}
			else
			{
				RenderBulkData data(m_buffer.data(), m_buffer.size());
				Driver::IIndexBuffer* deviceObj = m_rhiObject;
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

	return m_rhiObject;
}

//------------------------------------------------------------------------------
int IndexBuffer::GetIndexStride() const
{
	return Utils::GetIndexStride(m_format);
}

//------------------------------------------------------------------------------
void IndexBuffer::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		// 必要があればデータを保存する
		if (m_pool == GraphicsResourcePool::Managed)
		{
			m_buffer.resize(m_rhiObject->GetByteCount());

			void* buffer;
			size_t size;
			m_rhiObject->Lock(&buffer, &size);
			memcpy(m_buffer.data(), buffer, m_buffer.size());
			m_rhiObject->Unlock();
		}

		// オブジェクト破棄
		m_rhiObject.SafeRelease();
	}
	else
	{
		// 復帰後は次の ResolveDeviceObject() で新しい RHI オブジェクトにセットされる
		m_locked = true;
	}
}

//------------------------------------------------------------------------------
void IndexBuffer::UpdateFormat(int indexCount)
{
	if (m_format == IndexBufferFormat_UInt16 && indexCount > 0xFFFF)
	{
		m_format = IndexBufferFormat_UInt32;
	}
}

LN_NAMESPACE_END
