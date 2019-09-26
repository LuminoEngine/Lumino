
#include "Internal.hpp"
#include "FontManager.hpp"
#include "FontCore.hpp"
#include "FontGlyphCache.hpp"

namespace ln {
namespace detail {

//==============================================================================
// FontCore
	
FontCore::FontCore()
	: m_manager(nullptr)
{
}

void FontCore::init(FontManager* manager)
{
	if (LN_REQUIRE(manager)) return;
	m_manager = manager;
	m_manager->addAliveFontCore(this);
}

void FontCore::dispose()
{
	if (m_manager)
	{
		m_manager->removeAliveFontCore(this);
		m_manager = nullptr;
	}
}

void FontCore::finalize()
{
	// ObjectCache から削除されるとき用
	dispose();
}

//FontGlyphTextureCache* FontCore::getFontGlyphTextureCache()
//{
//	if (!m_fontGlyphTextureCache)
//	{
//		m_fontGlyphTextureCache = makeRef<FontGlyphTextureCache>();
//		m_fontGlyphTextureCache->init(this);
//	}
//	return m_fontGlyphTextureCache;
//}

void FontCore::beginCacheUsing()
{
	m_activeCacheIndex = 0;
	for (auto& cache : m_fontGlyphTextureCacheList) cache->clearIndex();
}

void FontCore::endCacheUsing()
{
}

void FontCore::getFontGlyphTextureCache(FontGlyphTextureCacheRequest* inout)
{
	for (int i = m_activeCacheIndex; i < m_fontGlyphTextureCacheList.size(); i++) {
		if (!m_fontGlyphTextureCacheList[i]) {
			m_fontGlyphTextureCacheList[i] = makeRef<FontGlyphTextureCache>();
			m_fontGlyphTextureCacheList[i]->init(this);
		}

		if (m_fontGlyphTextureCacheList[i]->requestGlyphs(inout)) {
			return;
		}
	}

	// TODO: ここまで来た場合は一度にテクスチャに収まりきらないほど大量の文字を書こうとしたか、
	// 4枚分のキャッシュに収まらないほどの文字を書こうとした。
	// 後者の場合は、今は現フレームでは使われていない文字もカウントしてしまうので対策の余地はある。
	LN_NOTIMPLEMENTED();
}

} // namespace detail
} // namespace ln

