
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "../Engine/RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/IndexBuffer.hpp>

namespace ln {

//==============================================================================
// IndexBuffer

IndexBuffer::IndexBuffer()
	: m_rhiObject(nullptr)
	, m_format(IndexBufferFormat::UInt16)
	, m_usage(GraphicsResourceUsage::Static)
	, m_pool(GraphicsResourcePool::Managed)
	, m_primaryIndexCount(0)
	, m_buffer()
	, m_rhiLockedBuffer(nullptr)
	, m_initialUpdate(true)
	, m_modified(false)
{
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::initialize(int indexCount, IndexBufferFormat format, GraphicsResourceUsage usage)
{
	GraphicsResource::initialize();
	m_format = format;
	m_usage = usage;
	m_modified = true;
	resize(indexCount);
}

void IndexBuffer::initialize(int indexCount, IndexBufferFormat format, const void* initialData, GraphicsResourceUsage usage)
{
	IndexBuffer::initialize(indexCount, format, usage);
	if (initialData)
	{
		m_rhiObject = manager()->deviceContext()->createIndexBuffer(m_usage, m_format, indexCount, initialData);
		m_modified = false;
	}
}

void IndexBuffer::dispose()
{
	m_rhiObject.reset();
	GraphicsResource::dispose();
}

int IndexBuffer::size() const
{
	return m_primaryIndexCount;
}

int IndexBuffer::bytesSize() const
{
	return m_primaryIndexCount * getIndexStride();
}

void IndexBuffer::reserve(int indexCount)
{
	m_buffer.reserve(static_cast<size_t>(indexCount * getIndexStride()));
}

void IndexBuffer::resize(int indexCount)
{
	m_primaryIndexCount = indexCount;
	m_buffer.resize(static_cast<size_t>(indexCount* getIndexStride()));
}

void* IndexBuffer::map(MapMode mode)
{
	// if have not entried the Command List at least once, can rewrite directly with map().
	if (m_initialUpdate && m_usage == GraphicsResourceUsage::Static && m_pool == GraphicsResourcePool::None)
	{
		if (!m_rhiObject) {
			m_rhiObject = manager()->deviceContext()->createIndexBuffer(m_usage, m_format, size(),nullptr);
		}

		if (m_rhiLockedBuffer == nullptr)
		{
			m_rhiLockedBuffer = m_rhiObject->map();
		}

		m_modified = true;
		return m_rhiLockedBuffer;
	}

	// prepare for GraphicsResourcePool::None
	size_t primarySize = bytesSize();
	if (m_buffer.size() < primarySize) {
		m_buffer.resize(primarySize);
	}

	m_modified = true;
	return m_buffer.data();
}

void IndexBuffer::clear()
{
	if (LN_REQUIRE(m_usage == GraphicsResourceUsage::Dynamic)) return;
	m_buffer.clear();
	m_primaryIndexCount = 0;
	m_modified = true;
}

void IndexBuffer::setFormat(IndexBufferFormat format)
{
	size_t indexCount = size();
	IndexBufferFormat oldFormat = m_format;
	m_format = format;

	if (indexCount > 0)
	{
		if (oldFormat == IndexBufferFormat::UInt16 && m_format == IndexBufferFormat::UInt32)
		{
			// 16 -> 32
			m_buffer.resize(indexCount * sizeof(uint32_t));
			auto* rpos16 = (uint16_t*)(m_buffer.data() + ((indexCount - 1) * sizeof(uint16_t)));
			auto* rend16 = (uint16_t*)(m_buffer.data());
			auto* rpos32 = (uint32_t*)(m_buffer.data() + ((indexCount - 1) * sizeof(uint32_t)));
			auto* rend32 = (uint32_t*)(m_buffer.data());
			for (; rpos32 >= rend32; rpos32--, rpos16--)
			{
				uint16_t t = *rpos16;
				*rpos32 = t;
			}
		}
		else if (oldFormat == IndexBufferFormat::UInt32 && m_format == IndexBufferFormat::UInt16)
		{
			LN_NOTIMPLEMENTED();
		}
	}
}

void IndexBuffer::setIndex(int index, int vertexIndex)
{
	void* indexBuffer = map(MapMode::Write);

	if (m_format == IndexBufferFormat::UInt16)
	{
		uint16_t* i = (uint16_t*)indexBuffer;
		i[index] = vertexIndex;
	}
	else if (m_format == IndexBufferFormat::UInt32)
	{
		uint32_t* i = (uint32_t*)indexBuffer;
		i[index] = vertexIndex;
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

int IndexBuffer::index(int index)
{
    void* indexBuffer = map(MapMode::Read);

    if (m_format == IndexBufferFormat::UInt16)
    {
        uint16_t* i = (uint16_t*)indexBuffer;
        return i[index];
    }
    else if (m_format == IndexBufferFormat::UInt32)
    {
        uint32_t* i = (uint32_t*)indexBuffer;
        return i[index];
    }
    else
    {
        LN_NOTIMPLEMENTED();
    }
}

void IndexBuffer::setResourceUsage(GraphicsResourceUsage usage)
{
	// Prohibit while direct locking.
	if (LN_REQUIRE(!m_rhiLockedBuffer)) return;
	if (m_usage != usage) {
		m_usage = usage;
		m_modified = true;
	}
}

void IndexBuffer::setResourcePool(GraphicsResourcePool pool)
{
	m_pool = pool;
}

detail::IIndexBuffer* IndexBuffer::resolveRHIObject()
{
	if (m_modified)
	{
		if (m_rhiLockedBuffer)
		{
			m_rhiObject->unmap();
			m_rhiLockedBuffer = nullptr;
		}
		else
		{
			size_t requiredSize = bytesSize();
			if (!m_rhiObject || m_rhiObject->getBytesSize() != requiredSize || m_rhiObject->usage() != m_usage)
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

	if (LN_ENSURE(m_rhiObject)) return nullptr;

	if (m_usage == GraphicsResourceUsage::Static && m_pool == GraphicsResourcePool::None) {
		m_buffer.clear();
		m_buffer.shrink_to_fit();
	}

	m_initialUpdate = false;
	m_modified = false;
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
