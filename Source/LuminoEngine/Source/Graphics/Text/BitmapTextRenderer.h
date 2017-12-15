
#pragma once
#include <Lumino/Graphics/Common.h>

LN_NAMESPACE_BEGIN

class BitmapTextRenderer
	: public RefObject
{
public:
	BitmapTextRenderer();
	virtual ~BitmapTextRenderer();

	void initialize(detail::GraphicsManager* manager);

	GlyphRun* getTempGlyphRun() { return m_tmpGlyphRun; }	// テクスチャの drawText 等、ユーティリティ的に使いたいときに new を減らす

	void setRenderArea(const RectI& area) { m_renderArea = area; }
	void setTextAlignment(TextAlignment align) { m_textAlignment = align; }
	void drawGlyphRun(RawBitmap* target, GlyphRun* glyphRun, const Color32& fillColor, const Color32& strokeColor, int strokeThickness);

private:
	detail::GraphicsManager*	m_manager;
	GlyphRun*					m_tmpGlyphRun;
	RectI						m_renderArea;
	TextAlignment				m_textAlignment;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
