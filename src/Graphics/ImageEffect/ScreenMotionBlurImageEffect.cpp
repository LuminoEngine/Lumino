
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Graphics/ImageEffect/ScreenMotionBlurImageEffect.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// ScreenMotionBlurImageEffect
//=============================================================================

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
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScreenMotionBlurImageEffect::OnRender(RenderingContext2* renderingContext, RenderTarget* source, RenderTarget* destination)
{
	LN_NOTIMPLEMENTED();

	const Size& sourceSize = source->GetSize();

	// m_accumTexture ‚Æ source ‚ÌƒTƒCƒY‚ªˆÙ‚È‚éê‡‚Íì‚è’¼‚·
	if (m_accumTexture == nullptr || m_accumTexture->GetSize() != sourceSize)
	{
		m_accumTexture = LN_NEW RenderTarget();
		m_accumTexture->CreateImpl(m_manager, sourceSize, 1, TextureFormat_R8G8B8A8);
		renderingContext->Blt(source, m_accumTexture);
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
