
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
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	LN_TR_PROPERTY(Vector4, Tone);

public:
	static ToneImageEffectPtr Create();

	void SetTone(const ToneF& tone);

	void ChangeTone(const ToneF& tone, double time);

protected:
	ToneImageEffect();
	virtual ~ToneImageEffect();
	void Initialize(detail::GraphicsManager* manager);
	virtual void OnRender(RenderingContext* context, RenderTarget* source, RenderTarget* destination) override;

private:
	tr::Property<Vector4> m_tone;

	struct ShaderInfo
	{
		Shader*          shader = nullptr;
		ShaderVariable*  varTone = nullptr;
		ShaderVariable*  varScreenTexture = nullptr;
	} m_shader;

};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
