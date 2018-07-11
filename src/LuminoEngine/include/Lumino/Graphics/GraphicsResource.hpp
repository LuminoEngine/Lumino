#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class GraphicsManager;
class IGraphicsDeviceContext;
}

/**
	Graphics 機能に関係するリソースのベースクラスです。

*/
class LN_API GraphicsResource
	: public Object
{
public:

	virtual void dispose() override;

protected:
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) = 0;

LN_CONSTRUCT_ACCESS:
	GraphicsResource();
	virtual ~GraphicsResource();
	void initialize();

LN_INTERNAL_ACCESS:
	detail::GraphicsManager* manager() const { return m_manager; }
	detail::IGraphicsDeviceContext* deviceContext() const;
private:
	detail::GraphicsManager* m_manager;
};

class LN_API GraphicsBufferResource
	: public GraphicsResource
{
public:
	int size() const;
	void reserve(int size);
	void resize(int size);
	void* map(MapMode mode);
	void clear();

protected:
	GraphicsBufferResource();
	virtual ~GraphicsBufferResource();
	void initialize(GraphicsResourceUsage usage, GraphicsResourcePool pool, const void* initialData, size_t initialDataSize);

	bool isRHIDirect() const { return m_initialUpdate && m_rhiObject != nullptr; }
	void resolveRHIObjectInternal();
	virtual detail::IGraphicsDeviceObject* createRHIObject(const void* initialData, size_t initialDataSize) = 0;
	virtual size_t getRHIBufferSize() = 0;
	virtual void* mapNative() = 0;
	virtual void unmapNative() = 0;
	virtual void setSubDataNative(size_t offset, const void* data, size_t size) = 0;

private:
	GraphicsResourceUsage			m_usage;
	GraphicsResourcePool	m_pool;
	std::vector<byte_t>		m_buffer;
	size_t					m_rhiBufferByteSize;
	detail::IGraphicsDeviceObject* m_rhiObject;
	void*					m_rhiLockedBuffer;
	bool					m_initialUpdate;
	bool m_mapping;
	bool					m_modified;
};

} // namespace ln
