#include "Internal.hpp"
#include <LuminoEngine/Engine/Module.hpp>

namespace ln {

Module::Module()
    : m_context(nullptr) {
}

Module::~Module() {
}

void Module::onRegisterTypes(EngineContext2* context) {
}

} // namespace ln
