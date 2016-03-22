/**
	@file	GlyphRun.h
*/
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief	同じ描画スタイルを適用できる一連のグリフを表します。
*/
class GlyphRun
	: public RefObject
{
public:
	GlyphRun();
	virtual ~GlyphRun();

public:
	void Update(const UTF32* text, int length);
	const Size GetRenderSize() const;

private:
	void AttachGlyphTextureCache(Internal::FontGlyphTextureCache* cache);

private:
	friend class Helper;
	Internal::FontGlyphTextureCache*	m_glyphTextureCache;
	TextLayoutResult*					m_glyphData;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
