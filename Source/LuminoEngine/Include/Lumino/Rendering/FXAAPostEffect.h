
#pragma once
#include "RenderView.h"

LN_NAMESPACE_BEGIN

/**
	@brief
*/
class FXAAPostEffect
	: public PostEffect
{
	LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
	FXAAPostEffect();
	virtual ~FXAAPostEffect();
	void initialize();
	virtual void onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

private:
	Ref<CommonMaterial>			m_material;
};

LN_NAMESPACE_END
