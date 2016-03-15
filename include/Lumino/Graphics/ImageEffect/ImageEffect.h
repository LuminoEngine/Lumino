
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief
*/
class ImageEffect
	: public Object
{
public:

protected:
	ImageEffect();
	virtual ~ImageEffect();
	virtual void OnRender(RenderingContext2* renderingContext, RenderTarget* source, RenderTarget* destination) = 0;

	friend class ViewportLayer;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
