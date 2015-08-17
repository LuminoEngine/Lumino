
#pragma once
#include "../Internal.h"
#include <Lumino/Graphics/GlyphRun.h>
#include "FontGlyphTextureCache.h"

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// GraphicsManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GlyphRun::GlyphRun()
	: m_glyphTextureCache(NULL)
	, m_glyphData(NULL)
{
	m_glyphData = LN_NEW TextLayoutResult();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GlyphRun::~GlyphRun()
{
	LN_SAFE_RELEASE(m_glyphTextureCache);
	LN_SAFE_DELETE(m_glyphData);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GlyphRun::AttachGlyphTextureCache(Internal::FontGlyphTextureCache* cache)
{
	LN_REFOBJ_SET(m_glyphTextureCache, cache);
}


} // namespace Graphics
} // namespace Lumino
