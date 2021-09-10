
#include "Internal.hpp"
#include <LuminoPlatform/PlatformModule.hpp>
#include "PlatformManager.hpp"

namespace ln {
    
PlatformModule* PlatformModule::initialize()
{
    detail::PlatformManager::Settings s;
    return detail::PlatformManager::initialize(s);
}

void PlatformModule::terminate()
{
    detail::PlatformManager::terminate();
}

} // namespace ln
