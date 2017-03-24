
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

	//const SizeI& GetGlyphsTextureSize() const;

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











//template<typename T>
//class FixedCache
//{
//public:
//	
//
//private:
//	std::unordered_map<char32_t, int>	m_glyphInfoIndexMap;
//};


struct VectorFontOutline
{
	int	startIndex = 0;
	int	vertexCount = 0;
};




class VectorFontGlyphCache
	: public RefObject
{
public:
	using Handle = int;

	VectorFontGlyphCache();
	virtual ~VectorFontGlyphCache();
	void Initialize(GraphicsManager* manager, int maxSize);
	int GetMaxCount() const { return m_glyphInfoList.GetCount(); }

	// callby main thread
	Handle GetGlyphInfo(char32_t utf32, bool* outFlushRequested);
	void OnFlush();	// メインスレッドで Flush したときにはこれを呼ぶ

	// callby rendering thread
	int GetTriangleCount(Handle info);
	void GenerateMesh(Handle info, const Vector3& positionOffset, const Matrix& transform, Vertex* outVertices, uint16_t* outIndices, uint16_t beginIndex);

private:
	void ResetUsedFlags();
	void MakePolygons(Handle info, const VectorFontOutline* outlines, int outlineSize, const Vector3* vertices, int vertexSize);
	
	class GryphInfo : public LinkedNode
	{
	public:
		int idIndex;
	};

	struct GryphBufferData
	{
		List<Vector3>	vertices;
		List<uint16_t>	indices;
	};

	GraphicsManager*					m_manager;

	// main thread resource
	List<GryphInfo>						m_glyphInfoList;		// fixed instance list
	std::unordered_map<char32_t, int>	m_glyphInfoIndexMap;
	Stack<int>							m_freeIndexStack;
	std::vector<bool>					m_inFlushUsedFlags;		// TODO: List<bool> 1度の Flush 間で、そのインデックスが使われたかどうか
	int									m_inFlushUsedCount;		// m_inFlushUsedFlags の中の true 数 (最大数に到達したら Flush が必要)

	// rendering thread resource
	List<GryphBufferData>				m_gryphBufferDataList;	// fixed instance list
};


} // namespace detail
LN_NAMESPACE_END
