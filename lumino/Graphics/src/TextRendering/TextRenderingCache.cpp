#include <LuminoGraphics/GPU/Texture.hpp>
#include "TextRenderingCache.hpp"
#include "FontGlyphCache.hpp"

namespace ln {
namespace detail {

//==============================================================================
// TextRenderingCache

TextRenderingCache::TextRenderingCache(FontCore* fontCore)
	: m_fontCore(fontCore)
	, m_fontGlyphTextureCacheList{}
	, m_activeCacheIndex(0)
{
}

void TextRenderingCache::beginCacheUsing()
{
}

void TextRenderingCache::endCacheUsing()
{
	m_activeCacheIndex = 0;
	for (auto& cache : m_fontGlyphTextureCacheList) {
		if (cache) {
			cache->clearIndex();
		}
	}
}

void TextRenderingCache::getFontGlyphTextureCache(FontGlyphTextureCacheRequest* inout)
{
	for (int i = m_activeCacheIndex; i < m_fontGlyphTextureCacheList.size(); i++) {
		if (!m_fontGlyphTextureCacheList[m_activeCacheIndex]) {
			m_fontGlyphTextureCacheList[m_activeCacheIndex] = makeRef<FontGlyphTextureCache>();
			m_fontGlyphTextureCacheList[m_activeCacheIndex]->init(m_fontCore);
		}

		if (m_fontGlyphTextureCacheList[m_activeCacheIndex]->requestGlyphs(inout)) {
			return;
		}
        m_activeCacheIndex++;
	}

	// TODO: ここまで来た場合は一度にテクスチャに収まりきらないほど大量の文字を書こうとしたか、
	// 4枚分のキャッシュに収まらないほどの文字を書こうとした。
	// 後者の場合は、今は現フレームでは使われていない文字もカウントしてしまうので対策の余地はある。
	LN_NOTIMPLEMENTED();
}
} // namespace detail
} // namespace ln

