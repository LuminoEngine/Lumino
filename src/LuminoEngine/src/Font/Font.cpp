
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include "FontManager.hpp"
#include "TextLayoutEngine.hpp"

namespace ln {

//==============================================================================
// Font

Ref<Font> Font::create()
{
    return newObject<Font>();
}

Ref<Font> Font::create(const String& family, float size)
{
    return newObject<Font>(family, size);
}

void Font::setDefaultFont(Font* font)
{
    if (LN_REQUIRE(font)) return;
    detail::EngineDomain::fontManager()->setDefaultFontDesc(font->m_desc);
}

Font::Font()
    : m_manager(detail::EngineDomain::fontManager())
    , m_desc()
    , m_rawFont(nullptr)
    , m_builtin(false)
{
}

Font::~Font()
{
}

void Font::initialize()
{
    m_desc = m_manager->defaultFontDesc();
}

void Font::initialize(const String& family, float size)
{
    initialize();
    setFamily(family);
    setSize(size);
}

void Font::setFamily(const String& familyName)
{
    if (LN_REQUIRE(!m_builtin)) return;
    m_desc.Family = familyName;
    m_rawFont = nullptr;
}

const String& Font::family() const
{
    return m_desc.Family;
}

void Font::setSize(int size)
{
    if (LN_REQUIRE(!m_builtin)) return;
    m_desc.Size = size;
    m_rawFont = nullptr;
}

int Font::size() const
{
    return m_desc.Size;
}

void Font::setBold(bool enabled)
{
    if (LN_REQUIRE(!m_builtin)) return;
    m_desc.isBold = enabled;
    m_rawFont = nullptr;
}

bool Font::isBold() const
{
    return m_desc.isBold;
}

void Font::setItalic(bool enabled)
{
    if (LN_REQUIRE(!m_builtin)) return;
    m_desc.isItalic = enabled;
    m_rawFont = nullptr;
}

bool Font::isItalic() const
{
    return m_desc.isItalic;
}

void Font::setAntiAlias(bool enabled)
{
    if (LN_REQUIRE(!m_builtin)) return;
    m_desc.isAntiAlias = enabled;
    m_rawFont = nullptr;
}

bool Font::isAntiAlias() const
{
    return m_desc.isAntiAlias;
}

Ref<Font> Font::clone() const
{
    auto ptr = newObject<Font>();
    ptr->m_manager = m_manager;
    ptr->m_desc = m_desc;
    ptr->m_rawFont = m_rawFont;
    ptr->m_builtin = m_builtin;
    return ptr;
}

Size Font::measureRenderSize(const StringRef& text)
{
    detail::FontCore* font = resolveFontCore();
    detail::MeasureTextLayoutEngine measureLayout;
    measureLayout.layout(font, text.data(), text.length(), Size(), detail::TextLayoutOptions::None);
    return measureLayout.areaSize;
}

detail::FontCore* Font::resolveFontCore()
{
    if (!m_rawFont) {
        m_rawFont = m_manager->lookupFontCore(m_desc);
    }
    return m_rawFont;
}

} // namespace ln

