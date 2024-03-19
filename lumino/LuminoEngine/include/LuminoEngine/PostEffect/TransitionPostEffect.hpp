#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Animation/EasingFunctions.hpp>
#include "PostEffect.hpp"

namespace ln {
class Texture;
namespace detail { class TransitionPostEffectInstance; }

class TransitionPostEffect
	: public PostEffect
{
public:
    static Ref<TransitionPostEffect> create();

    void setFadeColor(const Color& value) { m_fadeColor = value; }
    const Color& fadeColor() const { return m_fadeColor; }
    void setMaskTexture(Texture* value);
    Texture* maskTexture() const;
    void setVague(float value) { m_vague = value; }
    float vague() const { return m_vague; }
    void startFadeOut(float duration);
    void startFadeIn(float duration);
    void startCrossFade(float duration);

    bool isRunning() const { return !m_factor.isFinished(); }

public:	// TODO: protected
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual Ref<PostEffectInstance> onCreateInstance() override;
    //virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;
    //virtual void onPostRender() override;

LN_CONSTRUCT_ACCESS:
    TransitionPostEffect();
	void init();

private:
    enum class Mode
    {
        FadeOut,
        FadeIn,
        CrossFade,
    };


    Mode m_mode;

    Color m_fadeColor;
    Ref<Texture> m_maskTexture;

    // 0:no effect, 1:full overray
    EasingValue<float> m_factor;
    float m_vague;

    int m_freezeRevision;

    friend class detail::TransitionPostEffectInstance;
};

namespace detail {

class TransitionPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    TransitionPostEffectInstance();
    bool init(TransitionPostEffect* owner);

private:
    bool preparePreviousFrameTarget(int width, int height);
    void renderFadeInOut(CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination, float factor);
    void renderCrossFade(CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination);

    TransitionPostEffect* m_owner;

    // Rendering result of previous frame.
    Ref<RenderTargetTexture> m_previousFrameTarget;

    // Transition source image.
    Ref<RenderTargetTexture> m_overrayTarget;

    Ref<Material> m_withoutMaskMaterial;
    Ref<Material> m_withMaskMaterial;
    Ref<Material> m_copyMaterial;
    Texture* m_maskTexture;
    int m_freezeRevision;
};

} // namespace detail
} // namespace ln

