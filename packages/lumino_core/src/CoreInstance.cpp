#pragma once
#include <lumino_core/CoreInstance.hpp>
#include <lumino_core/runtime/ObjectRegistry.hpp>

namespace ln {

std::unique_ptr<CoreInstance> CoreInstance::s_instance;

VoidResult CoreInstance::initialize(const Settings& settings) {
    if (s_instance) return LN_MAKE_SUCCESS();
    s_instance = std::unique_ptr<CoreInstance>(LN_NEW CoreInstance());
    return s_instance->init(settings);
}

void CoreInstance::terminate() {
    if (s_instance) {
        s_instance->dispose();
        s_instance = nullptr;
    }
}

VoidResult CoreInstance::init(const Settings& settings) {
    m_settings = settings;
    m_objectRegistry = std::make_unique<ObjectRegistry>();
    return LN_MAKE_SUCCESS();
}

void CoreInstance::dispose() {
    m_objectRegistry.reset();
}

} // namespace ln
