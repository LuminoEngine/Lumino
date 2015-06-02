
#include "LFInternal.h"
#include "LFApplication.h"

//=============================================================================
// LFApplication
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LFApplication_Initialize()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::PreInitialize();
	LFManager::Application->Initialize(LFManager::ConfigData);
	LFManager::PostInitialize();
	LN_FUNC_TRY_END_RETURN;
}

