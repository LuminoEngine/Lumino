
#pragma once
#include "TextRenderer.h"

namespace Lumino
{
namespace Graphics
{

/**
	@brief		
*/
class GlyphRun
	: public RefObject
{
public:
	GlyphRun() {}
	virtual ~GlyphRun() {}

public:
	RefPtr<FontGlyphTextureCache>	m_glyphTextureCache;
	Imaging::TextLayoutResult		m_glyphData;
};

} // namespace Graphics
} // namespace Lumino
