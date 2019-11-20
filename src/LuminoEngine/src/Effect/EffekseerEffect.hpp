#pragma once
#include <LuminoEngine/Effect/Common.hpp>

#if LN_EFFEKSEER_ENABLED
#include <LuminoEngine/Effect/EffectModel.hpp>
#include <Effekseer.h>

namespace ln {
namespace detail {

// Wrap Effekseer::Effect for cache management
class EffekseerEffect
    : public RefObject
{
public:
    EffekseerEffect(::Effekseer::Effect* effect);
    ~EffekseerEffect();
    ::Effekseer::Effect* effect() const { return m_efkEffect; }

private:
    ::Effekseer::Effect* m_efkEffect;
};

class EffekseerEffectEmitter
    : public EffectEmitter
{
protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual bool onUpdate(float localTime, float elapsedSeconds) override;
    virtual void onRender(RenderingContext* renderingContext) override;

LN_CONSTRUCT_ACCESS:
    EffekseerEffectEmitter();
    void init(EffectManager* manager, EffekseerEffect* effect);

private:
    EffectManager* m_manager;   // TODO: EffectEmitter に持って行った方がいいかも
    Ref<EffekseerEffect> m_effect;
    ::Effekseer::Handle m_efkHandle;
};

} // namespace detail
} // namespace ln

#endif
