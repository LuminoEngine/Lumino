#include <LuminoCore/Object.hpp>

namespace ln {

Object::~Object() {
    if (m_registryIndex != 0) {
        LN_LOG_ERROR(
            "Object was not unregistered before destruction. registryIndex: %hu, generation: %hu",
            m_registryIndex,
            m_generation);
    }
}

} // namespace ln
