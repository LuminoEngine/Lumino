
#include "Internal.hpp"
#include "GraphicsDeviceContext.hpp"
#include "RHIProfiler.hpp"
#include "RHIBuffer.hpp"

namespace ln {
namespace detail {
	
//=============================================================================
// RHIBuffer

RHIBuffer::RHIBuffer()
	: m_type(RHIBufferType::VertexBuffer)
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

bool RHIBuffer::init(RHIBufferType type, uint64_t memorySize)
{
	m_type = type;
	m_memorySize = memorySize;
	return true;
}

} // namespace detail
} // namespace ln

