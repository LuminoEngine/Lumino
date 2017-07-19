/*
	# FontAwesomeVariables.inl を作るコード

	StreamReader r("font-awesome-4.7.0/less/variables.less");
	StreamWriter w("FontAwesomeVariables.inl");
	String line;
	while (r.readLine(&line))
	{
		if (line.contains("@fa-var-"))
		{
			line = line.replace("@fa-var-", "");
			MatchResult result;
			if (Regex::match(line, "([A-Za-z0-9-]+):\\s\"\\\\([A-Za-z0-9-]+)\";", &result))
			{
				w.writeLine("{{ \"fa-{0}\", 0x{1} }},", result[1], result[2]);
			}
		}
	}
*/
#include "../Internal.h"
#include "GlyphIconFontManager.h"

LN_NAMESPACE_BEGIN
namespace detail {

GlyphIconFontManager::GlyphIconFontManager()
	: m_glyphIconFontMap()
	, m_fontAwesomeVariablesMap({
#include "FontAwesomeVariables.inl"
	})
{
}

GlyphIconFontManager::~GlyphIconFontManager()
{
}

void GlyphIconFontManager::initialize()
{
}

void GlyphIconFontManager::dispose()
{
	m_glyphIconFontMap.clear();
}

Font* GlyphIconFontManager::getGlyphIconFont(int size)
{
	Font* font;
	auto itr = m_glyphIconFontMap.find(size);
	if (itr != m_glyphIconFontMap.end())
	{
		font = itr->second;
	}
	else
	{
		auto newFont = newObject<Font>(_T("FontAwesome"), size);
		m_glyphIconFontMap[size] = newFont;
		font = newFont;
	}
	return font;
}

uint32_t GlyphIconFontManager::getFontAwesomeCodePoint(const StringRef& glyphName)
{
	auto itr = m_fontAwesomeVariablesMap.find(glyphName);
	if (itr != m_fontAwesomeVariablesMap.end())
	{
		return itr->second;
	}
	else
	{
		LN_ENSURE(0, "not found FontAwesomeCodePoint.");
		return 0;
	}
}

} // namespace detail
LN_NAMESPACE_END
