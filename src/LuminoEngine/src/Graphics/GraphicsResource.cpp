
#include "Internal.hpp"
#include <Lumino/Graphics/GraphicsResource.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {

//==============================================================================
// GraphicsResource

GraphicsResource::GraphicsResource()
	: m_manager(nullptr)
{

}

GraphicsResource::~GraphicsResource()
{

}

void GraphicsResource::initialize()
{
	if (LN_REQUIRE(!m_manager)) return;
	m_manager = detail::EngineDomain::graphicsManager();
	m_manager->addGraphicsResource(this);
}

void GraphicsResource::dispose()
{
	if (m_manager) {
		m_manager->removeGraphicsResource(this);
		m_manager = nullptr;
	}
	Object::dispose();
}

detail::IGraphicsDeviceContext* GraphicsResource::deviceContext() const
{
	return m_manager->deviceContext();
}

//==============================================================================
// GraphicsBufferResource

GraphicsBufferResource::GraphicsBufferResource()
{
}

GraphicsBufferResource::~GraphicsBufferResource()
{
}

int GraphicsBufferResource::size() const
{
	return static_cast<int>(m_buffer.size());
}

void GraphicsBufferResource::reserve(int size)
{
	if (LN_REQUIRE(!isRHIDirect())) return;		// サイズ変更禁止

	size_t newSize = static_cast<size_t>(size);
	if (newSize != m_buffer.capacity())
	{
		m_buffer.reserve(newSize);
	}
}

void GraphicsBufferResource::resize(int size)
{
	if (LN_REQUIRE(!isRHIDirect())) return;		// サイズ変更禁止

	size_t newSize = static_cast<size_t>(size);
	if (newSize != m_buffer.size())
	{
		m_buffer.resize(newSize);
	}
}

void* GraphicsBufferResource::map(MapMode mode)
{
	if (LN_REQUIRE(!(m_usage == GraphicsResourceUsage::Static && mode == MapMode::Read))) return nullptr;

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
void GraphicsBufferResource::clear()
{
	m_buffer.clear();
	m_modified = true;
}


void GraphicsBufferResource::initialize(GraphicsResourceUsage usage, GraphicsResourcePool pool, const void* initialData, size_t initialDataSize)
{
	GraphicsResource::initialize();

	if (initialData) {
		m_rhiObject = createRHIObject(initialData, initialDataSize);
	}

	m_rhiBufferByteSize = initialDataSize;

	m_initialUpdate = true;
}

void GraphicsBufferResource::resolveRHIObjectInternal()
{
	if (m_modified)
	{
		if (isRHIDirect() && m_mapping)
		{
			unmapNative();
		}
		else
		{
			if (m_rhiObject == nullptr || m_rhiBufferByteSize != m_buffer.size())
			{
				m_rhiObject = createRHIObject(m_buffer.data(), m_buffer.size());
				m_rhiBufferByteSize = m_buffer.size();
			}
			else
			{
				setSubDataNative(0, m_buffer.data(), m_buffer.size());
			}
		}
	}

	if (LN_ENSURE(m_rhiObject)) return;

	m_initialUpdate = false;
	m_modified = false;
}

} // namespace ln
