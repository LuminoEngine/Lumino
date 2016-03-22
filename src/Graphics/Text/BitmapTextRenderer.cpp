
#include "../Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "BitmapTextRenderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// BitmapTextRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BitmapTextRenderer::BitmapTextRenderer()
	: m_manager(nullptr)
	, m_tmpGlyphRun(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BitmapTextRenderer::~BitmapTextRenderer()
{
	LN_SAFE_RELEASE(m_tmpGlyphRun);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void BitmapTextRenderer::Initialize(FontManager* manager)
{
	LN_CHECK_ARGS(manager != nullptr);
	m_manager = manager;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
