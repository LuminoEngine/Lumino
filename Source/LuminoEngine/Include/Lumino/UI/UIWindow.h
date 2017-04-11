
#pragma once
#include "UIElement.h"
//#include "../Graphics/Viewport.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIWindow
	: public Object
{
public:

LN_INTERNAL_ACCESS:	// TODO: いまはとりあえず内部用途
	UIWindow();
	virtual ~UIWindow();
	void InitializeDefault();

private:
};

LN_NAMESPACE_END
