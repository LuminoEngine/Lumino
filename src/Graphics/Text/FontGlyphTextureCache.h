
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Text/Font.h>
#include "TextLayoutEngine.h"	// TODO

LN_NAMESPACE_BEGIN
namespace detail {

struct CacheGlyphInfo
{
	//Bitmap*	fillGlyphBitmap;
	// TODO: outlinebitmap
	int		outlineOffset;
	RectI	srcRect;
};

// このクラスは、1つのテクスチャにできる限りグリフを詰め込むために使用する。
// たくさん文字を書くときでも、テクスチャの切り替えが無ければ1度のドローコールで全て書くことができる。
class FontGlyphTextureCache
	: public RefObject
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	FontGlyphTextureCache();
	virtual ~FontGlyphTextureCache();

	void Initialize(GraphicsManager* manager, RawFont* font);	// TODO: ストローク幅をpenで表すなら太さ分の引数が増えることになる。


	// まずメインスレッドでこの関数でキャッシュを検索する。
	// outFlush が true になったらキャッシュが一杯になったので、フラッシュする必要がある。
	// Bitmap が得られたときは CommitAndGetGlyphTextureInfo() に渡す前に、
	// 描画コマンド用の一時メモリにコピーしてか渡さなければならない。
	void LookupGlyphInfo(UTF32 ch, CacheGlyphInfo* outInfo, bool* outFlush);

	// 次に描画スレッドで、LookupFillGlyph() によって取得して CacheGlyphInfo を指定する。
	// LookupFillGlyph() の呼び出し回数と対応させなければならない。
	// 同時に、得られた srcRect で頂点バッファを作る。
	//void CommitCacheGlyphInfo(CacheGlyphInfo* info, RectI* srcFillRect, RectI* srcOutlineRect);

	// 最後に、描画するときにこのテクスチャを使う。
	Driver::ITexture* GetGlyphsFillTexture();

	// メインスレッドで Flush したときにはこれを呼ぶ
	void OnFlush();

	const SizeI& GetGlyphsTextureSize() const;

	TextLayoutEngine* GetTextLayoutEngine() { return &m_layoutEngine; }
	void Measure(const UTF32* text, int length, TextLayoutResult* outResult);	// ユーティリティ

private:
	void ResetUsedFlags();

	struct CachedGlyphInfo
	{
		int		index;
		SizeI	size;
	};

	typedef std::map<UTF32, CachedGlyphInfo>	CachedGlyphInfoMap;

	GraphicsManager*		m_manager;
	RawFont*				m_font;
	int						m_maxCacheGlyphs;		// キャッシュできる最大文字数
	SizeI					m_glyphMaxBitmapSize;	// 現在のフォントの1文字分のビットマップの最大サイズ
	int						m_glyphWidthCount;
	Stack<int>				m_indexStack;			// 空きキャッシュインデックス
	CachedGlyphInfoMap		m_cachedGlyphInfoMap;
	std::vector<bool>		m_curPrimUsedFlags;		// TODO: Array<bool> 1度の Flush 間で、そのインデックスが使われたかどうか
	int						m_curPrimUsedCount;		// m_curPrimUsedFlags の中の true 数 (m_maxCacheGlyphs に到達したら Flush が必要)

	//Driver::ITexture*		m_glyphsFillTexture;
	Texture2D*				m_fillGlyphsTexture;
	//Bitmap*					m_lockedFillBitmap;

	TextLayoutEngine		m_layoutEngine;	// TODO: GlyphRun に任せていいかも？


};

} // namespace detail
LN_NAMESPACE_END
