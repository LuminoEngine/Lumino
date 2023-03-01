#include <LuminoGraphics/RHIModule.hpp>
#include <LuminoGraphics/detail/GraphicsManager.hpp>

namespace ln {

GraphicsModule* GraphicsModule::initialize(const GraphicsModuleSettings& settings) {
    detail::GraphicsManager::Settings s = settings;
    return detail::GraphicsManager::initialize(s);
}

void GraphicsModule::terminate() {
    detail::GraphicsManager::terminate();
}

} // namespace ln
