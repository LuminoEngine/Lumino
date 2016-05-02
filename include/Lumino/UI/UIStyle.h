
#pragma once
#include "Common.h"
#include "Detail.h"
#include "../Base/Thickness.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIStyle
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIStyle();
	virtual ~UIStyle();

private:

	Nullable<ThicknessF>			m_margin;
	Nullable<ThicknessF>			m_padding;
	Nullable<VerticalAlignment>		m_verticalAlignment;
	Nullable<HorizontalAlignment>	m_horizontalAlignment;
	BrushPtr						m_background;
	BrushPtr						m_foreground;
	detail::BorderInfo				m_border;
	// UIElementFactory
};

LN_NAMESPACE_END
