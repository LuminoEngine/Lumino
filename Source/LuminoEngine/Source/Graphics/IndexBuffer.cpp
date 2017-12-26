
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
//static IndexBuffer* create(int indexCount, const void* initialData = NULL, IndexBufferFormat format = IndexBufferFormat_UInt16, DeviceResourceUsage usage = DeviceResourceUsage_Static);

/**
@brief		インデックスバッファを作成します。
@param[in]	manager			: 作成に使用する GraphicsManager
@param[in]	indexCount		: インデックスの要素数
@param[in]	initialData		: 初期値として設定するインデックスデータ
@param[in]	format			: インデックスバッファのフォーマット
@param[in]	usage			: インデックスバッファリソースの使用方法
@details	この関数はデフォルト以外の GraphicsManager を指定して作成する場合に使用します。
*/
//static IndexBuffer* create(GraphicsManager* manager, int indexCount, const void* initialData = NULL, IndexBufferFormat format = IndexBufferFormat_UInt16, DeviceResourceUsage usage = DeviceResourceUsage_Static);

////------------------------------------------------------------------------------
//IndexBuffer* IndexBuffer::create(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
//{
//	return create(GraphicsManager::getInstance(), indexCount, initialData, format, usage);
//}
//
////------------------------------------------------------------------------------
//IndexBuffer* IndexBuffer::create(GraphicsManager* manager, int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
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
}

//------------------------------------------------------------------------------
void IndexBuffer::initialize(detail::GraphicsManager* manager, int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage, bool sizeConst)
{
	GraphicsResourceObject::initialize();
	m_format = format;
	m_usage = usage;

	if (sizeConst)
	{
		m_rhiObject.attach(m_manager->getGraphicsDevice()->createIndexBuffer(indexCount, initialData, m_format, m_usage), false);
	}
	else
	{
		m_buffer.resize(Utils::getIndexBufferSize(m_format, indexCount));
	}
}

//------------------------------------------------------------------------------
void IndexBuffer::Dispose()
{
	m_rhiObject.safeRelease();
	GraphicsResourceObject::Dispose();
}

//------------------------------------------------------------------------------
int IndexBuffer::getIndexCount() const
{
	return static_cast<int>(m_buffer.size() / Utils::getIndexStride(m_format));
}

//------------------------------------------------------------------------------
void IndexBuffer::reserve(int indexCount)
{
	if (LN_REQUIRE(!isRHIDirect())) return;		// サイズ変更禁止

	size_t newSize = static_cast<size_t>(indexCount * getIndexStride());
	if (newSize != m_buffer.capacity())
	{
		m_buffer.reserve(newSize);
	}
}

//------------------------------------------------------------------------------
void IndexBuffer::resize(int indexCount)
{
	if (LN_REQUIRE(!isRHIDirect())) return;		// サイズ変更禁止

	IndexBufferFormat oldFormat = m_format;
	m_format = (indexCount > 0xFFFF) ? IndexBufferFormat_UInt32 : IndexBufferFormat_UInt16;

	size_t newSize = static_cast<size_t>(indexCount* getIndexStride());
	if (newSize != m_buffer.size())
	{
		m_buffer.resize(newSize);
	}

	if (oldFormat == IndexBufferFormat_UInt16 && m_format == IndexBufferFormat_UInt32)
	{
		// 16 -> 32
		auto* rpos16 = (uint16_t*)(m_buffer.data() + (indexCount * sizeof(uint16_t)));
		auto* rend16 = (uint16_t*)(m_buffer.data());
		auto* rpos32 = (uint32_t*)(m_buffer.data() + (indexCount * sizeof(uint32_t)));
		auto* rend32 = (uint32_t*)(m_buffer.data());
		for (; rpos32 < rend32; rpos32--, rpos16--)
		{
			uint16_t t = *rpos16;
			*rpos32 = t;
		}
	}
	else if (oldFormat == IndexBufferFormat_UInt32 && m_format == IndexBufferFormat_UInt16)
	{
		LN_NOTIMPLEMENTED();
	}
}

//------------------------------------------------------------------------------
void* IndexBuffer::getMappedData()
{
	if (m_usage == ResourceUsage::Static)
	{
		// sizeConst で、まだ1度も SetVertexBufferCommand に入っていない場合は直接 lock で書き換えできる
		if (m_initialUpdate && m_rhiObject != nullptr)
		{
			if (m_rhiLockedBuffer == nullptr)
			{
				size_t lockedSize;
				m_rhiObject->lock(&m_rhiLockedBuffer, &lockedSize);
			}
			m_locked = true;
			return m_rhiLockedBuffer;
		}
	}

	m_initialUpdate = false;
	m_locked = true;
	return m_buffer.data();
}

//------------------------------------------------------------------------------
void* IndexBuffer::requestMappedData(int indexCount)
{
	if (getIndexCount() < indexCount)
	{
		resize(indexCount);
	}
	return getMappedData();
}

//------------------------------------------------------------------------------
void IndexBuffer::clear()
{
	m_buffer.clear();
	m_locked = true;
}

//------------------------------------------------------------------------------
void IndexBuffer::setIndex(int index, int vertexIndex)
{
	void* indexBuffer = getMappedData();

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
Driver::IIndexBuffer* IndexBuffer::resolveRHIObject()
{
	if (m_locked)
	{
		if (isRHIDirect())
		{
			m_rhiObject->unlock();
		}
		else
		{
			if (m_rhiObject == nullptr || m_rhiObject->getByteCount() != m_buffer.size())
			{
				m_rhiObject.attach(m_manager->getGraphicsDevice()->createIndexBuffer(getIndexCount(), m_buffer.data(), m_format, m_usage), false);
			}
			else
			{
				detail::RenderBulkData data(m_buffer.data(), m_buffer.size());
				Driver::IIndexBuffer* deviceObj = m_rhiObject;
				LN_ENQUEUE_RENDER_COMMAND_2(
					VertexBuffer_SetSubData, m_manager,
					detail::RenderBulkData, data,
					Ref<Driver::IIndexBuffer>, deviceObj,
					{
						deviceObj->setSubData(0, data.getData(), data.getSize());
					});
			}
		}
	}

	return m_rhiObject;
}

//------------------------------------------------------------------------------
int IndexBuffer::getIndexStride() const
{
	return Utils::getIndexStride(m_format);
}

//------------------------------------------------------------------------------
void IndexBuffer::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		// 必要があればデータを保存する
		if (m_pool == GraphicsResourcePool::Managed)
		{
			m_buffer.resize(m_rhiObject->getByteCount());

			void* buffer;
			size_t size;
			m_rhiObject->lock(&buffer, &size);
			memcpy(m_buffer.data(), buffer, m_buffer.size());
			m_rhiObject->unlock();
		}

		// オブジェクト破棄
		m_rhiObject.safeRelease();
	}
	else
	{
		// 復帰後は次の resolveDeviceObject() で新しい RHI オブジェクトにセットされる
		m_locked = true;
	}
}

LN_NAMESPACE_END
