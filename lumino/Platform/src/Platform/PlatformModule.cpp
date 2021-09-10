
#include "Internal.hpp"
#include <LuminoPlatform/PlatformModule.hpp>
#include "PlatformManager.hpp"

namespace ln {
    
PlatformModule* PlatformModule::initialize()
{
    if (detail::PlatformManager::instance()) return detail::PlatformManager::instance();

    auto m = makeRef<detail::PlatformManager>();
    detail::PlatformManager::Settings s;
    if (!m->init(s)) return nullptr;
    
    EngineContext2::instance()->registerModule(m);
    EngineContext2::instance()->platformManager = m;
    return m;
}

void PlatformModule::terminate()
{
    auto m = static_cast<detail::PlatformManager*>(EngineContext2::instance()->platformManager);
    if (m) {
        m->dispose();
        EngineContext2::instance()->unregisterModule(m);
        EngineContext2::instance()->platformManager = nullptr;
    }
}

} // namespace ln
