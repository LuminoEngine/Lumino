
#pragma once
#include "../../Animation/EasingValue.h"
#include "../Color.h"
#include "ImageEffect.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief
*/
class ToneImageEffect
	: public ImageEffect
{
public:
	static ToneImageEffectPtr Create();

	void SetTone(const ToneF& tone);

protected:
	ToneImageEffect();
	virtual ~ToneImageEffect();
	void Initialize(GraphicsManager* manager);
	virtual void OnRender(RenderingContext2* renderingContext, RenderTarget* source, RenderTarget* destination) override;

private:
	Vector4		m_tone;

	struct ShaderInfo
	{
		Shader*          shader = nullptr;
		ShaderVariable*  varTone = nullptr;
		ShaderVariable*  varScreenTexture = nullptr;
	} m_shader;

};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
