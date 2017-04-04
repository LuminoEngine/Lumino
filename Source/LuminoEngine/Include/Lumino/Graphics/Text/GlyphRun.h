
#pragma once
#include "../Common.h"
#include "Font.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail { class FontGlyphTextureCache; }
namespace detail { class TextLayoutEngine; }

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
	void SetFont(RawFont* font);
	RawFont* GetFont() const;
	void SetText(const UTF32* str, int len);	// TODO: internal
	void SetText(const StringRef& text);
	void SetTextAlignment(TextAlignment align);	// TODO: Run に持たせるべき？
	const SizeI& GetRenderSize();

LN_INTERNAL_ACCESS:
	GlyphRun();
	virtual ~GlyphRun();
	void Initialize();
	void Initialize(detail::GraphicsManager* manager);
	const List<TextLayoutResultItem>& RequestLayoutItems();
	detail::FontGlyphTextureCache* LookupFontGlyphTextureCache();
	const UTF32* GetText() const { return m_utf32Text.c_str(); }
	int GetTextLength() const { return m_utf32Text.GetLength(); }

private:
	void UpdateTextLayoutItem();

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
