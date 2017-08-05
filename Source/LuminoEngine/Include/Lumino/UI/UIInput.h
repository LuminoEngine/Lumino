
#pragma once

LN_NAMESPACE_BEGIN
class UIElement;

/**
	@brief		
*/
class UIMouse
{
public:

LN_INTERNAL_ACCESS:
	static Point getPosition(UIElement* relativeTo);
};

LN_NAMESPACE_END
