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

    void start(float duration, int vague = 20);

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    TransitionImageEffect();
	void init();

private:
    bool preparePreviousFrameTarget(int width, int height);


    // Rendering result of previous frame.
    Ref<RenderTargetTexture> m_previousFrameTarget;

    // Transition source image.
    Ref<RenderTargetTexture> m_overrayTarget;


    float m_factor;

    Ref<Material> m_withoutMaskMaterial;
    Ref<Material> m_copyMaterial;

    // freeze を次回の onRender の中で行うかどうか
    bool m_freezeRequested;
};

} // namespace ln

