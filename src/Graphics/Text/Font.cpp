
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
// Font
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Font, Object);

//------------------------------------------------------------------------------
FontPtr Font::Create()
{
	auto ptr = FontPtr::MakeRef();
	return ptr;
}

//------------------------------------------------------------------------------
Font::Font()
	: m_manager(nullptr)
	, m_fontInfo()
	, m_rawFont(nullptr)
{
}

//------------------------------------------------------------------------------
void Font::Initialize(detail::GraphicsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
}

//------------------------------------------------------------------------------
Font::~Font()
{

}

//------------------------------------------------------------------------------
void Font::SetName(const String& familyName)
{
	m_fontInfo.Family = familyName;
	m_rawFont = nullptr;
}

//------------------------------------------------------------------------------
const String& Font::GetName() const
{
	return m_fontInfo.Family;
}

//------------------------------------------------------------------------------
void Font::SetSize(int size)
{
	m_fontInfo.Size = size;
	m_rawFont = nullptr;
}

//------------------------------------------------------------------------------
int Font::GetSize() const
{
	return m_fontInfo.Size;
}

//------------------------------------------------------------------------------
void Font::SetBold(bool enabled)
{
	m_fontInfo.IsBold = enabled;
	m_rawFont = nullptr;
}

//------------------------------------------------------------------------------
bool Font::IsBold() const
{
	return m_fontInfo.IsBold;
}

//------------------------------------------------------------------------------
void Font::SetItalic(bool enabled)
{
	m_fontInfo.IsItalic = enabled;
	m_rawFont = nullptr;
}

//------------------------------------------------------------------------------
bool Font::IsItalic() const
{
	return m_fontInfo.IsItalic;
}

//------------------------------------------------------------------------------
void Font::SetAntiAlias(bool enabled)
{
	m_fontInfo.IsAntiAlias = enabled;
	m_rawFont = nullptr;
}

//------------------------------------------------------------------------------
bool Font::IsAntiAlias() const
{
	return m_fontInfo.IsAntiAlias;
}


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
