
#pragma once
#include "ImageEffect.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief
*/
class ScreenMotionBlurImageEffect
	: public ImageEffect
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	LN_TR_PROPERTY(float, Amount);
public:
	static ScreenMotionBlurImageEffectPtr Create();

	/// ブラ―適用 (center はスクリーン座標空間で指定する(-1.0～1.0) )
	//void StartBlur(double duration, float power, float scale, const Vector3& center);

	/** 画像内に残す前のフレームの量 (0.0～1.0)。値が高くなるほど、モーショントレイルが長くなります。*/
	void SetAmount(float amount) { tr::PropertyInfo::SetPropertyValueDirect(this, AmountId, amount);/*m_amount = amount;*/ }

	/** ブラーエフェクトを放射状に適用するための情報を設定します。*/
	void SetRadialCenter(const Vector2& center) { m_center = center; }

	void SetRadialScale(float scale) { m_scale = scale; }

	void SetBlurStatus(float amount, const Vector2& center, float scale, float duration = 0.0f);

	//void StartBlurChange();

protected:
	ScreenMotionBlurImageEffect();
	virtual ~ScreenMotionBlurImageEffect();
	void Initialize(detail::GraphicsManager* manager);
	virtual void OnRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

	RefPtr<Material>		m_material;
	RenderTargetTexture*	m_accumTexture;	// 前回の画面描画内容
	Vector2					m_center;
	float					m_scale;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
