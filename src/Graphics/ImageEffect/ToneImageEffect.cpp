
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Graphics/ImageEffect/ToneImageEffect.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// ToneImageEffect
//=============================================================================

static const byte_t g_ToneImageEffect_fx_Data[] =
{
#include "../Resource/ToneImageEffect.fx.h"
};
static const size_t g_ToneImageEffect_fx_Len = LN_ARRAY_SIZE_OF(g_ToneImageEffect_fx_Data);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ToneImageEffectPtr ToneImageEffect::Create()
{
	ToneImageEffectPtr obj(LN_NEW ToneImageEffect(), false);
	obj->Initialize(detail::GetGraphicsManager(nullptr));
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ToneImageEffect::ToneImageEffect()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ToneImageEffect::~ToneImageEffect()
{
	LN_SAFE_RELEASE(m_shader.shader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ToneImageEffect::Initialize(GraphicsManager* manager)
{
	ImageEffect::Initialize(manager);

	m_shader.shader = LN_NEW Shader();
	m_shader.shader->Initialize(m_manager, g_ToneImageEffect_fx_Data, g_ToneImageEffect_fx_Len);
	m_shader.varTone = m_shader.shader->FindVariable(_T("Tone"));
	m_shader.varScreenTexture = m_shader.shader->FindVariable(_T("ScreenTexture"));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ToneImageEffect::ChangeTone(const ToneF& tone, double duration)
{
	m_tone.Start(tone, duration);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ToneImageEffect::OnRender(RenderingContext2* renderingContext, RenderTarget* source, RenderTarget* destination)
{
	m_shader.varTone->SetVector(m_tone.GetValue());
	m_shader.varScreenTexture->SetTexture(source);
	renderingContext->Blt(nullptr, destination, m_shader.shader);
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
