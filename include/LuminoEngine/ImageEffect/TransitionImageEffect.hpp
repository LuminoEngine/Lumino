#pragma once
#include "../Animation/EasingFunctions.hpp"
#include "ImageEffect.hpp"

namespace ln {
class Material;

class TransitionImageEffect
	: public ImageEffect
{
public:
    static Ref<TransitionImageEffect> create();

    void startFadeOut(float duration);
    void startFadeIn(float duration);
    void startCrossFade(float duration, int vague = 20);

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    TransitionImageEffect();
	void init();

private:
    enum class Mode
    {
        FadeOut,
        FadeIn,
        CrossFade,
    };

    bool preparePreviousFrameTarget(int width, int height);
    void renderFadeInOut(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination, float factor);
    void renderCrossFade(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination);

    Mode m_mode;

    // Rendering result of previous frame.
    Ref<RenderTargetTexture> m_previousFrameTarget;

    // Transition source image.
    Ref<RenderTargetTexture> m_overrayTarget;

    // 0:no effect, 1:full overray
    EasingValue<float> m_factor;

    Ref<Material> m_withoutMaskMaterial;
    Ref<Material> m_copyMaterial;

    // freeze を次回の onRender の中で行うかどうか
    bool m_freezeRequested;
};

} // namespace ln

