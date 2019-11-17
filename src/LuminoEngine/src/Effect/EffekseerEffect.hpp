#pragma once
#include <LuminoEngine/Effect/Common.hpp>

#if LN_EFFEKSEER_ENABLED
#include <LuminoEngine/Effect/EffectModel.hpp>
#include <Effekseer.h>

namespace ln {
namespace detail {

class EffekseerEffectEmitter
    : public EffectEmitter
{
protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual bool onUpdate(float localTime, float elapsedSeconds) override;
    virtual void onRender(RenderingContext* renderingContext) override;

LN_CONSTRUCT_ACCESS:
    EffekseerEffectEmitter();
    void init(EffectManager* manager, ::Effekseer::Effect* efkEffect);

private:
    EffectManager* m_manager;   // TODO: EffectEmitter に持って行った方がいいかも
    ::Effekseer::Effect* m_efkEffect;
    ::Effekseer::Handle m_efkHandle;
};

} // namespace detail
} // namespace ln

#endif
