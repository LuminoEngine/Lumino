
#include "Internal.hpp"
#include "RHIObject.hpp"

namespace ln {
namespace detail {
	
//=============================================================================
// IGraphicsDeviceObject

IGraphicsDeviceObject::IGraphicsDeviceObject()
    : m_device(nullptr)
	, m_objectId(0)
	, m_disposed(false)
	, m_profiling(false)
{
}

IGraphicsDeviceObject::~IGraphicsDeviceObject()
{
    if (!m_disposed) {
        LN_LOG_ERROR << "object [0x" << this << "] is not disposed";
    }
}

void IGraphicsDeviceObject::finalize()
{
#ifdef LN_DEBUG
    bool d = m_disposed;
    dispose();
    if (!d) {
        LN_CHECK(m_disposed);
    }
#else
    dispose();
#endif
	m_objectId = 0;
}

void IGraphicsDeviceObject::dispose()
{
    m_disposed = true;
}

} // namespace detail
} // namespace ln

