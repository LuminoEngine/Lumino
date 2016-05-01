
#include "Internal.h"
#include "EngineManager.h"
#include <Lumino/GameTime.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// GameTime
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
double GameTime::GetElapsedGameTime()
{
	return EngineManager::GetInstance()->GetFpsController().GetElapsedGameTime();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
double GameTime::GetElapsedRealTime()
{
	return EngineManager::GetInstance()->GetFpsController().GetElapsedRealTime();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
double GameTime::GetTotalGameTime()
{
	return EngineManager::GetInstance()->GetFpsController().GetTotalGameTime();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
double GameTime::GetTotalRealTime()
{
	return EngineManager::GetInstance()->GetFpsController().GetTotalRealTime();
}

LN_NAMESPACE_END
