
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN
class DrawList;

/**
	@brief
*/
class ImageEffect
	: public Object
{
	LN_OBJECT;
public:

protected:
	ImageEffect();
	virtual ~ImageEffect();
	void initialize(detail::GraphicsManager* manager);

	virtual void onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;

	friend class ViewportLayer;

	detail::GraphicsManager*	m_manager;
};

LN_NAMESPACE_END
