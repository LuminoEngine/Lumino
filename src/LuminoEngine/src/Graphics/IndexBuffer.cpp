
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <Lumino/Graphics/IndexBuffer.hpp>

namespace ln {

//==============================================================================
// IndexBuffer
//==============================================================================
	/*
	
	- GraphicsResourcePool::None + GraphicsResourceUsage::Static
		- resolve で m_buffer を削除する。省メモリが目的。基本的に使うべきではない。

	- GraphicsResourcePool::Managed + GraphicsResourceUsage::Static
		- resolve で m_buffer を削除しない。DeviceReset 時には復元する。デフォルト。

	- GraphicsResourcePool::None + GraphicsResourceUsage::Dynamic
		- resolve で m_buffer を削除しない（Dynamic 設定による）。DeviceReset 時には復元しない。毎フレーム新規作成されるような用途。

	- GraphicsResourcePool::Managed + GraphicsResourceUsage::Dynamic
		- resolve で m_buffer を削除しない（Dynamic 設定による）。DeviceReset 時には復元する。毎フレームほどではないが、比較的頻繁に発生するイベントで部分変更される用途。（drawText）

	*/

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
	, m_rhiBufferByteSize(0)
	, m_format(IndexBufferFormat::Index16)
	, m_usage(GraphicsResourceUsage::Static)
	, m_pool(GraphicsResourcePool::Managed)
	, m_buffer()
	, m_rhiLockedBuffer(nullptr)
	, m_initialUpdate(true)
	, m_modified(false)
{
}

//------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
}

//------------------------------------------------------------------------------
void IndexBuffer::initialize(int indexCount, const void* initialData, IndexBufferFormat format, GraphicsResourceUsage usage)
{
	GraphicsResource::initialize();
	m_format = format;
	m_usage = usage;

	if (m_usage == GraphicsResourceUsage::Static)
	{
		m_rhiObject = manager()->deviceContext()->createIndexBuffer(m_usage, m_format, indexCount, initialData);
		m_rhiBufferByteSize = getIndexBufferSize(m_format, indexCount);
	}
	else
	{
		m_buffer.resize(getIndexBufferSize(m_format, indexCount));
	}
}

//------------------------------------------------------------------------------
void IndexBuffer::dispose()
{
	m_rhiObject = nullptr;
	GraphicsResource::dispose();
}

//------------------------------------------------------------------------------
int IndexBuffer::size() const
{
	return static_cast<int>(m_buffer.size() / getIndexStride(m_format));
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
	if (LN_REQUIRE(m_usage == GraphicsResourceUsage::Dynamic)) return;

	size_t newSize = static_cast<size_t>(indexCount* getIndexStride());
	if (newSize != m_buffer.size())
	{
		m_buffer.resize(newSize);
	}
}

//------------------------------------------------------------------------------
void* IndexBuffer::getMappedData()
{
	if (m_usage == GraphicsResourceUsage::Static)
	{
		// sizeConst で、まだ1度も SetVertexBufferCommand に入っていない場合は直接 lock で書き換えできる
		if (m_initialUpdate && m_rhiObject != nullptr)
		{
			void* data = m_rhiObject->map(0, getIndexBufferSize(m_format, size()));
			m_modified = true;
			return data;
		}
	}

	m_initialUpdate = false;
	m_modified = true;
	return m_buffer.data();
}

//------------------------------------------------------------------------------
//void* IndexBuffer::requestMappedData(int indexCount)
//{
//	if (size() < indexCount)
//	{
//		resize(indexCount);
//	}
//	return getMappedData();
//}

//------------------------------------------------------------------------------
void IndexBuffer::clear()
{
	m_buffer.clear();
	m_modified = true;
}

void IndexBuffer::setFormat(IndexBufferFormat format)
{
	size_t indexCount = size();
	IndexBufferFormat oldFormat = m_format;
	m_format = format;

	if (indexCount > 0)
	{
		if (oldFormat == IndexBufferFormat::Index16 && m_format == IndexBufferFormat::Index32)
		{
			// 16 -> 32
			m_buffer.resize(indexCount * sizeof(uint32_t));
			auto* rpos16 = (uint16_t*)(m_buffer.data() + (indexCount * sizeof(uint16_t)));
			auto* rend16 = (uint16_t*)(m_buffer.data());
			auto* rpos32 = (uint32_t*)(m_buffer.data() + (indexCount * sizeof(uint32_t)));
			auto* rend32 = (uint32_t*)(m_buffer.data());
			for (; rpos32 >= rend32; rpos32--, rpos16--)
			{
				uint16_t t = *rpos16;
				*rpos32 = t;
			}
		}
		else if (oldFormat == IndexBufferFormat::Index32 && m_format == IndexBufferFormat::Index16)
		{
			LN_NOTIMPLEMENTED();
		}
	}
}

//------------------------------------------------------------------------------
void IndexBuffer::setIndex(int index, int vertexIndex)
{
	void* indexBuffer = getMappedData();

	if (m_format == IndexBufferFormat::Index16)
	{
		uint16_t* i = (uint16_t*)indexBuffer;
		i[index] = vertexIndex;
	}
	else if (m_format == IndexBufferFormat::Index32)
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
detail::IIndexBuffer* IndexBuffer::resolveRHIObject()
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
				m_rhiObject = manager()->deviceContext()->createIndexBuffer(m_usage, m_format, size(), m_buffer.data());
			}
			else
			{
				detail::RenderBulkData data(m_buffer.data(), m_buffer.size());
				detail::IIndexBuffer* rhiObject = m_rhiObject;
				LN_ENQUEUE_RENDER_COMMAND_2(
					IndexBuffer_setSubData, manager(),
					detail::RenderBulkData, data,
					Ref<detail::IIndexBuffer>, rhiObject,
					{
						rhiObject->setSubData(0, data.data(), data.size());
					});
			}
		}
	}

	return m_rhiObject;
}

int IndexBuffer::getIndexStride() const
{
	return getIndexStride(m_format);
}

void IndexBuffer::onChangeDevice(detail::IGraphicsDeviceContext* device)
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
