#pragma once
#include <LuminoGraphics/Font/detail/FontCore.hpp>

namespace ln {
namespace detail {
class FontCore;
class FontGlyphTextureCache;
struct FontGlyphTextureCacheRequest;

class TextRenderingCache : public AbstractTextRenderingCache
{
public:
	TextRenderingCache(FontCore* fontCore);
	void beginCacheUsing();
	void endCacheUsing();
	void getFontGlyphTextureCache(FontGlyphTextureCacheRequest* inout);

private:
	FontCore* m_fontCore;
	std::array<Ref<FontGlyphTextureCache>, 4> m_fontGlyphTextureCacheList;
	int m_activeCacheIndex;
};

} // namespace detail
} // namespace ln

