
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
void BitmapTextRenderer::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	m_manager = manager;

	m_tmpGlyphRun = LN_NEW GlyphRun();
	m_tmpGlyphRun->Initialize(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void BitmapTextRenderer::DrawGlyphRun(Bitmap* target, GlyphRun* glyphRun)
{
	LN_CHECK_ARGS_RETURN(target != nullptr);
	LN_CHECK_ARGS_RETURN(glyphRun != nullptr);

	Font* font = glyphRun->GetFont();
	auto& items = glyphRun->GetLayoutItems();
	for (auto& item : items)
	{
		FontGlyphBitmap* gb = font->LookupGlyphBitmap(item.Char);
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
