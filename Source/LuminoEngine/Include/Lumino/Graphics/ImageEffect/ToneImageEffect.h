
#pragma once
#include "../../Animation/EasingValue.h"
#include "../Color.h"
#include "ImageEffect.h"

LN_NAMESPACE_BEGIN

/**
	@brief
*/
class ToneImageEffect
	: public ImageEffect
{
	LN_OBJECT;
public:
	Vector4	m_tone;

public:
	static ToneImageEffectPtr create();

	void setTone(const ToneF& tone);

	void changeTone(const ToneF& tone, double time);

protected:
	ToneImageEffect();
	virtual ~ToneImageEffect();
	void initialize(detail::GraphicsManager* manager);
	virtual void onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

private:
	Ref<CommonMaterial>	m_material;
};

LN_NAMESPACE_END
