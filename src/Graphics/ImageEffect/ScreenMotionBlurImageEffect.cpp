
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Graphics/ImageEffect/ScreenMotionBlurImageEffect.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// ScreenMotionBlurImageEffect
//=============================================================================

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
	obj->Initialize(detail::GetGraphicsManager(nullptr));
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScreenMotionBlurImageEffect::ScreenMotionBlurImageEffect()
	: m_accumTexture(nullptr)
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

	m_shader.varBlurPower = m_shader.shader->FindVariable(_T("gBlurPower"));
	m_shader.varBlurColor = m_shader.shader->FindVariable(_T("gBlurColor"));
	m_shader.varBlurMatrix = m_shader.shader->FindVariable(_T("gBlurMatrix"));
	m_shader.varSecondaryTexture = m_shader.shader->FindVariable(_T("gSecondaryTexture"));
	m_shader.techMainDraw = m_shader.shader->GetTechniques()[0];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::OnRender(RenderingContext2* renderingContext, RenderTarget* source, RenderTarget* destination)
{
	//LN_NOTIMPLEMENTED();

	const Size& sourceSize = source->GetSize();

	// m_accumTexture ‚Æ source ‚ÌƒTƒCƒY‚ªˆÙ‚È‚éê‡‚Íì‚è’¼‚·
	if (m_accumTexture == nullptr || m_accumTexture->GetSize() != sourceSize)
	{
		m_accumTexture = LN_NEW RenderTarget();
		m_accumTexture->CreateImpl(m_manager, sourceSize, 1, TextureFormat_R8G8B8A8);
		renderingContext->Blt(source, m_accumTexture);
	}

#if 1
	Matrix m;
	m.Scale(1.05);
	m_shader.varBlurPower->SetFloat(0.5);
	m_shader.varBlurColor->SetVector(Vector4(1, 1, 1, 1));
	m_shader.varBlurMatrix->SetMatrix(m);
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
