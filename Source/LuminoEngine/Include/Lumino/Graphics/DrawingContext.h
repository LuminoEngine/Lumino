
#pragma once
#include "Rendering.h"

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class DrawingContext
	: public DrawList
{
public:

LN_CONSTRUCT_ACCESS:
	DrawingContext();
	virtual ~DrawingContext();
	void Initialize();
};

LN_NAMESPACE_END
