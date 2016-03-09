
#pragma once
#include "Graphics/Texture.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class Viewport
	: public Object
{
public:


LN_INTERNAL_ACCESS:	// TODO: いまはとりあえず内部用途
	Viewport();
	virtual ~Viewport();
	void Initialize(RenderTarget* renderTarget);

private:
	RenderTarget*	m_renderTarget;
};

LN_NAMESPACE_END
