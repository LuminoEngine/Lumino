
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
	return EngineManager::getInstance()->getFpsController().getElapsedGameTime();
}

//------------------------------------------------------------------------------
double GameTime::getElapsedRealTime()
{
	return EngineManager::getInstance()->getFpsController().getElapsedRealTime();
}

//------------------------------------------------------------------------------
double GameTime::getTotalGameTime()
{
	return EngineManager::getInstance()->getFpsController().getTotalGameTime();
}

//------------------------------------------------------------------------------
double GameTime::getTotalRealTime()
{
	return EngineManager::getInstance()->getFpsController().getTotalRealTime();
}

LN_NAMESPACE_END
