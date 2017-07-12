
#pragma once
#include "Rendering.h"
LN_NAMESPACE_BEGIN
class World;

/**
	@brief	
*/
class RenderingContext
	: public DrawList
{
public:
	RenderView* getRenderView() const { return m_currentRenderView; }

LN_CONSTRUCT_ACCESS:
	RenderingContext();
	virtual ~RenderingContext();
	void initialize();

private:
	void setRenderView(RenderView* renderView) { m_currentRenderView = renderView; }

	RenderView*	m_currentRenderView;

	friend class World;
};

LN_NAMESPACE_END

