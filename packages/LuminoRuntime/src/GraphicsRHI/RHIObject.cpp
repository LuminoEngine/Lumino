#include <LuminoEngine/GraphicsRHI/RHIObject.hpp>

namespace ln {
namespace detail {

//==============================================================================
// RHIDeviceObject

RHIDeviceObject::RHIDeviceObject()
    : m_device(nullptr)
    , m_objectId(0)
    , m_destroyed(false)
    , m_profiling(false) {
}

RHIDeviceObject::~RHIDeviceObject() {
    if (!m_destroyed) {
        LN_LOG_ERROR("object [0x{:x}] is not destroyed", (intptr_t)this);
    }
}

void RHIDeviceObject::finalize() {
#ifdef LN_DEBUG
    bool d = m_destroyed;
    destroy();
    if (!d) {
        LN_CHECK(m_destroyed);
    }
#else
    destroy();
#endif
    m_objectId = 0;
}

void RHIDeviceObject::destroy() {
    if (!m_destroyed) {
        onDestroy();
    }
    m_destroyed = true;
}

} // namespace detail
} // namespace ln
