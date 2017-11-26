
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Material.h>
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Graphics/ImageEffect/ScreenMotionBlurImageEffect.h>
#include "../../Animation/AnimationManager.h"
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// ScreenMotionBlurImageEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ScreenMotionBlurImageEffect, ImageEffect);

static const byte_t g_ScreenMotionBlurImageEffect_fx_Data[] =
{
#include "../Resource/ScreenMotionBlurImageEffect.fx.h"
};
static const size_t g_ScreenMotionBlurImageEffect_fx_Len = LN_ARRAY_SIZE_OF(g_ScreenMotionBlurImageEffect_fx_Data);

//------------------------------------------------------------------------------
ScreenMotionBlurImageEffectPtr ScreenMotionBlurImageEffect::create()
{
	ScreenMotionBlurImageEffectPtr obj(LN_NEW ScreenMotionBlurImageEffect(), false);
	obj->initialize(detail::GraphicsManager::getInstance());
	return obj;
}

//------------------------------------------------------------------------------
ScreenMotionBlurImageEffect::ScreenMotionBlurImageEffect()
	: m_accumTexture(nullptr)
	, m_amount(0)
	, m_center(0, 0)
	, m_scale(1.0)
{
	initializeProperties();
}

//------------------------------------------------------------------------------
ScreenMotionBlurImageEffect::~ScreenMotionBlurImageEffect()
{
	LN_SAFE_RELEASE(m_accumTexture);
}

//------------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::initialize(detail::GraphicsManager* manager)
{
	ImageEffect::initialize(manager);

	auto shader = Object::makeRef<Shader>(m_manager, g_ScreenMotionBlurImageEffect_fx_Data, g_ScreenMotionBlurImageEffect_fx_Len);

	m_material = Object::makeRef<CommonMaterial>();
	m_material->setShader(shader);
	m_material->setBlendMode(BlendMode::Alpha);
}

//------------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::setBlurStatus(float amount, const Vector2& center, float scale, float duration)
{
	setRadialCenter(center);
	setRadialScale(scale);

	if (duration == 0.0f)
	{
		setAmount(amount);			// アニメーション再生中でも、ここでプロパティを再設定するときに停止される
	}
	else
	{
		LN_NOTIMPLEMENTED();
		//auto anim = ValueEasingCurve<float>::create(0, duration, EasingMode::Linear);
		//AnimationClock* ac = m_manager->getAnimationManager()->StartPropertyAnimation(this);
		//ac->addAnimationCurve(anim.Get(), this, AmountId, amount);
	}
}

//------------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
	if (m_amount == 0.0f)
	{
		context->blit(source, destination, Matrix::Identity);
		return;
	}

	const SizeI& sourceSize = source->getSize();

	// m_accumTexture と source のサイズが異なる場合は作り直す
	if (m_accumTexture == nullptr || m_accumTexture->getSize() != sourceSize)
	{
		m_accumTexture = LN_NEW RenderTargetTexture();
		m_accumTexture->createImpl(m_manager, sourceSize, 1, TextureFormat::R8G8B8X8);
		context->blit(source, m_accumTexture, Matrix::Identity);
	}

	Matrix blurMatrix;
	blurMatrix.translate(-m_center.x, -m_center.y, 0);
	blurMatrix.scale(m_scale);
	blurMatrix.translate(m_center.x, m_center.y, 0);

	m_material->setVectorParameter(_LT("_BlurColor"), Vector4(1, 1, 1, m_amount));
	m_material->setMatrixParameter(_LT("_BlurMatrix"), blurMatrix);

	//// m_accumTexture > source
	//context->blit(m_accumTexture, destination, m_material);

	//// save
	//context->blit(destination, m_accumTexture, Matrix::Identity);

	// m_accumTexture > source
	context->blit(m_accumTexture, source, m_material);

	// save
	context->blit(source, m_accumTexture, Matrix::Identity);

	context->blit(m_accumTexture, destination, Matrix::Identity);

}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
