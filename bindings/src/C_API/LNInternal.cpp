#pragma once
#include "LNInternal.h"

extern "C"
{

//==============================================================================
// LFInternal
//==============================================================================

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
LN_API void LCSInternal_GetIntPtrStringLength(int string, int* len)
{
	if (string == 0) {
		*len = 0;
		return;
	}
	*len = _tcslen((const LNChar*)string);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
LN_API void LCSInternal_GetIntPtrString(int string, LNChar* buf)
{
	if (string == 0 || buf == NULL)
		return;

	int len;
	LCSInternal_GetIntPtrStringLength(string, &len);
	memcpy(buf, (const LNChar*)string, len * sizeof(LNChar));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
LN_API void LHSPInternal_GetIntPtrStringLength(int string, int* len)
{
	if (string == 0) {
		*len = 0;
		return;
	}
	*len = _tcslen((const LNChar*)string);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
LN_API void LHSPInternal_GetIntPtrString(int string, LNChar* buf)
{
	if (string == 0 || buf == NULL)
		return;

	int len;
	LHSPInternal_GetIntPtrStringLength(string, &len);
	memcpy(buf, (const LNChar*)string, len * sizeof(LNChar));
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
LN_API const char* LNInternal_ConvertToUTF8String(const LNChar* str, int len)
{
	len = (len < 0) ? _tcslen(str) : len;
	const ByteBuffer& buf = LFManager::TCharToUTF8Converter.Convert(str, len * sizeof(LNChar));
	return (const char*)buf.GetConstData();
}

	

} // extern "C"
