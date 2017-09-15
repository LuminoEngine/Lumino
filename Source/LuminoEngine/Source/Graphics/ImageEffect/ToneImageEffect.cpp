
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Material.h>
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Graphics/ImageEffect/ToneImageEffect.h>
#include "../../Animation/AnimationManager.h"
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// ToneImageEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ToneImageEffect, ImageEffect);

static const byte_t g_ToneImageEffect_fx_Data[] =
{
#include "../Resource/ToneImageEffect.fx.h"
};
static const size_t g_ToneImageEffect_fx_Len = LN_ARRAY_SIZE_OF(g_ToneImageEffect_fx_Data);

//------------------------------------------------------------------------------
ToneImageEffectPtr ToneImageEffect::create()
{
	ToneImageEffectPtr obj(LN_NEW ToneImageEffect(), false);
	obj->initialize(detail::GraphicsManager::getInstance());
	return obj;
}

//------------------------------------------------------------------------------
ToneImageEffect::ToneImageEffect()
{
	initializeProperties();
}

//------------------------------------------------------------------------------
ToneImageEffect::~ToneImageEffect()
{
}

//------------------------------------------------------------------------------
void ToneImageEffect::initialize(detail::GraphicsManager* manager)
{
	ImageEffect::initialize(manager);

	m_material = Object::makeRef<Material>();
	m_material->setShader(Object::makeRef<Shader>(m_manager, g_ToneImageEffect_fx_Data, g_ToneImageEffect_fx_Len));

	//m_shader.shader = LN_NEW Shader();
	//m_shader.shader->initialize(m_manager, g_ToneImageEffect_fx_Data, g_ToneImageEffect_fx_Len);
	//m_shader.varTone = m_shader.shader->findVariable(_LT("Tone"));
	//m_shader.varScreenTexture = m_shader.shader->findVariable(_LT("ScreenTexture"));


	//printf("%p %p\n", m_material.Get(), m_material->getShader());
}

//------------------------------------------------------------------------------
void ToneImageEffect::setTone(const ToneF& tone)
{
	m_tone = tone;
}

//------------------------------------------------------------------------------
void ToneImageEffect::changeTone(const ToneF& tone, double time)
{
	LN_NOTIMPLEMENTED();
	//auto anim = ValueEasingCurve<Vector4>::create(tone, time, EasingMode::Linear);
	//AnimationClock* ac = m_manager->getAnimationManager()->StartPropertyAnimation(this);
	//ac->addAnimationCurve(anim.Get(), this, ToneImageEffect::ToneId, Tone.Get());
}

//------------------------------------------------------------------------------
void ToneImageEffect::onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
	//if (Tone != Vector4::Zero)
	{
		//printf("ToneImageEffect::onRender %p > %p\n", source, destination);
		m_material->setVectorParameter(_LT("_Tone"), m_tone);
		context->blit(source, destination, m_material);

		//m_shader.varTone->setVector(Tone.Get());
		//m_shader.varScreenTexture->setTexture(source);
		//context->blit(nullptr, destination, m_shader.shader);
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
