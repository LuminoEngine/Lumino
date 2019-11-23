
#include "Internal.hpp"
#include "EffectManager.hpp"
#include "EffekseerEffect.hpp"
#if LN_EFFEKSEER_ENABLED

namespace ln {
namespace detail {

EffekseerEffect::EffekseerEffect(::Effekseer::Effect* effect)
    : m_efkEffect(effect)
{}

EffekseerEffect::~EffekseerEffect()
{
    ES_SAFE_RELEASE(m_efkEffect);
}

//==============================================================================
// EffekseerEffectEmitter

EffekseerEffectEmitter::EffekseerEffectEmitter()
    : m_manager(nullptr)
    , m_effect(nullptr)
    , m_efkHandle(-1)
{
}

void EffekseerEffectEmitter::init(EffectManager* manager, EffekseerEffect* effect)
{
    EffectEmitter::init(nullptr);
    m_manager = manager;
    m_effect = effect;

    m_efkHandle = m_manager->effekseerManager()->Play(m_effect->effect(), 0, 0, 0);
}

void EffekseerEffectEmitter::onDispose(bool explicitDisposing)
{
    if (m_efkHandle >= 0) {
        m_manager->effekseerManager()->StopEffect(m_efkHandle);
        m_efkHandle = -1;
    }

    if (m_effect) {
        m_manager->releaseEffekseerEffect(m_effect);
        m_effect = nullptr;
    }

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
