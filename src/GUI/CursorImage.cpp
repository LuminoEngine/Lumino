
#include "../Internal.h"
#include <Lumino/GUI/CursorImage.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// CursorImage
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CursorImage::CursorImage(GUIManagerImpl* manager)
	: m_manager(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CursorImage::~CursorImage()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CursorImage::Draw(RenderingContext* painter, const PointF& point, float time)
{
	if (painter == NULL) { return; }
	if (m_texture == NULL) { return; }
	if (m_srcRect.IsEmpty()) { return; }

	float localTime = fmodf(time, m_patternDuration);
	float unitTime = m_patternDuration / m_patternCount;
	int pattern = (int)(localTime / unitTime);

	Rect srcRect(m_srcRect.X + pattern * m_srcRect.Width, m_srcRect.Y, m_srcRect.Width, m_srcRect.Height);
	painter->SetTexture(m_texture, srcRect);
	painter->DrawRectangle(RectF(point, (float)m_srcRect.Width, (float)m_srcRect.Height));
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

