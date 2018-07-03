
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <Lumino/Graphics/VertexBuffer.hpp>

namespace ln {

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
	, m_rhiBufferByteSize(0)
	, m_usage(GraphicsResourceUsage::Static)
	, m_pool(GraphicsResourcePool::Managed)
	, m_buffer()
	, m_rhiLockedBuffer(nullptr)
	, m_initialUpdate(true)
	, m_modified(false)
{
}

//------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::initialize(size_t bufferSize, GraphicsResourceUsage usage)
{
	GraphicsResource::initialize();
	m_usage = usage;
	m_buffer.resize(bufferSize);
	m_modified = true;
}

void VertexBuffer::initialize(size_t bufferSize, const void* initialData, GraphicsResourceUsage usage)
{
	GraphicsResource::initialize();
	m_usage = usage;

	if (initialData)
	{
		m_rhiObject = manager()->deviceContext()->createVertexBuffer(m_usage, bufferSize, initialData);
		m_rhiBufferByteSize = bufferSize;
	}
	else
	{
		m_buffer.resize(bufferSize);
		m_modified = true;
	}
}

//------------------------------------------------------------------------------
void VertexBuffer::dispose()
{
	if (m_rhiObject) {
		m_rhiObject->dispose();
		m_rhiObject = nullptr;
	}

	GraphicsResource::dispose();
}

//------------------------------------------------------------------------------
int VertexBuffer::size() const
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
	if (m_usage == GraphicsResourceUsage::Static)
	{
		// sizeConst で、まだ1度も SetVertexBufferCommand に入っていない場合は直接 lock で書き換えできる
		if (m_initialUpdate && m_rhiObject != nullptr)
		{
			if (m_rhiLockedBuffer == nullptr)
			{
				m_rhiLockedBuffer = m_rhiObject->map(0, size());
			}
			m_modified = true;
			return m_rhiLockedBuffer;
		}
	}

	m_modified = true;
	return m_buffer.data();
}

////------------------------------------------------------------------------------
//void* VertexBuffer::requestMappedData(int size)
//{
//	if (getSize() < size)
//	{
//		resize(size);
//	}
//	return getMappedData();
//}

//------------------------------------------------------------------------------
void VertexBuffer::clear()
{
	m_buffer.clear();
	m_modified = true;
}

//------------------------------------------------------------------------------
detail::IVertexBuffer* VertexBuffer::resolveRHIObject()
{
	if (m_modified)
	{
		if (isRHIDirect())
		{
			m_rhiObject->unmap();
		}
		else
		{
			if (m_rhiObject == nullptr || m_rhiBufferByteSize != m_buffer.size())
			{
				m_rhiObject = manager()->deviceContext()->createVertexBuffer(m_usage, m_buffer.size(), m_buffer.data());
				m_rhiBufferByteSize = m_buffer.size();
			}
			else
			{
				detail::RenderBulkData data(m_buffer.data(), m_buffer.size());
				detail::IVertexBuffer* rhiObject = m_rhiObject;
				LN_ENQUEUE_RENDER_COMMAND_2(
					VertexBuffer_SetSubData, manager(),
					detail::RenderBulkData, data,
					Ref<detail::IVertexBuffer>, rhiObject,
					{
						rhiObject->setSubData(0, data.data(), data.size());
					});
			}
		}
	}

	if (LN_ENSURE(m_rhiObject)) return nullptr;

	m_initialUpdate = false;
	m_modified = false;
	return m_rhiObject;
}

//------------------------------------------------------------------------------
void VertexBuffer::onChangeDevice(detail::IGraphicsDeviceContext* device)
{
	if (device)
	{
		if (m_pool == GraphicsResourcePool::Managed)
		{
			// data is transferred by the next resolveRHIObject()
			m_modified = true;
		}
	}
	else
	{
		m_rhiObject.reset();
	}
}

} // namespace ln
