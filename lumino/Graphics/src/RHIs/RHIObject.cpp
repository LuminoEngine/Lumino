
#include "Internal.hpp"
#include "RHIObject.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// RHIDeviceObject

RHIDeviceObject::RHIDeviceObject()
    : m_device(nullptr)
	, m_objectId(0)
	, m_disposed(false)
	, m_profiling(false)
{
}

RHIDeviceObject::~RHIDeviceObject()
{
    if (!m_disposed) {
        LN_LOG_ERROR("object [0x{:x}] is not disposed", (intptr_t)this);
    }
}

void RHIDeviceObject::finalize()
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

void RHIDeviceObject::dispose()
{
    m_disposed = true;
}

} // namespace detail
} // namespace ln

