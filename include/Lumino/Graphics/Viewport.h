
#pragma once
#include "Texture.h"

LN_NAMESPACE_BEGIN

class RenderingCompositionContext
{
public:
	RenderTarget* RequestRenderTarget(int width, int height);
};

// Camera Ç∆Ç©Ç™åpè≥Ç∑ÇÈ
class IRenderingCompositionItem
{
public:
	void Render(RenderingCompositionContext* context);
};

/**
	@brief		
*/
class Viewport
	: public Object
{
public:


LN_INTERNAL_ACCESS:	// TODO: Ç¢Ç‹ÇÕÇ∆ÇËÇ†Ç¶Ç∏ì‡ïîópìr
	Viewport();
	virtual ~Viewport();
	void Initialize(RenderTarget* renderTarget);

private:
	RenderTarget*	m_renderTarget;
};

LN_NAMESPACE_END
