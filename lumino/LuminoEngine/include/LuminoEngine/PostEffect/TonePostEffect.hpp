#pragma once
#include <LuminoEngine/Animation/EasingFunctions.hpp>
#include "PostEffect.hpp"

namespace ln {
namespace detail { class TonePostEffectInstance; }

class TonePostEffect
	: public PostEffect
{
public:
    static Ref<TonePostEffect> create();

    // [experimental]
    void play(const ColorTone& tone, double time);

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual Ref<PostEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    TonePostEffect();
	virtual ~TonePostEffect();
	void init();

private:
    EasingValue<Vector4> m_toneValue;

    friend class detail::TonePostEffectInstance;
};

namespace detail {

class TonePostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    TonePostEffectInstance();
    bool init(TonePostEffect* owner);

private:
    TonePostEffect* m_owner;
    Ref<Material> m_material;
};

} // namespace detail
} // namespace ln

