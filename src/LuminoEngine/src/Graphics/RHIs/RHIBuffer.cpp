
#include "Internal.hpp"
#include "GraphicsDeviceContext.hpp"
#include "RHIProfiler.hpp"
#include "RHIBuffer.hpp"

namespace ln {
namespace detail {
	
//=============================================================================
// RHIBuffer

RHIBuffer::RHIBuffer()
	: m_type(RHIBufferType::Unknown)
	, m_usage(GraphicsResourceUsage::Static)
	, m_memorySize(0)
{
	LN_LOG_VERBOSE << "RHIBuffer [0x" << this << "] constructed.";
}

RHIBuffer::~RHIBuffer()
{
	if (IGraphicsDevice* d = device()) {
		switch (m_type)
		{
		case RHIBufferType::VertexBuffer:
			d->profiler()->removeVertexBuffer(this);
			break;
		case RHIBufferType::IndexBuffer:
			d->profiler()->removeIndexBuffer(this);
			break;
		case RHIBufferType::UniformBuffer:
			break;
		default:
			break;
		}
	}
}

bool RHIBuffer::initAsVertexBuffer(GraphicsResourceUsage usage, uint64_t memorySize)
{
	m_type = RHIBufferType::VertexBuffer;
	m_usage = usage;
	m_memorySize = memorySize;
	return true;
}

bool RHIBuffer::initAsIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, uint32_t indexCount)
{
	m_type = RHIBufferType::IndexBuffer;
	m_usage = usage;

	// Buffer Size
	int stride = 0;
	if (format == IndexBufferFormat::UInt16) {
		//m_indexFormat = DXGI_FORMAT_R16_UINT;
		stride = 2;
	}
	else if (format == IndexBufferFormat::UInt32) {
		//m_indexFormat = DXGI_FORMAT_R32_UINT;
		stride = 4;
	}
	else {
		LN_UNREACHABLE();
		return false;
	}

	m_memorySize = stride * indexCount;

	return true;
}

bool RHIBuffer::initAsUniformBuffer(GraphicsResourceUsage usage, uint64_t memorySize)
{
	// 今のところ UniformBuffer は Dynamic (複数 DrawCall で共有しない) 前提
	// TODO: OpenGL の Streaming みたいにもうひとつ用意して区別したほうがいいかも
	if (LN_REQUIRE(usage == GraphicsResourceUsage::Dynamic)) return false;
	m_type = RHIBufferType::UniformBuffer;
	m_usage = usage;
	m_memorySize = memorySize;
	return true;
}

} // namespace detail
} // namespace ln

