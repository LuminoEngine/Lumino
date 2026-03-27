#include "pch.hpp"
#include <mutex>
#include <lumino_base/Assertion.hpp>
#include <lumino_base/RefObject.hpp>

namespace ln {

void RefObject::finalize() {
    LN_ASSERT(refCount_ == 0);
}

void RefObject::addRef() const {
    refCount_.fetch_add(1, std::memory_order_relaxed);
}

void RefObject::release() const {
    if (refCount_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete this;
    }
}

} // namespace ln
