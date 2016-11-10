
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
	ptr->Initialize(detail::GraphicsManager::GetInstance(), nullptr);
	return ptr;
}

//------------------------------------------------------------------------------
FontPtr Font::Create(const String& family)
{
	auto ptr = FontPtr::MakeRef();
	ptr->SetFamily(family);
	return ptr;
}

//------------------------------------------------------------------------------
FontPtr Font::Create(const String& family, float size)
{
	auto ptr = FontPtr::MakeRef();
	ptr->Initialize(detail::GraphicsManager::GetInstance(), nullptr);
	ptr->SetFamily(family);
	ptr->SetSize(size);
	return ptr;
}

//------------------------------------------------------------------------------
FontPtr Font::GetDefault()
{
	return detail::GraphicsManager::GetInstance()->GetFontManager()->GetDefaultFont();
}

//------------------------------------------------------------------------------
FontPtr Font::GetBuiltin(BuiltinFontSize size)
{
	return detail::GraphicsManager::GetInstance()->GetFontManager()->GetBuiltinFont(size);
}

//------------------------------------------------------------------------------
Font::Font()
	: m_manager(nullptr)
	, m_fontInfo()
	, m_rawFont(nullptr)
	, m_builtin(false)
{
}

//------------------------------------------------------------------------------
void Font::Initialize(detail::GraphicsManager* manager, RawFont* builtinRawFont)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;

	if (builtinRawFont != nullptr)
	{
		m_rawFont = builtinRawFont;
		m_builtin = true;
	}
}

//------------------------------------------------------------------------------
Font::~Font()
{
}

//------------------------------------------------------------------------------
void Font::SetFamily(const String& familyName)
{
	LN_CHECK_STATE(!m_builtin);
	m_fontInfo.Family = familyName;
	m_rawFont = nullptr;
}

//------------------------------------------------------------------------------
const String& Font::GetFamily() const
{
	return m_fontInfo.Family;
}

//------------------------------------------------------------------------------
void Font::SetSize(int size)
{
	LN_CHECK_STATE(!m_builtin);
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
	LN_CHECK_STATE(!m_builtin);
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
	LN_CHECK_STATE(!m_builtin);
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
	LN_CHECK_STATE(!m_builtin);
	m_fontInfo.IsAntiAlias = enabled;
	m_rawFont = nullptr;
}

//------------------------------------------------------------------------------
bool Font::IsAntiAlias() const
{
	return m_fontInfo.IsAntiAlias;
}

//------------------------------------------------------------------------------
Size Font::MeasureRenderSize(const StringRef& text)
{
	RawFont* raw = ResolveRawFont();
	SizeI size = raw->GetTextSize(text);
	return Size(size.width, size.height);
}

//------------------------------------------------------------------------------
RawFont* Font::ResolveRawFont()
{
	if (m_rawFont == nullptr)
	{
		m_rawFont = m_manager->GetFontManager()->LookupRawFont(m_fontInfo);
	}
	return m_rawFont;
}

//==============================================================================
// RawFont
//==============================================================================

//------------------------------------------------------------------------------
RawFontPtr RawFont::GetDefaultFont()
{
	RawFontPtr font(detail::GraphicsManager::GetInstance()->GetFontManager()->GetDefaultRawFont(), true);
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
