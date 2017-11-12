
#pragma once
//#include "../Animation/EasingValue.h"
//#include "../Graphics/Color.h"
//#include "../Graphics/ImageEffect/ImageEffect.h"
#include "RenderView.h"

LN_NAMESPACE_BEGIN

/**
	@brief
*/
class SSAOImageEffect
	: public PostEffect
{
	LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
	SSAOImageEffect();
	virtual ~SSAOImageEffect();
	void initialize();
	virtual void onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

private:
	Ref<CommonMaterial>	m_material;
	Ref<CommonMaterial>	m_material2;
	Ref<RenderTargetTexture>	m_ssaoRenderTarget;
};

LN_NAMESPACE_END
