
#include "../Internal.h"
#include <Lumino/Graphics/DrawingContext.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// DrawingContext
//==============================================================================

//------------------------------------------------------------------------------
DrawingContext::DrawingContext()
{
}

//------------------------------------------------------------------------------
DrawingContext::~DrawingContext()
{
}

//------------------------------------------------------------------------------
void DrawingContext::Initialize()
{
	DrawList::Initialize(detail::EngineDomain::GetGraphicsManager());
}

LN_NAMESPACE_END

