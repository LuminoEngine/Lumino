#pragma once
#include "ImageEffect.hpp"
#include "../Animation/EasingFunctions.hpp"

namespace ln {

class ScreenBlurImageEffect
	: public ImageEffect
{
public:
    static Ref<ScreenBlurImageEffect> create();

    /** 画像内に残す前のフレームの量 (0.0～1.0)。値が高くなるほど、モーショントレイルが長くなります。(default:0.0) */
    void setAmount(float amount) { m_amountValue.start(amount, 0); }

    /** ブラーエフェクトを放射状に適用するための情報を設定します。(default:Vector2::Zero) */
    void setRadialCenter(const Vector2& center) { m_center = center; }

    void setRadialScale(float scale) { m_scale = scale; }

    // [experimental]
    void play(float amount, const Vector2& center, float scale, float duration = 0.0f);

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    ScreenBlurImageEffect();
	virtual ~ScreenBlurImageEffect();
	void init();

private:
	Ref<Material> m_materialForCopySourceTo;
	Ref<Material> m_materialForCopyAccumTo;
    Ref<Material> m_material;
    //float m_amount;
    EasingValue<float> m_amountValue;
    Ref<RenderTargetTexture> m_accumTexture;	// Save previous screen
    Vector2 m_center;
    float m_scale;
};

} // namespace ln

