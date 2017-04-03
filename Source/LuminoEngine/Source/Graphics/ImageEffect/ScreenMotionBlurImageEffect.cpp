
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Material.h>
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Graphics/ImageEffect/ScreenMotionBlurImageEffect.h>
#include "../../Animation/AnimationManager.h"
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// ScreenMotionBlurImageEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ScreenMotionBlurImageEffect, ImageEffect);
LN_TR_PROPERTY_IMPLEMENT(ScreenMotionBlurImageEffect, float, Amount, tr::PropertyMetadata());

static const byte_t g_ScreenMotionBlurImageEffect_fx_Data[] =
{
#include "../Resource/ScreenMotionBlurImageEffect.fx.h"
};
static const size_t g_ScreenMotionBlurImageEffect_fx_Len = LN_ARRAY_SIZE_OF(g_ScreenMotionBlurImageEffect_fx_Data);

//------------------------------------------------------------------------------
ScreenMotionBlurImageEffectPtr ScreenMotionBlurImageEffect::Create()
{
	ScreenMotionBlurImageEffectPtr obj(LN_NEW ScreenMotionBlurImageEffect(), false);
	obj->Initialize(detail::GraphicsManager::GetInstance());
	return obj;
}

//------------------------------------------------------------------------------
ScreenMotionBlurImageEffect::ScreenMotionBlurImageEffect()
	: m_accumTexture(nullptr)
	, Amount(0)
	, m_center(0, 0)
	, m_scale(1.0)
{
	InitializeProperties();
}

//------------------------------------------------------------------------------
ScreenMotionBlurImageEffect::~ScreenMotionBlurImageEffect()
{
	LN_SAFE_RELEASE(m_accumTexture);
}

//------------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::Initialize(detail::GraphicsManager* manager)
{
	ImageEffect::Initialize(manager);

	auto shader = Object::MakeRef<Shader>(m_manager, g_ScreenMotionBlurImageEffect_fx_Data, g_ScreenMotionBlurImageEffect_fx_Len);

	m_material = Object::MakeRef<Material>();
	m_material->SetShader(shader);
	m_material->SetBlendMode(BlendMode::Alpha);
}

//------------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::SetBlurStatus(float amount, const Vector2& center, float scale, float duration)
{
	SetRadialCenter(center);
	SetRadialScale(scale);

	if (duration == 0.0f)
	{
		SetAmount(amount);			// アニメーション再生中でも、ここでプロパティを再設定するときに停止される
	}
	else
	{
		auto anim = ValueEasingCurve<float>::Create(0, duration, EasingMode::Linear);
		AnimationClock* ac = m_manager->GetAnimationManager()->StartPropertyAnimation(this);
		ac->AddAnimationCurve(anim.Get(), this, AmountId, amount);
	}
}

//------------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::OnRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
	if (Amount == 0.0f)
	{
		context->Blit(source, destination, Matrix::Identity);
		return;
	}

	const SizeI& sourceSize = source->GetSize();

	// m_accumTexture と source のサイズが異なる場合は作り直す
	if (m_accumTexture == nullptr || m_accumTexture->GetSize() != sourceSize)
	{
		m_accumTexture = LN_NEW RenderTargetTexture();
		m_accumTexture->CreateImpl(m_manager, sourceSize, 1, TextureFormat::R8G8B8X8);
		context->Blit(source, m_accumTexture, Matrix::Identity);
	}

	Matrix blurMatrix;
	blurMatrix.Translate(-m_center.x, -m_center.y, 0);
	blurMatrix.Scale(m_scale);
	blurMatrix.Translate(m_center.x, m_center.y, 0);

	m_material->SetVectorParameter(_T("_BlurColor"), Vector4(1, 1, 1, Amount.Get()));
	m_material->SetMatrixParameter(_T("_BlurMatrix"), blurMatrix);

	//// m_accumTexture > source
	//context->Blit(m_accumTexture, destination, m_material);

	//// save
	//context->Blit(destination, m_accumTexture, Matrix::Identity);

	// m_accumTexture > source
	context->Blit(m_accumTexture, source, m_material);

	// save
	context->Blit(source, m_accumTexture, Matrix::Identity);

	context->Blit(m_accumTexture, destination, Matrix::Identity);

}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
