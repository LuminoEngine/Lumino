
#pragma once
#include <unordered_map>
#include <Lumino/Base/Cache.h>
#include <Lumino/Base/LinkedNodeList.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/Graphics/Vertex.h>
#include "TextLayoutEngine.h"	// TODO

LN_NAMESPACE_BEGIN
namespace detail {

struct CacheGlyphInfo
{
	//RawBitmap*	fillGlyphBitmap;
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

	void initialize(GraphicsManager* manager, RawFont* font);	// TODO: ストローク幅をpenで表すなら太さ分の引数が増えることになる。


	// まずメインスレッドでこの関数でキャッシュを検索する。
	// outFlush が true になったらキャッシュが一杯になったので、フラッシュする必要がある。
	// RawBitmap が得られたときは CommitAndGetGlyphTextureInfo() に渡す前に、
	// 描画コマンド用の一時メモリにコピーしてか渡さなければならない。
	void lookupGlyphInfo(UTF32 ch, CacheGlyphInfo* outInfo, bool* outFlush);

	// 描画するときにこのテクスチャを使う。
	//Driver::ITexture* getGlyphsFillTexture();
	Texture2D* getGlyphsFillTexture();

	// メインスレッドで Flush したときにはこれを呼ぶ
	void onFlush();

	//const SizeI& GetGlyphsTextureSize() const;

	TextLayoutEngine* getTextLayoutEngine() { return &m_layoutEngine; }
	void measure(const UTF32* text, int length, TextLayoutResult* outResult);	// ユーティリティ

private:
	void resetUsedFlags();

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
	Ref<Texture2D>		m_fillGlyphsTexture;
	//RawBitmap*					m_lockedFillBitmap;

	TextLayoutEngine		m_layoutEngine;	// TODO: GlyphRun に任せていいかも？


};








class VectorFontGlyphCache
	: public RefObject
{
public:
	using Handle = int;

	VectorFontGlyphCache();
	virtual ~VectorFontGlyphCache();
	void initialize(GraphicsManager* manager, RawFont* font, int maxSize);
	int getMaxCount() const { return m_glyphInfoList.getCount(); }

	// callby main thread
	Handle getGlyphInfo(char32_t utf32Code, bool* outFlushRequested);
	void onFlush();	// メインスレッドで Flush したときにはこれを呼ぶ

	// callby rendering thread
	int getVertexCount(Handle info);
	int getIndexCount(Handle info);
	void generateMesh(Handle info, const Vector3& baselineOrigin, const Matrix& transform, Vertex* outVertices, uint16_t* outIndices, uint16_t beginIndex);

private:
	void resetUsedFlags();
	void registerPolygons(Handle infoIndex, const RawFont::FontOutlineVertex* vertices, int vertexSize, const RawFont::OutlineInfo* outlines, int outlineSize);
	
	class GryphInfo : public LinkedNode
	{
	public:
		char32_t utf32Code;
		int idIndex;
	};

	GraphicsManager*					m_manager;
	RawFont*							m_font;

	// main thread resource
	List<GryphInfo>						m_glyphInfoList;		// fixed instance list
	std::unordered_map<char32_t, int>	m_glyphInfoIndexMap;
	int									m_freeIndexCount;
	LinkedNodeList<GryphInfo>			m_olderInfoList;
	std::vector<bool>					m_inFlushUsedFlags;		// TODO: List<bool> 1度の Flush 間で、そのインデックスが使われたかどうか
	int									m_inFlushUsedCount;		// m_inFlushUsedFlags の中の true 数 (最大数に到達したら Flush が必要)

	// rendering thread resource
	List<RawFont::VectorGlyphInfo>		m_gryphBufferDataList;	// fixed instance list
};


} // namespace detail
LN_NAMESPACE_END
