
#include "Internal.hpp"
#include <LuminoPlatform/PlatformModule.hpp>
#include "PlatformManager.hpp"

namespace ln {
    
bool PlatformModule::initialize()
{
    return detail::PlatformManager::initialize();
}

void PlatformModule::terminate()
{
    detail::PlatformManager::terminate();
}

} // namespace ln
