
#pragma once
#include <Lumino/Graphics/Common.h>

LN_NAMESPACE_BEGIN

class BitmapTextRenderer
	: public RefObject
{
public:
	BitmapTextRenderer();
	virtual ~BitmapTextRenderer();

	void Initialize(detail::GraphicsManager* manager);

	GlyphRun* GetTempGlyphRun() { return m_tmpGlyphRun; }	// テクスチャの DrawText 等、ユーティリティ的に使いたいときに new を減らす

	void SetRenderArea(const Rect& area) { m_renderArea = area; }
	void SetTextAlignment(TextAlignment align) { m_textAlignment = align; }
	void DrawGlyphRun(Bitmap* target, GlyphRun* glyphRun, const Color32& fillColor, const Color32& strokeColor, int strokeThickness);

private:
	detail::GraphicsManager*	m_manager;
	GlyphRun*					m_tmpGlyphRun;
	Rect						m_renderArea;
	TextAlignment				m_textAlignment;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
