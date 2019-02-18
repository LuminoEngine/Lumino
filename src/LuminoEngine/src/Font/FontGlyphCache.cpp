
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include "FontCore.hpp"
#include "FontGlyphCache.hpp"

namespace ln {
namespace detail {

//==============================================================================
// FontGlyphTextureCache

FontGlyphTextureCache::FontGlyphTextureCache()
	: m_maxCacheGlyphs(0)
	, m_glyphMaxBitmapSize()
	, m_glyphWidthCount(0)
	, m_fillGlyphsTexture(nullptr)
	, m_indexStack()
	, m_cachedGlyphInfoMap()
	, m_curPrimUsedFlags()
	, m_curPrimUsedCount(0)
{
}

bool FontGlyphTextureCache::init(FontCore* font)
{
	if (LN_REQUIRE(font)) return false;

	m_font = font;
	m_maxCacheGlyphs = 2048;// TODO 定数なのはなんとかしたい

	// すべてのグリフが収まるビットマップサイズ
	FontGlobalMetrics metrix;
	m_font->getGlobalMetrics(&metrix);
	int mw = std::ceil(metrix.boundingMaxX - metrix.boundingMinX);
	int mh = std::ceil(metrix.boundingMaxY - metrix.boundingMinY);

	// 横方向に並べる数
	// +1.0 は切り捨て対策。テクスチャサイズはmaxCharactersが収まる大きさであれば良い。(小さくなければOK)
	m_glyphWidthCount = (int)(sqrt((double)m_maxCacheGlyphs) + 1.0);

	// キャッシュ用テクスチャ作成
	int w = m_glyphWidthCount * std::max(mw, mh);
	m_glyphMaxBitmapSize = SizeI(w, w);
	m_fillGlyphsTexture = newObject<Texture2D>(m_glyphMaxBitmapSize.width, m_glyphMaxBitmapSize.height, TextureFormat::RGBA32, false, GraphicsResourceUsage::Dynamic);
	// TODO: 最大 DeviceSize チェック

	// 検索に使う情報をリセット
	m_curPrimUsedFlags.resize(m_maxCacheGlyphs);
	for (int i = 0; i < m_maxCacheGlyphs; i++) {
		m_indexStack.push(i);
	}
	resetUsedFlags();

	return true;
}

void FontGlyphTextureCache::lookupGlyphInfo(UTF32 ch, CacheGlyphInfo* outInfo, bool* outFlush)
{
	LN_DCHECK(outInfo);
	LN_DCHECK(outFlush);

	int cacheIndex = -1;
	CachedGlyphInfoMap::const_iterator itr = m_cachedGlyphInfoMap.find(ch);
	if (itr != m_cachedGlyphInfoMap.end())
	{
		const CachedGlyphInfo& info = itr->second;
		cacheIndex = info.index;
		outInfo->outlineOffset = 0;
		outInfo->srcRect.set(	// 描画スレッド側で作るといろいろな情報にアクセスしなければならないのでここで作ってしまう
			((info.index % m_glyphWidthCount) * m_glyphMaxBitmapSize.width),
			((info.index / m_glyphWidthCount) * m_glyphMaxBitmapSize.height),
			info.size.width, info.size.height);
	}
	else
	{
		if (m_indexStack.size() == 0) {
			// TODO: 古いキャッシュ破棄
			LN_NOTIMPLEMENTED();
		}

		// ビットマップを新しく作ってキャッシュに登録したい
		BitmapGlyphInfo bitmapGlyphInfo;
		m_font->lookupGlyphBitmap(ch, &bitmapGlyphInfo);

		// 空いてるインデックスを取りだす
		cacheIndex = m_indexStack.top();
		m_indexStack.pop();

		// キャッシュマップに登録
		CachedGlyphInfo info;
		info.index = cacheIndex;
		info.size = bitmapGlyphInfo.size;//bitmapGlyphInfo.glyphBitmap->();
		m_cachedGlyphInfoMap[ch] = info;

		outInfo->outlineOffset = 0; // TODO: もしかしたら必要ないかも //bitmapGlyphInfo->OutlineOffset;
		outInfo->srcRect.set(	// 描画スレッド側で作るといろいろな情報にアクセスしなければならないのでここで作ってしまう
			((info.index % m_glyphWidthCount) * m_glyphMaxBitmapSize.width),
			((info.index / m_glyphWidthCount) * m_glyphMaxBitmapSize.height),
			info.size.width, info.size.height);

		// Fill
		PointI pt(outInfo->srcRect.x + outInfo->outlineOffset, outInfo->srcRect.y + outInfo->outlineOffset);
		Bitmap2D* dst = m_fillGlyphsTexture->map(MapMode::Write);
		dst->blit(RectI(pt.x, pt.y, info.size), bitmapGlyphInfo.glyphBitmap, RectI(0, 0, info.size), Color32::White, BitmapBlitOptions::None);
	}

	// 今回、cacheIndex を使うことをマーク
	if (!m_curPrimUsedFlags[cacheIndex]) {
		m_curPrimUsedFlags[cacheIndex] = true;
		m_curPrimUsedCount++;
	}

	// キャッシュが一杯になっていないかチェック。
	// 一杯になってたら呼び出し元に Flush してもらわないと、一部の文字が描画できないことになる。
	if (m_curPrimUsedCount == m_maxCacheGlyphs) {
		resetUsedFlags();
		(*outFlush) = true;
	}
	else {
		(*outFlush) = false;
	}
}

void FontGlyphTextureCache::onFlush()
{
	resetUsedFlags();
}

void FontGlyphTextureCache::resetUsedFlags()
{
	for (int i = 0; i < m_maxCacheGlyphs; i++) {
		m_curPrimUsedFlags[i] = false;
	}
	m_curPrimUsedCount = 0;
}

} // namespace detail
} // namespace ln

