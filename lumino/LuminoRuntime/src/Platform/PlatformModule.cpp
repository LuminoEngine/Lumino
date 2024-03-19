
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformModule.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>

namespace ln {

PlatformModule* PlatformModule::initialize(const PlatformModuleSettings& settings) {
    detail::PlatformManager::Settings s(settings);
    return detail::PlatformManager::initialize(s);
}

void PlatformModule::terminate() {
    detail::PlatformManager::terminate();
}

} // namespace ln
