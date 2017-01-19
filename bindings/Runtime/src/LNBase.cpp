
#include "Internal.h"
#include "LuminoC.generated.h"
#include "WrapperIF.generated.h"

extern "C" {

//==============================================================================
// LNError
//==============================================================================

//------------------------------------------------------------------------------
LN_API LNResultCode LNError_GetLastErrorCode()
{
	return (LNResultCode)LFManager::LastErrorCode;
}

//------------------------------------------------------------------------------
// outStr : 文字列の先頭ポインタを受け取る変数のポインタ
LN_API void LNError_GetLastErrorMessage(const LNChar** outStr)
{
	if (LFManager::LastException != nullptr)
		*outStr = LFManager::LastException->GetMessage();
	else
		*outStr = nullptr;
}

//==============================================================================
// LNDiag
//==============================================================================

//------------------------------------------------------------------------------
LN_API LNResultCode LNDiag_GetHandleCount(int* outCount)
{
	LWIG_CHECK_ARG_NOT_ZERO(outCount);
	*outCount = LFManager::GetHandleCount();
	return LN_OK;
}

//==============================================================================
// LNObject
//==============================================================================

//------------------------------------------------------------------------------
LN_API LNResultCode LNObject_Release(LNHandle hadnleObject)
{
	LWIG_CHECK_ARG_NOT_ZERO(hadnleObject);
	LFManager::ReleaseObject(hadnleObject);
	return LN_OK;
}

//------------------------------------------------------------------------------
LN_API LNResultCode LNObject_AddRef(LNHandle hadnleObject)
{
	LWIG_CHECK_ARG_NOT_ZERO(hadnleObject);
	LFManager::AddRefObject(hadnleObject);
	return LN_OK;
}

//------------------------------------------------------------------------------
LN_API LNResultCode LNObject_GetRefCount(LNHandle hadnleObject, int* outCount)
{
	LWIG_CHECK_ARG_NOT_ZERO(hadnleObject);
	LWIG_CHECK_ARG_NOT_ZERO(outCount);
	*outCount = LFManager::GetObjectEntry(hadnleObject)->RefCount;
	return LN_OK;
}

//------------------------------------------------------------------------------
LN_API LNUserData LNObject_GetBindingTypeData(LNHandle hadnleObject)
{
	auto* obj = LFManager::GetObjectEntry(hadnleObject)->Object;
	return reinterpret_cast<LNUserData>(tr::TypeInfo::GetBindingTypeInfo(obj));
}

//------------------------------------------------------------------------------
//LN_API LNUserData LNObject_GetInternalObject(LNHandle hadnleObject)
//{
//	return LFManager::GetObjectEntry(hadnleObject);
//}

//------------------------------------------------------------------------------
LN_API void LNObject_SetUserData(LNHandle hadnleObject, LNUserData data)
{
	LFManager::GetObjectEntry(hadnleObject)->UserData = data;
}

//------------------------------------------------------------------------------
LN_API LNUserData LNObject_GetUserData(LNHandle hadnleObject)
{
	return LFManager::GetObjectEntry(hadnleObject)->UserData;
}


//==============================================================================
// LFInternal
//==============================================================================

//------------------------------------------------------------------------------
LN_API void LCSInternal_GetIntPtrStringLength(intptr_t string, int* len)
{
	if (string == 0)
	{
		*len = 0;
		return;
	}
	*len = _tcslen((const LNChar*)string);
}

//------------------------------------------------------------------------------
LN_API void LCSInternal_GetIntPtrString(intptr_t string, LNChar* buf)
{
	if (string == 0 || buf == nullptr) return;

	int len;
	LCSInternal_GetIntPtrStringLength(string, &len);
	memcpy(buf, (const LNChar*)string, len * sizeof(LNChar));
	buf[len] = '\0';
}

//------------------------------------------------------------------------------
LN_API void LHSPInternal_GetIntPtrStringLength(intptr_t string, int* len)
{
	if (string == 0) {
		*len = 0;
		return;
	}
	*len = _tcslen((const LNChar*)string);
}

//------------------------------------------------------------------------------
LN_API void LHSPInternal_GetIntPtrString(intptr_t string, LNChar* buf)
{
	if (string == 0 || buf == nullptr)
		return;

	int len;
	LHSPInternal_GetIntPtrStringLength(string, &len);
	memcpy(buf, (const LNChar*)string, len * sizeof(LNChar));
}


//------------------------------------------------------------------------------
LN_API const char* LNInternal_ConvertToUTF8String(const LNChar* str, int len)
{
	len = (len < 0) ? _tcslen(str) : len;
	const ByteBuffer& buf = LFManager::TCharToUTF8Converter->Convert(str, len * sizeof(LNChar));
	return (const char*)buf.GetConstData();
}


} // extern "C"

