#pragma once
#include <LuminoEngine/Animation/EasingFunctions.hpp>
#include "PostEffect.hpp"

namespace ln {
namespace detail { class ScreenBlurPostEffectInstance; }

class ScreenBlurPostEffect
	: public PostEffect
{
public:
    static Ref<ScreenBlurPostEffect> create();

    /** 画像内に残す前のフレームの量 (0.0～1.0)。値が高くなるほど、モーショントレイルが長くなります。(default:0.0) */
    void setAmount(float amount) { m_amountValue.start(amount, 0); }

    /** ブラーエフェクトを放射状に適用するための情報を設定します。(default:Vector2::Zero) */
    void setRadialCenter(const Vector2& center) { m_center = center; }

    void setRadialScale(float scale) { m_scale = scale; }

    // [experimental]
    void play(float amount, const Vector2& center, float scale, float duration = 0.0f);

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual Ref<PostEffectInstance> onCreateInstance() override;
    //virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    ScreenBlurPostEffect();
	virtual ~ScreenBlurPostEffect();
	void init();

private:
    //float m_amount;
    EasingValue<float> m_amountValue;
    Vector2 m_center;
    float m_scale;

    friend class detail::ScreenBlurPostEffectInstance;
};

namespace detail {

class ScreenBlurPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    ScreenBlurPostEffectInstance();
    bool init(ScreenBlurPostEffect* owner);

private:
    ScreenBlurPostEffect* m_owner;

    Ref<Material> m_materialForCopySourceTo;
    Ref<Material> m_materialForCopyAccumTo;
    Ref<Material> m_material;
    Ref<RenderTargetTexture> m_accumTexture;	// Save previous screen
};

} // namespace detail
} // namespace ln

