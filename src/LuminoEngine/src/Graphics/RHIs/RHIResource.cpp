
#include "Internal.hpp"
#include "GraphicsDeviceContext.hpp"
#include "RHIProfiler.hpp"
#include "RHIResource.hpp"

namespace ln {
namespace detail {
	
//=============================================================================
// RHIResource

RHIResource::RHIResource()
	: m_type(RHIResourceType::Unknown)
	, m_usage(GraphicsResourceUsage::Static)
	, m_memorySize(0)
{
	LN_LOG_VERBOSE << "RHIResource [0x" << this << "] constructed.";
}

RHIResource::~RHIResource()
{
	if (IGraphicsDevice* d = device()) {
		switch (m_type)
		{
		case RHIResourceType::VertexBuffer:
			d->profiler()->removeVertexBuffer(this);
			break;
		case RHIResourceType::IndexBuffer:
			d->profiler()->removeIndexBuffer(this);
			break;
		case RHIResourceType::UniformBuffer:
			d->profiler()->removeUniformBuffer(this);
			break;
		default:
			break;
		}
	}
}

bool RHIResource::initAsVertexBuffer(GraphicsResourceUsage usage, uint64_t memorySize)
{
	m_type = RHIResourceType::VertexBuffer;
	m_usage = usage;
	m_memorySize = memorySize;
	return true;
}

bool RHIResource::initAsIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, uint32_t indexCount)
{
	m_type = RHIResourceType::IndexBuffer;
	m_usage = usage;

	int stride = 0;
	if (format == IndexBufferFormat::UInt16) {
		stride = 2;
	}
	else if (format == IndexBufferFormat::UInt32) {
		stride = 4;
	}
	else {
		LN_UNREACHABLE();
		return false;
	}
	m_memorySize = stride * indexCount;

	return true;
}

bool RHIResource::initAsUniformBuffer(GraphicsResourceUsage usage, uint64_t memorySize)
{
	// 今のところ UniformBuffer は Dynamic (複数 DrawCall で共有しない) 前提
	// TODO: OpenGL の Streaming みたいにもうひとつ用意して区別したほうがいいかも
	if (LN_REQUIRE(usage == GraphicsResourceUsage::Dynamic)) return false;
	m_type = RHIResourceType::UniformBuffer;
	m_usage = usage;
	m_memorySize = memorySize;
	return true;
}

void* RHIResource::map()
{
	LN_UNREACHABLE();
	return nullptr;
}

void RHIResource::unmap()
{
	LN_UNREACHABLE();
}

} // namespace detail
} // namespace ln

