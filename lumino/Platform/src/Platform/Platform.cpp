
#include "Internal.hpp"
#include <LuminoPlatform/Platform.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformManager.hpp>

namespace ln {
    
bool Platform::processEvents()
{
    detail::PlatformManager::instance()->processSystemEventQueue();
    return !detail::PlatformManager::instance()->shouldQuit();
}

PlatformWindow* Platform::mainWindow()
{
    return detail::PlatformManager::instance()->mainWindow();
}

} // namespace ln
