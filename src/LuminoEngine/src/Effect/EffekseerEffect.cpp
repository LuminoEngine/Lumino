
#include "Internal.hpp"
#include "EffectManager.hpp"
#include "EffekseerEffect.hpp"
#if LN_EFFEKSEER_ENABLED

namespace ln {
namespace detail {

//==============================================================================
// EffekseerEffectEmitter

EffekseerEffectEmitter::EffekseerEffectEmitter()
    : m_manager(nullptr)
    , m_efkEffect(nullptr)
    , m_efkHandle(-1)
{
}

void EffekseerEffectEmitter::init(EffectManager* manager, ::Effekseer::Effect* efkEffect)
{
    EffectEmitter::init(nullptr);
    m_manager = manager;
    m_efkEffect = efkEffect;
    ES_SAFE_ADDREF(m_efkEffect);

    m_efkHandle = m_manager->effekseerManager()->Play(m_efkEffect, 0, 0, 0);
}

void EffekseerEffectEmitter::onDispose(bool explicitDisposing)
{
    if (m_efkHandle >= 0) {
        m_manager->effekseerManager()->StopEffect(m_efkHandle);
        m_efkHandle = -1;
    }

    ES_SAFE_RELEASE(m_efkEffect);

    EffectEmitter::onDispose(explicitDisposing);
}

bool EffekseerEffectEmitter::onUpdate(float localTime, float elapsedSeconds)
{
    if (m_manager->effekseerManager()->Exists(m_efkHandle)) {
        m_manager->effekseerManager()->SetLocation(m_efkHandle, ::Effekseer::Vector3D(position().x, position().y, position().z));
        return true;
    }
    else {
        return false;
    }
}

void EffekseerEffectEmitter::onRender(RenderingContext* renderingContext)
{
    // Rendered by EffectManager
}

} // namespace detail
} // namespace ln

#endif
