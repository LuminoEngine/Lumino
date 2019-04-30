#pragma once
#include "../Animation/EasingFunctions.hpp"
#include "ImageEffect.hpp"

namespace ln {
class Material;

class ToneImageEffect
	: public ImageEffect
{
public:
    static Ref<ToneImageEffect> create();

    // [experimental]
    void play(const ColorTone& tone, double time);

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    ToneImageEffect();
	virtual ~ToneImageEffect();
	void init();

private:
    Ref<Material> m_material;
    EasingValue<Vector4> m_toneValue;
};

} // namespace ln

