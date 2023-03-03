#include <LuminoGraphicsRHI/detail/GraphicsRHIManager.hpp> 

namespace ln {
namespace detail {

//==============================================================================
// GraphicsRHIManager

GraphicsRHIManager* GraphicsRHIManager::s_instance = nullptr;

GraphicsRHIManager* GraphicsRHIManager::initialize(const Settings& settings) {
    if (s_instance) return s_instance;

    auto m = Ref<GraphicsRHIManager>(LN_NEW GraphicsRHIManager(), false);
    if (!m->init(settings)) return nullptr;
    return m;
}

void GraphicsRHIManager::terminate() {
    if (s_instance) {
        s_instance->dispose();
        s_instance = nullptr;
    }
}

GraphicsRHIManager::GraphicsRHIManager() {
}

Result<> GraphicsRHIManager::init(const Settings& settings) {
    return ok();
}

void GraphicsRHIManager::dispose() {
}

} // namespace detail
} // namespace ln
