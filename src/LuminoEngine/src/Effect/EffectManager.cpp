
#include "Internal.hpp"
#include <LuminoEngine/Effect/EffectContext.hpp>
#include "EffectManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// EffectManager

EffectManager::EffectManager()
{
}

void EffectManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "EffectManager Initialization started.";


    LN_LOG_DEBUG << "EffectManager Initialization ended.";
}

void EffectManager::dispose()
{
}

} // namespace detail
} // namespace ln

