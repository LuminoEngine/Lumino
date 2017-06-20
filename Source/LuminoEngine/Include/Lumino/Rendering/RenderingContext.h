
#pragma once
#include "Rendering.h"
LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class RenderingContext
	: public DrawList
{
public:

LN_CONSTRUCT_ACCESS:
	RenderingContext();
	virtual ~RenderingContext();
	void initialize();
};

LN_NAMESPACE_END

