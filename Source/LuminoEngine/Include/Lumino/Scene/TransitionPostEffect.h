
#pragma once
#include "../UI/UIViewport.h"

LN_NAMESPACE_BEGIN

/**
	@brief
*/
class TransitionPostEffect
	: public PostEffect
{
	LN_OBJECT();
public:
	static RefPtr<TransitionPostEffect> create();
	void transition(float duration, Texture* mask, int vague);

private:
	virtual void onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
	TransitionPostEffect();
	virtual ~TransitionPostEffect();
	void initialize();

private:
	RefPtr<RenderTargetTexture>	m_primaryTarget;
	RefPtr<RenderTargetTexture>	m_savedTarget;
	float						m_factor;
	bool						m_freezeRequested;

	struct WithoutMaskShader
	{
		RefPtr<Shader>			shader;
		ShaderVariable*			varFactor;
		ShaderVariable*			varFreezedTexture;
	} m_withoutMaskShader;
};

LN_NAMESPACE_END
