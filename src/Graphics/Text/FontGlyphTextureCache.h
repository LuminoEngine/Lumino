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

class FontGlyphTextureCache
	: public RefObject
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	FontGlyphTextureCache(GraphicsManager* manager, Font* font);	// TODO: ストローク幅をpenで表すなら太さ分の引数が増えることになる。

	void LookupGlyph(UTF32 ch, Texture** texture, Rect* srcRect);

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

	TextLayoutEngine	m_layoutEngine;
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

