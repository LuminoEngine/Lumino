
#pragma once
#include "../Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "FontGlyphTextureCache.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

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
void GlyphRun::Update(const UTF32* text, int length)
{
	m_glyphData->Items.Clear();
	m_glyphTextureCache->Measure(text, length, m_glyphData);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size GlyphRun::GetRenderSize() const
{
	return m_glyphData->AreaSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GlyphRun::AttachGlyphTextureCache(Internal::FontGlyphTextureCache* cache)
{
	LN_REFOBJ_SET(m_glyphTextureCache, cache);
}


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
