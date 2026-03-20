
#pragma once
#include "../../Effect/ParticleEffectModel.hpp"
#include "../../Effect/ParticleEffectModel2.hpp"
#include "../../Visual/VisualComponent.hpp"

namespace ln {
namespace detail {
class ParticleInstance2;
}

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


class ParticleEmitterComponent2
    : public VisualComponent
{
LN_CONSTRUCT_ACCESS:
    ParticleEmitterComponent2();
    void init(ParticleModel* model);

protected:
    virtual void onUpdate(float elapsedSeconds) override;
    virtual void onRender(RenderingContext* context) override;

private:
    Ref<ParticleModel> m_model;
    Ref<detail::ParticleInstance2> m_instance;
};

} // namespace ln
