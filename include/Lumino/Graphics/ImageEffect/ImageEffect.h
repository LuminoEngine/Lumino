
#pragma once
#include "../../Animation/AnimatableObject.h"
#include "../Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief
*/
class ImageEffect
	: public AnimatableObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

protected:
	ImageEffect();
	virtual ~ImageEffect();
	void Initialize(GraphicsManager* manager);

	virtual void OnRender(RenderingContext* renderingContext, RenderTarget* source, RenderTarget* destination) = 0;

	friend class ViewportLayer;

	GraphicsManager*	m_manager;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
