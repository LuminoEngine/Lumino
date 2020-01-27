
#pragma once
#include "../Effect/ParticleEffectModel.hpp"
#include "VisualComponent.hpp"

namespace ln {

/**
    パーティクルエミッタのクラスです。
    @detail		パーティクルエミッタは、実際に表示されるパーティクルの放出点となる VisualNode です。
*/
class ParticleEmitterComponent
    : public VisualComponent
{
LN_CONSTRUCT_ACCESS:
    ParticleEmitterComponent();
    virtual ~ParticleEmitterComponent();
    void init(SpriteParticleModel* model);

protected:
    virtual void onUpdate(float elapsedSeconds) override;
    virtual void onRender(RenderingContext* context) override;

private:
    Ref<SpriteParticleModel> m_model;
    Ref<detail::SpriteParticleModelInstance> m_instance;
};

} // namespace ln
