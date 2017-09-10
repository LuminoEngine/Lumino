
#include "../Internal.h"
#include "GraphicsManager.h"
#include "RenderingCommand.h"
#include "Device/GraphicsDriverInterface.h"
#include <Lumino/Graphics/VertexBuffer.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// VertexBuffer
//==============================================================================


	/**
		@brief		頂点バッファを作成します。
		@param[in]	vertexElements	: 頂点データレイアウトを表す VertexElement の配列
		@param[in]	elementsCount	: vertexElements の要素数
		@param[in]	vertexCount		: 頂点の数
		@param[in]	data			: 作成と同時に書き込む初期データ (必要なければ NULL)
		@param[in]	usage			: 頂点バッファリソースの使用方法
	*/
	//static VertexBuffer* create(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data = NULL, DeviceResourceUsage usage = DeviceResourceUsage_Static);

	/**
		@brief		頂点バッファを作成します。
		@param[in]	manager			: 作成に使用する GraphicsManager
		@param[in]	vertexElements	: 頂点データレイアウトを表す VertexElement の配列
		@param[in]	elementsCount	: vertexElements の要素数
		@param[in]	vertexCount		: 頂点の数
		@param[in]	data			: 作成と同時に書き込む初期データ (必要なければ NULL)
		@param[in]	usage			: 頂点バッファリソースの使用方法
		@details	この関数はデフォルト以外の GraphicsManager を指定して作成する場合に使用します。
	*/
	//static VertexBuffer* create(GraphicsManager* manager, const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data = NULL, DeviceResourceUsage usage = DeviceResourceUsage_Static);
	
////------------------------------------------------------------------------------
//VertexBuffer* VertexBuffer::create(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage)
//{
//	return create(GraphicsManager::getInstance(), vertexElements, elementsCount, vertexCount, data, usage);
//}
//
////------------------------------------------------------------------------------
//VertexBuffer* VertexBuffer::create(GraphicsManager* manager, const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage)
//{
//	LN_THROW(manager != NULL, ArgumentException);
//	return LN_NEW VertexBuffer(manager, vertexElements, elementsCount, vertexCount, data, usage);
//}

//------------------------------------------------------------------------------
VertexBuffer::VertexBuffer()
	: m_rhiObject(nullptr)
	, m_usage(ResourceUsage::Static)
	, m_pool(GraphicsResourcePool::Managed)
	, m_buffer()
	, m_rhiLockedBuffer(nullptr)
	, m_initialUpdate(true)
	, m_locked(false)
{
}

//------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
}

//------------------------------------------------------------------------------
void VertexBuffer::initialize(detail::GraphicsManager* manager, size_t bufferSize, const void* data, ResourceUsage usage, bool sizeConst)
{
	GraphicsResourceObject::initialize();
	m_usage = usage;

	if (sizeConst)
	{
		m_rhiObject = m_manager->getGraphicsDevice()->createVertexBuffer(bufferSize, data, usage);
	}
	else
	{
		m_buffer.resize(bufferSize);
	}
}

//------------------------------------------------------------------------------
void VertexBuffer::Dispose()
{
	LN_SAFE_RELEASE(m_rhiObject);
	GraphicsResourceObject::Dispose();
}

//------------------------------------------------------------------------------
int VertexBuffer::getSize() const
{
	return static_cast<int>(m_buffer.size());
}

//------------------------------------------------------------------------------
void VertexBuffer::reserve(int size)
{
	if (LN_REQUIRE(!isRHIDirect())) return;		// サイズ変更禁止

	size_t newSize = static_cast<size_t>(size);
	if (newSize != m_buffer.capacity())
	{
		m_buffer.reserve(newSize);
	}
}

//------------------------------------------------------------------------------
void VertexBuffer::resize(int size)
{
	if (LN_REQUIRE(!isRHIDirect())) return;		// サイズ変更禁止

	size_t newSize = static_cast<size_t>(size);
	if (newSize != m_buffer.size())
	{
		m_buffer.resize(newSize);
	}
}

//------------------------------------------------------------------------------
void* VertexBuffer::getMappedData()
{
	if (m_usage == ResourceUsage::Static)
	{
		// sizeConst で、まだ1度も SetVertexBufferCommand に入っていない場合は直接 lock で書き換えできる
		if (m_initialUpdate && m_rhiObject != nullptr)
		{
			if (m_rhiLockedBuffer == nullptr)
			{
				m_rhiLockedBuffer = m_rhiObject->lock();
			}
			m_locked = true;
			return m_rhiLockedBuffer;
		}
	}

	m_locked = true;
	return m_buffer.data();
}

//------------------------------------------------------------------------------
void* VertexBuffer::requestMappedData(int size)
{
	if (getSize() < size)
	{
		resize(size);
	}
	return getMappedData();
}

//------------------------------------------------------------------------------
void VertexBuffer::clear()
{
	m_buffer.clear();
	m_locked = true;
}

//------------------------------------------------------------------------------
Driver::IVertexBuffer* VertexBuffer::resolveRHIObject()
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
				LN_SAFE_RELEASE(m_rhiObject);
				m_rhiObject = m_manager->getGraphicsDevice()->createVertexBuffer(m_buffer.size(), m_buffer.data(), m_usage);
			}
			else
			{
				detail::RenderBulkData data(m_buffer.data(), m_buffer.size());
				Driver::IVertexBuffer* deviceObj = m_rhiObject;
				LN_ENQUEUE_RENDER_COMMAND_2(
					VertexBuffer_SetSubData, m_manager,
					detail::RenderBulkData, data,
					Ref<Driver::IVertexBuffer>, deviceObj,
					{
						deviceObj->setSubData(0, data.getData(), data.getSize());
					});
			}
		}
	}

	m_initialUpdate = false;
	m_locked = false;
	return m_rhiObject;
}

//------------------------------------------------------------------------------
void VertexBuffer::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		// 必要があればデータを保存する
		if (m_pool == GraphicsResourcePool::Managed)
		{
			m_buffer.resize(m_rhiObject->getByteCount());
			memcpy(m_buffer.data(), m_rhiObject->lock(), m_buffer.size());
			m_rhiObject->unlock();
		}

		// オブジェクト破棄
		LN_SAFE_RELEASE(m_rhiObject);
	}
	else
	{
		// 復帰後は次の resolveDeviceObject() で新しい RHI オブジェクトにセットされる
		m_locked = true;
	}
}

LN_NAMESPACE_END
