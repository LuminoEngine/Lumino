#pragma once
#include "../Animation/EasingFunctions.hpp"
#include "ImageEffect.hpp"

namespace ln {
namespace detail { class ToneImageEffectInstance; }

class ToneImageEffect
	: public ImageEffect
{
public:
    static Ref<ToneImageEffect> create();

    // [experimental]
    void play(const ColorTone& tone, double time);

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual Ref<ImageEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    ToneImageEffect();
	virtual ~ToneImageEffect();
	void init();

private:
    EasingValue<Vector4> m_toneValue;

    friend class detail::ToneImageEffectInstance;
};

namespace detail {

class ToneImageEffectInstance
    : public ImageEffectInstance
{
protected:
    void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    ToneImageEffectInstance();
    bool init(ToneImageEffect* owner);

private:
    ToneImageEffect* m_owner;
    Ref<Material> m_material;
};

} // namespace detail
} // namespace ln

