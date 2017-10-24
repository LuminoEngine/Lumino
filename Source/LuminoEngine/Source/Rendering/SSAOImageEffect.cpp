
#include "../Internal.h"
//#include <Lumino/Graphics/Texture.h>
//#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Material.h>
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Rendering/SSAOImageEffect.h>
#include "../Animation/AnimationManager.h"
#include "../Graphics/GraphicsManager.h"

LN_NAMESPACE_BEGIN

namespace detail
{

	extern RenderTargetTexture* g_m_normalRenderTarget;
}

//==============================================================================
// SSAOImageEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SSAOImageEffect, PostEffect);

SSAOImageEffect::SSAOImageEffect()
{
	initializeProperties();
}

SSAOImageEffect::~SSAOImageEffect()
{
}

void SSAOImageEffect::initialize()
{
	PostEffect::initialize();

	m_material = Object::makeRef<Material>();
	m_material->setShader(Object::makeRef<Shader>(detail::GraphicsManager::getInstance(), _T("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Rendering/Resource/SSAOImageEffect.fx")));
}

void SSAOImageEffect::onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
	//if (Tone != Vector4::Zero)
	{
		m_material->setTextureParameter(_T("normalDepth"), detail::g_m_normalRenderTarget);
		//printf("ToneImageEffect::onRender %p > %p\n", source, destination);
		//m_material->setVectorParameter(_LT("_Tone"), m_tone);
		context->blit(source, destination, m_material);

		//m_shader.varTone->setVector(Tone.Get());
		//m_shader.varScreenTexture->setTexture(source);
		//context->blit(nullptr, destination, m_shader.shader);
	}
}

LN_NAMESPACE_END
