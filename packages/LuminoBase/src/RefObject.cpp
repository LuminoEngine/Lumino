#include "pch.hpp"
#include <LuminoBase/RefObject.hpp>

namespace ln {

void RefObject::finalize() {
    assert(refCount_ == 0);
}

void RefObject::addRef() {
    refCount_.fetch_add(1, std::memory_order_relaxed);
}

void RefObject::release() {
    if (refCount_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        finalize();
        delete this;
    }
}

} // namespace ln
