/**
	@file	GlyphRun.h
*/
#pragma once
#include "Common.h"

namespace Lumino
{
namespace Graphics
{

/**
	@brief	同じ描画スタイルを適用できる一連のグリフを表します。
*/
class GlyphRun
	: public RefObject
{
public:
	GlyphRun();
	virtual ~GlyphRun();

private:
	void AttachGlyphTextureCache(Internal::FontGlyphTextureCache* cache);

private:
	friend class Helper;
	Internal::FontGlyphTextureCache*	m_glyphTextureCache;
	TextLayoutResult*					m_glyphData;
};

} // namespace Graphics
} // namespace Lumino
