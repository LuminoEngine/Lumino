
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Graphics/ImageEffect/ToneImageEffect.h>
#include "../../Animation/AnimationManager.h"
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// ToneImageEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ToneImageEffect, ImageEffect);
LN_TR_PROPERTY2_IMPLEMENT(ToneImageEffect, Vector4, Tone, tr::PropertyMetadata());

static const byte_t g_ToneImageEffect_fx_Data[] =
{
#include "../Resource/ToneImageEffect.fx.h"
};
static const size_t g_ToneImageEffect_fx_Len = LN_ARRAY_SIZE_OF(g_ToneImageEffect_fx_Data);

//------------------------------------------------------------------------------
ToneImageEffectPtr ToneImageEffect::Create()
{
	ToneImageEffectPtr obj(LN_NEW ToneImageEffect(), false);
	obj->Initialize(detail::GraphicsManager::GetInstance());
	return obj;
}

//------------------------------------------------------------------------------
ToneImageEffect::ToneImageEffect()
{
}

//------------------------------------------------------------------------------
ToneImageEffect::~ToneImageEffect()
{
	LN_SAFE_RELEASE(m_shader.shader);
}

//------------------------------------------------------------------------------
void ToneImageEffect::Initialize(detail::GraphicsManager* manager)
{
	ImageEffect::Initialize(manager);

	m_shader.shader = LN_NEW Shader();
	m_shader.shader->Initialize(m_manager, g_ToneImageEffect_fx_Data, g_ToneImageEffect_fx_Len);
	m_shader.varTone = m_shader.shader->FindVariable(_T("Tone"));
	m_shader.varScreenTexture = m_shader.shader->FindVariable(_T("ScreenTexture"));
}

//------------------------------------------------------------------------------
void ToneImageEffect::SetTone(const ToneF& tone)
{
	Tone = tone;
}

//------------------------------------------------------------------------------
void ToneImageEffect::ChangeTone(const ToneF& tone, double time)
{
	auto anim = ValueEasingCurve<Vector4>::Create(tone, time, EasingMode::Linear);
	AnimationClock* ac = m_manager->GetAnimationManager()->StartPropertyAnimation(this);
	ac->AddAnimationCurve(anim.Get(), this, ToneImageEffect::ToneId, Tone.Get());
}

//------------------------------------------------------------------------------
void ToneImageEffect::OnRender(RenderingContext* context, RenderTarget* source, RenderTarget* destination)
{
	if (Tone != Vector4::Zero)
	{
		m_shader.varTone->SetVector(Tone.Get());
		m_shader.varScreenTexture->SetTexture(source);
		context->Blt(nullptr, destination, m_shader.shader);
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
