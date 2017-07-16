
#pragma once
#include "../Common.h"
#include "Font.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail { class FontGlyphTextureCache; }
namespace detail { class TextLayoutEngine; }

struct GlyphHit
{
	int	index;
};

// TODO: detail
struct TextLayoutResultItem
{
	UTF32	Char;
	FontGlyphLocation	Location;
};

struct TextLayoutResult
{
	SizeI						AreaSize;
	List<TextLayoutResultItem>	Items;
};

/**
	@brief	同じ描画スタイルを適用できる一連のグリフを表します。
*/
class GlyphRun
	: public Object
{
public:
	void setFont(RawFont* font);
	RawFont* getFont() const;
	void setText(const UTF32* str, int len);	// TODO: internal
	void setText(const StringRef& text);
	void setTextAlignment(TextAlignment align);	// TODO: run に持たせるべき？
	const SizeI& getRenderSize();

	/** 行頭からの指定した距離に対応する文字ヒットを取得します。*/
	bool getCharacterHitFromDistance(const Point& pos, GlyphHit* outResult);
	
	/** 行頭から指定した文字までの距離を取得します。*/
	bool getDistanceFromCharacterHit(int index, Point* outPos);

LN_INTERNAL_ACCESS:
	GlyphRun();
	virtual ~GlyphRun();
	void initialize();
	void initialize(detail::GraphicsManager* manager);
	const List<TextLayoutResultItem>& requestLayoutItems();
	detail::FontGlyphTextureCache* lookupFontGlyphTextureCache();
	const UTF32* getText() const { return m_utf32Text.c_str(); }
	int getTextLength() const { return m_utf32Text.getLength(); }

private:
	void updateTextLayoutItem();

	friend class Helper;
	detail::GraphicsManager*			m_manager;
	GenericStringBuilderCore<UTF32>		m_utf32Text;
	TextLayoutResult					m_glyphData;
	detail::TextLayoutEngine*			m_layoutEngine;	// TODO: detail にして new しない
	detail::FontGlyphTextureCache*		m_glyphTextureCache;
	bool								m_modifiedRenderSize;
	bool								m_modifiedItems;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
