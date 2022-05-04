#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoGraphics/RHI/Texture.hpp>
#include "../../Font/src/FontCore.hpp"
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
	m_maxCacheGlyphs = 1024;// TODO 定数なのはなんとかしたい
    // TODO: 最大に達するとなんか落ちる

	// すべてのグリフが収まるビットマップサイズ
	FontGlobalMetrics metrix;
	m_font->getGlobalMetrics(&metrix);
	int mw = metrix.lineSpace;
	int mh = metrix.lineSpace;

	// 横方向に並べる数
	// +1.0 は切り捨て対策。テクスチャサイズはmaxCharactersが収まる大きさであれば良い。(小さくなければOK)
	m_glyphWidthCount = (int)(sqrt((double)m_maxCacheGlyphs) + 1.0);

	// キャッシュ用テクスチャ作成
    int w = std::max(mw, mh);
    m_glyphMaxBitmapSize = SizeI(w, w);
    SizeI textureSize(m_glyphWidthCount * w, m_glyphWidthCount * w);
	m_fillGlyphsTexture = makeObject<Texture2D>(textureSize.width, textureSize.height, TextureFormat::RGBA8);
	m_fillGlyphsTexture->setResourceUsage(GraphicsResourceUsage::Dynamic);
	// TODO: 最大 DeviceSize チェック

	// 検索に使う情報をリセット
	m_curPrimUsedFlags.resize(m_maxCacheGlyphs);
	for (int i = 0; i < m_maxCacheGlyphs; i++) {
		m_indexStack.push(i);
	}
	resetUsedFlags();

	return true;
}

void FontGlyphTextureCache::clearIndex()
{
	// TODO:
}

bool FontGlyphTextureCache::requestGlyphs(FontGlyphTextureCacheRequest* request)
{
	int missingCount = 0;
	for (auto& item : request->glyphs) {
		if (m_cachedGlyphInfoMap.find(item.codePoint) == m_cachedGlyphInfoMap.end()) {
			missingCount++;
		}
	}
	if (missingCount > m_indexStack.size()) {
		// 新しい文字を作りたいが、このキャッシュには収まりきらない
		return false;
	}
	else {
		for (auto& item : request->glyphs) {
			bool dummy;
			lookupGlyphInfo(item.codePoint, &item.info, &dummy);
		}
		request->texture = m_fillGlyphsTexture;
		return true;
	}
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
		dst->blit(RectI(pt.x, pt.y, info.size), bitmapGlyphInfo.glyphBitmap, RectI(0, 0, info.size), ColorI::White, BitmapBlitOptions::None);

		//dst->save(u"test.png");
		//bitmapGlyphInfo.glyphBitmap->save(u"test2.png");
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

