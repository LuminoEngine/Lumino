
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Graphics/ImageEffect/ScreenMotionBlurImageEffect.h>
#include "../../Animation/AnimationManager.h"
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// ScreenMotionBlurImageEffect
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ScreenMotionBlurImageEffect, ImageEffect);
LN_TR_PROPERTY_IMPLEMENT(ScreenMotionBlurImageEffect, float, AmountProperty, "Amount", m_amount, tr::PropertyMetadata());

static const byte_t g_ScreenMotionBlurImageEffect_fx_Data[] =
{
#include "../Resource/ScreenMotionBlurImageEffect.fx.h"
};
static const size_t g_ScreenMotionBlurImageEffect_fx_Len = LN_ARRAY_SIZE_OF(g_ScreenMotionBlurImageEffect_fx_Data);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScreenMotionBlurImageEffectPtr ScreenMotionBlurImageEffect::Create()
{
	ScreenMotionBlurImageEffectPtr obj(LN_NEW ScreenMotionBlurImageEffect(), false);
	obj->Initialize(GraphicsManager::GetInstance());
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScreenMotionBlurImageEffect::ScreenMotionBlurImageEffect()
	: m_accumTexture(nullptr)
	, m_amount(0)
	, m_center(0, 0)
	, m_scale(1.0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScreenMotionBlurImageEffect::~ScreenMotionBlurImageEffect()
{
	LN_SAFE_RELEASE(m_accumTexture);
	LN_SAFE_RELEASE(m_shader.shader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::Initialize(GraphicsManager* manager)
{
	ImageEffect::Initialize(manager);

	m_shader.shader = LN_NEW Shader();
	m_shader.shader->Initialize(m_manager, g_ScreenMotionBlurImageEffect_fx_Data, g_ScreenMotionBlurImageEffect_fx_Len);

	m_shader.varBlurPower = m_shader.shader->FindVariable(_T("g_blurPower"));
	m_shader.varBlurColor = m_shader.shader->FindVariable(_T("g_blurColor"));
	m_shader.varBlurMatrix = m_shader.shader->FindVariable(_T("g_blurMatrix"));
	m_shader.varSecondaryTexture = m_shader.shader->FindVariable(_T("g_secondaryTexture"));
	m_shader.techMainDraw = m_shader.shader->GetTechniques()[0];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::SetBlurStatus(float amount, const Vector2& center, float scale, float duration)
{
	if (duration == 0.0f)
	{
		AnimationClock::DeactivatePropertyAnimation(this, AmountProperty);
		SetAmount(amount);
		SetRadialCenter(center);
		SetRadialScale(scale);
	}
	else
	{
		auto anim = ValueEasingCurve<float>::Create(0, duration, EasingMode::Linear);
		AnimationClock* ac = m_manager->GetAnimationManager()->StartPropertyAnimation(this);
		ac->AddAnimationCurve(anim.GetObjectPtr(), this, AmountProperty, amount);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::OnRender(RenderingContext2* renderingContext, RenderTarget* source, RenderTarget* destination)
{
	if (m_amount == 0.0)
	{
		renderingContext->Blt(source, destination);
		return;
	}

	const Size& sourceSize = source->GetSize();

	// m_accumTexture と source のサイズが異なる場合は作り直す
	if (m_accumTexture == nullptr || m_accumTexture->GetSize() != sourceSize)
	{
		m_accumTexture = LN_NEW RenderTarget();
		m_accumTexture->CreateImpl(m_manager, sourceSize, 1, TextureFormat_R8G8B8X8);
		renderingContext->Blt(source, m_accumTexture);
	}

#if 1
	Matrix blurMatrix;
	blurMatrix.Translate(-m_center.x, -m_center.y, 0);
	blurMatrix.Scale(m_scale);
	blurMatrix.Translate(m_center.x, m_center.y, 0);


	m_shader.varBlurPower->SetFloat(m_amount);
	m_shader.varBlurColor->SetVector(Vector4(1, 1, 1, 1));
	m_shader.varBlurMatrix->SetMatrix(blurMatrix);
	m_shader.varSecondaryTexture->SetTexture(m_accumTexture);
	renderingContext->Blt(nullptr, source, m_shader.shader);

	renderingContext->Blt(source, m_accumTexture);

	renderingContext->Blt(m_accumTexture, destination);
#endif
#if 0
	Matrix m;
	m.Scale(1.05);
	m_shader.varBlurPower->SetFloat(0.5);
	m_shader.varBlurColor->SetVector(Vector4(1, 1, 1, 1));
	m_shader.varBlurMatrix->SetMatrix(m);
	m_shader.varSecondaryTexture->SetTexture(m_accumTexture);
	renderingContext->Blt(source, source, m_shader.shader);
	renderingContext->Blt(source, m_accumTexture);
	renderingContext->Blt(source, destination);
#endif
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
