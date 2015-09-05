
#include "LNInternal.h"
#include "LNApplication.h"

extern "C" {

//=============================================================================
// LNApplication
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNApplication_Initialize()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::PreInitialize();
	LFManager::PostInitialize();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNApplication_Update()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Application->UpdateFrame();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNApplication_ResetFrameDelay()
{
	LFManager::Application->ResetFrameDelay();
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNApplication_IsEndRequested(LNBool* requested)
{
	*requested = LNOTE_BOOL_TO_LNBOOL(LFManager::Application->IsEndRequested());
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNApplication_Terminate()
{
	LFManager::Finalize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void* LNApplication_GetInternalObject()
//{
//	return LFManager::CoreManager;
//}

} // extern "C"
