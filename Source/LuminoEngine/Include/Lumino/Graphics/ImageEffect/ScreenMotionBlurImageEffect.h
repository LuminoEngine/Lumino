
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
	LN_OBJECT();
public:
	float	m_amount;

public:
	static ScreenMotionBlurImageEffectPtr create();

	/// ブラ―適用 (center はスクリーン座標空間で指定する(-1.0～1.0) )
	//void StartBlur(double duration, float power, float scale, const Vector3& center);

	/** 画像内に残す前のフレームの量 (0.0～1.0)。値が高くなるほど、モーショントレイルが長くなります。*/
	void setAmount(float amount) { m_amount = amount; }

	/** ブラーエフェクトを放射状に適用するための情報を設定します。*/
	void setRadialCenter(const Vector2& center) { m_center = center; }

	void setRadialScale(float scale) { m_scale = scale; }

	void setBlurStatus(float amount, const Vector2& center, float scale, float duration = 0.0f);

	//void StartBlurChange();

protected:
	ScreenMotionBlurImageEffect();
	virtual ~ScreenMotionBlurImageEffect();
	void initialize(detail::GraphicsManager* manager);
	virtual void onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

	Ref<Material>		m_material;
	RenderTargetTexture*	m_accumTexture;	// 前回の画面描画内容
	Vector2					m_center;
	float					m_scale;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
