
#include "Internal.hpp"
#include <LuminoPlatform/Platform.hpp>
#include "PlatformManager.hpp"

namespace ln {
    
void Platform::processEvents()
{
    detail::PlatformManager::instance()->processSystemEventQueue();
}

PlatformWindow* Platform::mainWindow()
{
    return detail::PlatformManager::instance()->mainWindow();
}

} // namespace ln
