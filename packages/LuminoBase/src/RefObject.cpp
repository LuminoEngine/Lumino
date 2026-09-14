#include "pch.hpp"
#include <LuminoBase/RefObject.hpp>

namespace ln {

void RefObject::finalize() {
    assert(m_refCount == 0);
}

void RefObject::addRef() {
    m_refCount.fetch_add(1, std::memory_order_relaxed);
}

void RefObject::release() {
    if (m_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        finalize();
        delete this;
    }
}

} // namespace ln
