#pragma once
#include "RHIObject.hpp"

namespace ln {
namespace detail {

enum class RHIBufferType : uint8_t
{
	Unknown,
	VertexBuffer,
	IndexBuffer,
	UniformBuffer,
};

class RHIBuffer
	: public IGraphicsDeviceObject
{
public:
	RHIBufferType resourceType() const { return m_type; }
	uint64_t memorySize() const { return m_memorySize; }

protected:
	RHIBuffer();
	virtual ~RHIBuffer();
	bool initAsVertexBuffer(GraphicsResourceUsage usage, uint64_t memorySize);
	bool initAsIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, uint32_t indexCount);
	bool initAsUniformBuffer(GraphicsResourceUsage usage, uint64_t memorySize);

private:
	RHIBufferType m_type;
	GraphicsResourceUsage m_usage;
	uint64_t m_memorySize;
};

class IUniformBuffer
	: public IGraphicsDeviceObject
{
public:
	virtual void* map() = 0;
	virtual void unmap() = 0;

protected:
	virtual ~IUniformBuffer();
};

} // namespace detail
} // namespace ln

