
#include "Internal.hpp"
#include <LuminoFont/Font.hpp>
#include "FontManager.hpp"
#include "TextLayoutEngine.hpp"

namespace ln {

//==============================================================================
// Font

const int Font::DefaultSize = 12.0f;

Ref<Font> Font::create()
{
    return makeObject_deprecated<Font>();
}

Ref<Font> Font::create(const String& family, float size)
{
    return makeObject_deprecated<Font>(family, size);
}

void Font::setDefaultFont(Font* font)
{
    if (LN_REQUIRE(font)) return;
    detail::FontManager::instance()->setDefaultFont(font);
}

void Font::registerFontFromFile(const StringView& fontFilePath, bool defaultFamily)
{
	detail::FontManager::instance()->registerFontFromFile(fontFilePath, defaultFamily);
}

Font::Font()
    : m_manager(detail::FontManager::instance())
    , m_desc()
    , m_rawFont(nullptr)
    , m_builtin(false)
{
}

Font::~Font()
{
}

void Font::init()
{
	Object::init();
    m_desc = m_manager->defaultFontDesc();
}

void Font::init(const String& family, float size)
{
    init();
    setFamily(family);
    setSize(size);
}

void Font::init(const detail::FontDesc& desc)
{
	Object::init();
	m_desc = desc;
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
    auto ptr = makeObject_deprecated<Font>();
    ptr->m_manager = m_manager;
    ptr->m_desc = m_desc;
    ptr->m_rawFont = m_rawFont;
    ptr->m_builtin = m_builtin;
    return ptr;
}

Size Font::measureRenderSize(const StringView& text, float dpiScale)
{
    detail::FontCore* font = resolveFontCore(dpiScale);
    detail::MeasureTextLayoutEngine measureLayout;
    measureLayout.layout(font, text.data(), text.length(), Rect(), 0, TextAlignment::Forward);
    return measureLayout.areaSize;
}

Size Font::measureRenderSize(uint32_t codePoint, float dpiScale)
{
	auto core = resolveFontCore(dpiScale);
	detail::FontGlyphMetrics metrics;
	core->getGlyphMetrics(codePoint, &metrics);
	return metrics.size;
}

detail::FontCore* Font::resolveFontCore(float dpiScale)
{
    if (!m_rawFont) {
        m_rawFont = m_manager->lookupFontCore(m_desc, dpiScale);
    }
    return m_rawFont;
}

//detail::FontCore* Font::resolveFontCore(const detail::FontRequester& fontRequester)
//{
//	if (!m_rawFont) {
//		m_rawFont = m_manager->(m_desc, dpiScale);
//	}
//	return m_rawFont;
//}


namespace detail {

FontRequester::FontRequester()
	: m_manager(detail::FontManager::instance())
	, size(Font::DefaultSize)
{
}

detail::FontCore* FontRequester::resolveFontCore(float scaleFactor)
{
	if (!m_rawFont || !Math::nearEqual(m_lastScaleFactor, scaleFactor)) {
		FontDesc desc;
		desc.Family = (font) ? font->family() : String::Empty;
		desc.Size = size;
		desc.isBold = isBold;
		desc.isItalic = isItalic;
		desc.isAntiAlias = isAntiAlias;
		m_rawFont = m_manager->lookupFontCore(desc, scaleFactor);
		m_lastScaleFactor = scaleFactor;
	}
	return m_rawFont;
}

} // namespace detail
} // namespace ln

