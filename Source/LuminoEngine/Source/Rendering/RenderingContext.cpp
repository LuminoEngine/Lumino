
#include "Internal.h"
#include <Lumino/Rendering/RenderingContext.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// RenderingContext
//==============================================================================

//------------------------------------------------------------------------------
RenderingContext::RenderingContext()
	: m_currentRenderView(nullptr)
{
}

//------------------------------------------------------------------------------
RenderingContext::~RenderingContext()
{
}

//------------------------------------------------------------------------------
void RenderingContext::initialize()
{
	DrawList::initialize(detail::EngineDomain::getGraphicsManager());
}

LN_NAMESPACE_END
