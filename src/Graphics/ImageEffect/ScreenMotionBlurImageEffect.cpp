
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
	: m_manager(nullptr)
	, m_accumTexture(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScreenMotionBlurImageEffect::~ScreenMotionBlurImageEffect()
{
	LN_SAFE_RELEASE(m_shader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_shader = LN_NEW Shader();
	m_shader->Initialize(m_manager, g_ScreenMotionBlurImageEffect_fx_Data, g_ScreenMotionBlurImageEffect_fx_Len);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::OnRender(RenderingContext2* renderingContext, RenderTarget* source, RenderTarget* destination)
{
	//LN_NOTIMPLEMENTED();

	//const Size& sourceSize = source->GetSize();

	//// m_accumTexture ‚Æ source ‚ÌƒTƒCƒY‚ªˆÙ‚È‚éê‡‚Íì‚è’¼‚·
	//if (m_accumTexture == nullptr || m_accumTexture->GetSize() != sourceSize)
	//{
	//	m_accumTexture = LN_NEW RenderTarget();
	//	m_accumTexture->CreateImpl(m_manager, sourceSize, 1, TextureFormat_R8G8B8A8);
	//	renderingContext->Blt(source, m_accumTexture);
	//}

	renderingContext->Blt(source, destination, m_shader);
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
