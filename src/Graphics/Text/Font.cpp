
#include "../Internal.h"
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "../GraphicsManager.h"
#include "FontManager.h"
#include "FontGlyphTextureCache.h"
#include "BitmapTextRenderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// RawFont
//==============================================================================

//------------------------------------------------------------------------------
RawFontPtr RawFont::GetDefaultFont()
{
	RawFontPtr font(detail::GraphicsManager::GetInstance()->GetFontManager()->GetDefaultFont(), true);
	return font;
}

//------------------------------------------------------------------------------
RawFont::RawFont()
	: m_glyphTextureCache(nullptr)
{
}

//------------------------------------------------------------------------------
RawFont::~RawFont()
{
	LN_SAFE_RELEASE(m_glyphTextureCache);
}

//------------------------------------------------------------------------------
SizeI RawFont::GetTextSize(const StringRef& text)
{
	auto* r = GetManager()->GetGraphicsManager()->GetBitmapTextRenderer();
	auto* gr = r->GetTempGlyphRun();
	gr->SetFont(this);
	gr->SetText(text);
	return gr->GetRenderSize();
}

//------------------------------------------------------------------------------
detail::FontGlyphTextureCache* RawFont::GetGlyphTextureCache()
{
	if (m_glyphTextureCache == nullptr)
	{
		m_glyphTextureCache = LN_NEW detail::FontGlyphTextureCache();
		m_glyphTextureCache->Initialize(GetManager()->GetGraphicsManager(), this);
	}
	return m_glyphTextureCache;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
