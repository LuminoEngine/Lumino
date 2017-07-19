
#pragma once
#include <unordered_map>
#include <Lumino/Graphics/Text/Font.h>

LN_NAMESPACE_BEGIN
namespace detail {

// Font Awesome や Emoji アイコンのフォント管理を行う。
// GlyphIcon オブジェクトでシーングラフのノードの1つとして描画すると、Font インスタンスが大量に作られる可能性がある。
// これらは基本的にサイズが異なるだけで多くのプロパティが同一であるため、できるだけ一元管理する。
class GlyphIconFontManager
	: public RefObject
{
public:
	GlyphIconFontManager();
	virtual ~GlyphIconFontManager();
	void initialize();
	void dispose();

	Font* getGlyphIconFont(int size);
	uint32_t getFontAwesomeCodePoint(const StringRef& glyphName);

private:
	std::unordered_map<int, Ref<Font>>		m_glyphIconFontMap;
	std::unordered_map<String, uint32_t>	m_fontAwesomeVariablesMap;
};

} // namespace detail
LN_NAMESPACE_END
