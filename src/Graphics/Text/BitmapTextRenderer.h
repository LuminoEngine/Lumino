
#pragma once
#include <Lumino/Graphics/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

class BitmapTextRenderer
	: public RefObject
{
public:
	BitmapTextRenderer();
	virtual ~BitmapTextRenderer();

	void Initialize(FontManager* manager);

private:
	FontManager*	m_manager;
	GlyphRun*		m_tmpGlyphRun;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
