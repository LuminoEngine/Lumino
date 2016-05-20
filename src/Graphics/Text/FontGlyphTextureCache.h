/**
	@file	FontGlyphTextureCache.h
*/
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Text/Font.h>
#include "TextLayoutEngine.h"	// TODO


LN_BEGIN_INTERNAL_NAMESPACE(Graphics)

struct CacheGlyphInfo
{
	//Bitmap*	fillGlyphBitmap;
	// TODO: outlinebitmap
	int		outlineOffset;
	Rect	srcRect;
};

class FontGlyphTextureCache
	: public RefObject
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	FontGlyphTextureCache();
	virtual ~FontGlyphTextureCache();

	void Initialize(GraphicsManager* manager, Font* font);	// TODO: ストローク幅をpenで表すなら太さ分の引数が増えることになる。


	// まずメインスレッドでこの関数でキャッシュを検索する。
	// outFlush が true になったらキャッシュが一杯になったので、フラッシュする必要がある。
	// Bitmap が得られたときは CommitAndGetGlyphTextureInfo() に渡す前に、
	// 描画コマンド用の一時メモリにコピーしてか渡さなければならない。
	void LookupGlyphInfo(UTF32 ch, CacheGlyphInfo* outInfo, bool* outFlush);

	// 次に描画スレッドで、LookupFillGlyph() によって取得して CacheGlyphInfo を指定する。
	// LookupFillGlyph() の呼び出し回数と対応させなければならない。
	// 同時に、得られた srcRect で頂点バッファを作る。
	//void CommitCacheGlyphInfo(CacheGlyphInfo* info, Rect* srcFillRect, Rect* srcOutlineRect);

	// 最後に、描画するときにこのテクスチャを使う。
	Driver::ITexture* GetGlyphsFillTexture();

	// メインスレッドで Flush したときにはこれを呼ぶ
	void OnFlush();

	const Size& GetGlyphsTextureSize() const;

	TextLayoutEngine* GetTextLayoutEngine() { return &m_layoutEngine; }
	void Measure(const UTF32* text, int length, TextLayoutResult* outResult);	// ユーティリティ

private:
	void ResetUsedFlags();

	struct CachedGlyphInfo
	{
		int		index;
		Size	size;
	};

	typedef std::map<UTF32, CachedGlyphInfo>	CachedGlyphInfoMap;

	GraphicsManager*		m_manager;
	RefPtr<Font>			m_font;
	int						m_maxCacheGlyphs;		// キャッシュできる最大文字数
	Size					m_glyphMaxBitmapSize;	// 現在のフォントの1文字分のビットマップの最大サイズ
	int						m_glyphWidthCount;
	Stack<int>				m_indexStack;			// 空きキャッシュインデックス
	CachedGlyphInfoMap		m_cachedGlyphInfoMap;
	std::vector<bool>		m_curPrimUsedFlags;		// TODO: Array<bool> 1度の Flush 間で、そのインデックスが使われたかどうか
	int						m_curPrimUsedCount;		// m_curPrimUsedFlags の中の true 数 (m_maxCacheGlyphs に到達したら Flush が必要)

	//Driver::ITexture*		m_glyphsFillTexture;
	Texture2D*				m_fillGlyphsTexture;
	//Bitmap*					m_lockedFillBitmap;

	TextLayoutEngine		m_layoutEngine;	// TODO: GlyphRun に任せていいかも？



#if 0
	FontGlyphTextureCache(GraphicsManager* manager, Font* font);	// TODO: ストローク幅をpenで表すなら太さ分の引数が増えることになる。

	void LookupGlyph(UTF32 ch, int strokeThickness, Texture** texture, Rect* srcRect);

	uint64_t CalcFontSettingHash() const;

	TextLayoutEngine* GetTextLayoutEngine() { return &m_layoutEngine; }

	void Measure(const UTF32* text, int length, TextLayoutResult* outResult);	// ユーティリティ

private:
	struct CachedGlyphInfo
	{
		int		Index;
		Size	Size;
	};

	typedef std::map<UTF32, CachedGlyphInfo>	CachedGlyphInfoMap;

	GraphicsManager*		m_manager;
	RefPtr<Font>			m_font;
	RefPtr<Texture2D>		m_glyphCacheTexture;
	CachedGlyphInfoMap		m_cachedGlyphInfoMap;
	int						m_glyphWidthCount;
	Size					m_glyphMaxBitmapSize;	///< 現在のフォント情報の1文字分のビットマップの最大サイズ
	Stack<int>				m_indexStack;			///< 空きキャッシュインデックス
	RefPtr<Bitmap>			m_tmpBitmap;

	TextLayoutEngine	m_layoutEngine;	// TODO: GlyphRun に任せていいかも？
#endif
};

LN_END_INTERNAL_NAMESPACE

//LN_NAMESPACE_BEGIN
//LN_NAMESPACE_GRAPHICS_BEGIN
//
#if 0
/**
	@file	文字列の描画を行うクラスです。

	TODO: このクラスは internal にしたい。Painter に任せる。
*/
class TextRenderer
	: public RefObject
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:

	/**
		@brief		TextRenderer のインスタンスを作成します。
	*/
	static TextRenderer* Create(GraphicsManager* manager);

public:
	void SetTransform(const Matrix& matrix);
	void SetViewProjection(const Matrix& view, const Matrix& proj, const Size& viewPixelSize);


	void SetFont(Font* font) { m_font = font; m_layoutEngine.SetFont(font); m_fontFaceModified = true; }
	void SetForeColor(Color color) { m_foreColor = color; }
	void SetStrokeColor(Color color) { m_strokeColor = color; }
	void SetStrokeSize(int size) { m_strokeSize = size; m_fontFaceModified = true; }
	void SetTextAlignment(TextAlignment align) { m_layoutEngine.SetTextAlignment(align); }
	void SetTextTrimming(TextTrimming triming) { m_layoutEngine.SetTextTrimming(triming); }
	void SetFlowDirection(FlowDirection dir) { m_layoutEngine.SetFlowDirection(dir); }
	//void SetDrawingArea(const Rect& area) { m_drawingArea = area; }	// いらないかも

	/**
		@brief		
	*/
	void Measure(const UTF32* text, int length, TextLayoutResult* outResult);

	/**
		@brief		
	*/
	void DrawText(const char* text, int length = -1);
	void DrawText(const wchar_t* text, int length = -1);
	void DrawChar(UTF32 ch);

	/**
		@brief		要求されているスプライトを全て描画します。
	*/
	void Flash();

private:
	struct CachedGlyphInfo
	{
		int		Index;
		Size	Size;
	};

	void DrawSprite(const CachedGlyphInfo& info, const Point& point);
	void CheckResetCache();

private:
	TextRenderer(GraphicsManager* manager);
	virtual ~TextRenderer();
	//GraphicsManager*			m_manager;

	typedef std::map<UTF32, CachedGlyphInfo>	CachedGlyphInfoMap;

	RefPtr<SpriteRenderer>	m_spriteRenderer;

	RefPtr<Font>	m_font;
	Color				m_foreColor;
	Color				m_strokeColor;
	int					m_strokeSize;
	TextLayoutEngine	m_layoutEngine;
	//TextAlignment		m_textAlignment;
	//TextTrimming		m_textTrimming;
	//FlowDirection		m_flowDirection;
	//Rect				m_drawingArea;

	FontGlyphLocation*	m_prevGlyphLocationData;

	// キャッシュ系情報
	RefPtr<Texture>		m_glyphCacheTexture;
	CachedGlyphInfoMap	m_cachedGlyphInfoMap;
	int					m_glyphWidthCount;
	Size				m_glyphMaxBitmapSize;	///< 現在のフォント情報の1文字分のビットマップの最大サイズ
	Stack<int>			m_indexStack;			///< 空きキャッシュインデックス
	RefPtr<Bitmap>	m_tmpBitmap;
	bool				m_fontFaceModified;
};
#endif


//LN_NAMESPACE_GRAPHICS_END

