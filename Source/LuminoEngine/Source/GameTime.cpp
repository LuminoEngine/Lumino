
#include "Internal.h"
#include "EngineManager.h"
#include <Lumino/GameTime.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// GameTime
//==============================================================================

//------------------------------------------------------------------------------
double GameTime::getElapsedGameTime()
{
	return detail::EngineDomain::getEngineManager()->getFpsController().getElapsedGameTime();
}

//------------------------------------------------------------------------------
double GameTime::getElapsedRealTime()
{
	return detail::EngineDomain::getEngineManager()->getFpsController().getElapsedRealTime();
}

//------------------------------------------------------------------------------
double GameTime::getTotalGameTime()
{
	return detail::EngineDomain::getEngineManager()->getFpsController().getTotalGameTime();
}

//------------------------------------------------------------------------------
double GameTime::getTotalRealTime()
{
	return detail::EngineDomain::getEngineManager()->getFpsController().getTotalRealTime();
}

LN_NAMESPACE_END
