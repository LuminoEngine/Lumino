#include <lumino_core/Object.hpp>

namespace ln {

Object::~Object() {
    if (registryIndex_ != 0) {
        LN_LOG_ERROR(
            "Object was not unregistered before destruction. registryIndex: %hu, generation: %hu",
            registryIndex_,
            generation_);
    }
}

} // namespace ln
