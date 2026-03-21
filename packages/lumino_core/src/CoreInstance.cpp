#pragma once
#include <lumino_core/CoreInstance.hpp>

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
    return LN_MAKE_SUCCESS();
}

void CoreInstance::dispose() {
}

} // namespace ln
