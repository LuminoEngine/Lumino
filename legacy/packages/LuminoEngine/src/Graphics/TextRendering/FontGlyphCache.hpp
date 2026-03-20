#pragma once

namespace ln {
class Texture2D;
namespace detail {
class FontCore;

struct CacheGlyphInfo
{
	int		outlineOffset;
	RectI	srcRect;
};

struct FontGlyphTextureCacheRequestItem
{
	uint32_t codePoint;	// input
	CacheGlyphInfo info;
};

struct FontGlyphTextureCacheRequest
{
	std::vector<FontGlyphTextureCacheRequestItem> glyphs;
	Texture2D* texture;
};


// このクラスは、1つのテクスチャにできる限りグリフを詰め込むために使用する。
// たくさん文字を書くときでも、テクスチャの切り替えが無ければ1度のドローコールで全て書くことができる。
class FontGlyphTextureCache
	: public RefObject
{
public:
	FontGlyphTextureCache();
	bool init(FontCore* font);

	void clearIndex();
	bool requestGlyphs(FontGlyphTextureCacheRequest* request);

	// まずメインスレッドでこの関数でキャッシュを検索する。
	// GlyphsFillTexture にビットマップ転送を行い、その結果の領域を返す。
	// outFlush が true になったら、この書き込みでキャッシュが一杯になったのでフラッシュする必要がある。
	void lookupGlyphInfo(UTF32 ch, CacheGlyphInfo* outInfo, bool* outFlush);

	// 描画するときにこのテクスチャを使う。resolve してレンダリングスレッドに渡す。
	const Ref<Texture2D>& glyphsFillTexture() const { return m_fillGlyphsTexture; }

	// メインスレッドで Flush した後にはこれを呼ぶ。
	void onFlush();

private:
	struct CachedGlyphInfo
	{
		int index;
		SizeI size;
	};

	typedef std::unordered_map<UTF32, CachedGlyphInfo>	CachedGlyphInfoMap;

	void resetUsedFlags();

	FontCore* m_font;

	int m_maxCacheGlyphs;						// キャッシュできる最大文字数
	SizeI m_glyphMaxBitmapSize;					// 現在のフォントの1文字分のビットマップの最大サイズ
	int m_glyphWidthCount;
	Ref<Texture2D> m_fillGlyphsTexture;

	std::stack<int> m_indexStack;				// 空きキャッシュインデックス
	CachedGlyphInfoMap m_cachedGlyphInfoMap;
	std::vector<bool> m_curPrimUsedFlags;		// 1度の Flush 間で、そのインデックスが使われたかどうか
	int m_curPrimUsedCount;						// m_curPrimUsedFlags の中の true 数 (m_maxCacheGlyphs に到達したら Flush が必要)
};

} // namespace detail
} // namespace ln

