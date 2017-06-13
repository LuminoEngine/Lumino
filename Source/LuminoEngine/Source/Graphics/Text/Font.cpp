
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
FontPtr Font::create()
{
	auto ptr = FontPtr::makeRef();
	ptr->initialize(detail::GraphicsManager::getInstance(), nullptr);
	return ptr;
}

//------------------------------------------------------------------------------
FontPtr Font::create(const String& family)
{
	auto ptr = FontPtr::makeRef();
	ptr->initialize(detail::GraphicsManager::getInstance(), nullptr);
	ptr->SetFamily(family);
	return ptr;
}

//------------------------------------------------------------------------------
FontPtr Font::create(const String& family, float size)
{
	auto ptr = FontPtr::makeRef();
	ptr->initialize(detail::GraphicsManager::getInstance(), nullptr);
	ptr->SetFamily(family);
	ptr->setSize(size);
	return ptr;
}

//------------------------------------------------------------------------------
FontPtr Font::getDefault()
{
	return detail::GraphicsManager::getInstance()->getFontManager()->GetDefaultFont();
}

//------------------------------------------------------------------------------
FontPtr Font::GetBuiltin(BuiltinFontSize size)
{
	return detail::GraphicsManager::getInstance()->getFontManager()->GetBuiltinFont(size);
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
void Font::initialize(detail::GraphicsManager* manager, RawFont* builtinRawFont)
{
	if (LN_CHECK_ARG(manager != nullptr)) return;
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
	if (LN_CHECK_STATE(!m_builtin)) return;
	m_fontInfo.Family = familyName;
	m_rawFont = nullptr;
}

//------------------------------------------------------------------------------
const String& Font::GetFamily() const
{
	return m_fontInfo.Family;
}

//------------------------------------------------------------------------------
void Font::setSize(int size)
{
	if (LN_CHECK_STATE(!m_builtin)) return;
	m_fontInfo.Size = size;
	m_rawFont = nullptr;
}

//------------------------------------------------------------------------------
int Font::getSize() const
{
	return m_fontInfo.Size;
}

//------------------------------------------------------------------------------
void Font::SetBold(bool enabled)
{
	if (LN_CHECK_STATE(!m_builtin)) return;
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
	if (LN_CHECK_STATE(!m_builtin)) return;
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
	if (LN_CHECK_STATE(!m_builtin)) return;
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
FontPtr Font::Clone() const
{
	auto ptr = FontPtr::makeRef();
	ptr->m_manager = m_manager;
	ptr->m_fontInfo = m_fontInfo;
	ptr->m_rawFont = m_rawFont;
	ptr->m_builtin = m_builtin;
	return ptr;
}

//------------------------------------------------------------------------------
RawFont* Font::ResolveRawFont()
{
	if (m_rawFont == nullptr)
	{
		m_rawFont = m_manager->getFontManager()->LookupRawFont(m_fontInfo);
	}
	return m_rawFont;
}

//==============================================================================
// RawFont
//==============================================================================

//------------------------------------------------------------------------------
RawFontPtr RawFont::GetDefaultFont()
{
	RawFontPtr font(detail::GraphicsManager::getInstance()->getFontManager()->GetDefaultRawFont(), true);
	return font;
}

//------------------------------------------------------------------------------
RawFont::RawFont()
	: m_glyphTextureCache(nullptr)
	, m_vectorGlyphCache(nullptr)
{
}

//------------------------------------------------------------------------------
RawFont::~RawFont()
{
	LN_SAFE_RELEASE(m_glyphTextureCache);
	LN_SAFE_RELEASE(m_vectorGlyphCache);
}

//------------------------------------------------------------------------------
void RawFont::initialize()
{
	m_manager = detail::EngineDomain::getGraphicsManager()->getFontManager();
	m_manager->AddFontResource_(this);
}

//------------------------------------------------------------------------------
void RawFont::finalize_()
{
	Dispose_();
}

//------------------------------------------------------------------------------
void RawFont::Dispose_()
{
	if (m_manager != nullptr)
	{
		m_manager->RemoveFontResource_(this);
		m_manager = nullptr;
	}
}

//------------------------------------------------------------------------------
SizeI RawFont::GetTextSize(const StringRef& text)
{
	auto* r = getManager()->getGraphicsManager()->getBitmapTextRenderer();
	auto* gr = r->GetTempGlyphRun();
	gr->SetFont(this);
	gr->setText(text);
	return gr->getRenderSize();
}

//------------------------------------------------------------------------------
detail::FontGlyphTextureCache* RawFont::GetGlyphTextureCache()
{
	if (m_glyphTextureCache == nullptr)
	{
		m_glyphTextureCache = LN_NEW detail::FontGlyphTextureCache();
		m_glyphTextureCache->initialize(getManager()->getGraphicsManager(), this);
	}
	return m_glyphTextureCache;
}

//------------------------------------------------------------------------------
detail::VectorFontGlyphCache* RawFont::GetVectorGlyphCache()
{
	if (m_vectorGlyphCache == nullptr)
	{
		m_vectorGlyphCache = LN_NEW detail::VectorFontGlyphCache();
		m_vectorGlyphCache->initialize(getManager()->getGraphicsManager(), this, 2048);	// TODO
	}
	return m_vectorGlyphCache;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
