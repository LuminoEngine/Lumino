//==============================================================================
// LFInternal 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "LFInternal.h"

extern "C"
{

//==============================================================================
// LFInternal
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI void LCSInternal_GetIntPtrStringLength(int string, int* len)
	{
		if (string == 0) {
			*len = 0;
			return;
		}
		*len = strlen((const char*)string);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI void LCSInternal_GetIntPtrString(int string, char* buf)
	{
		if (string == 0 || buf == NULL)
			return;

		int len;
		LCSInternal_GetIntPtrStringLength(string, &len);
		memcpy(buf, (const char*)string, len);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI void LHSPInternal_GetIntPtrStringLength(int string, int* len)
	{
		if (string == 0) {
			*len = 0;
			return;
		}
		*len = strlen((const char*)string);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI void LHSPInternal_GetIntPtrString(int string, char* buf)
	{
		if (string == 0 || buf == NULL)
			return;

		int len;
		LHSPInternal_GetIntPtrStringLength(string, &len);
		memcpy(buf, (const char*)string, len);
	}

	

} // extern "C"
