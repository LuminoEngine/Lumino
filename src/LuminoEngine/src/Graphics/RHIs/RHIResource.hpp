#pragma once
#include "RHIObject.hpp"

namespace ln {
namespace detail {

enum class RHIResourceType : uint8_t
{
	Unknown,
	VertexBuffer,
	IndexBuffer,
	UniformBuffer,
};

class RHIResource
	: public RHIDeviceObject
{
public:
	RHIResourceType resourceType() const { return m_type; }
	uint64_t memorySize() const { return m_memorySize; }

	// map/unmap は Usage=Streaming のみ許可。
	// 今は UniformBuffer が Usage にかかわらずそのような実装になっているのみ。
	virtual void* map();
	virtual void unmap();

protected:
	RHIResource();
	virtual ~RHIResource();
	bool initAsVertexBuffer(GraphicsResourceUsage usage, uint64_t memorySize);
	bool initAsIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, uint32_t indexCount);
	bool initAsUniformBuffer(GraphicsResourceUsage usage, uint64_t memorySize);

private:
	RHIResourceType m_type;
	GraphicsResourceUsage m_usage;
	uint64_t m_memorySize;
};

} // namespace detail
} // namespace ln

