#include "Internal.h"
#include "Lumino.FlatC.generated.h"

extern "C" {

//------------------------------------------------------------------------------
LN_API LNResultCode LNObject_Release(LNHandle hadnleObject)
{
	//LN_CHECK_ARG_HANDLE(hadnleObject);
	LFManager::ReleaseObject(hadnleObject);
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LN_API LNResultCode LNObject_AddRef(LNHandle hadnleObject)
{
	//LN_CHECK_ARG_HANDLE(hadnleObject);
	LFManager::AddRefObject(hadnleObject);
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LN_API LNResultCode LNObject_GetRefCount(LNHandle hadnleObject, int* count)
{
	//LN_CHECK_ARG_HANDLE(hadnleObject);
	//LN_CHECK_ARG_RETURN(count != NULL);
	*count = LFManager::GetObjectEntry(hadnleObject)->RefCount;
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LN_API LNUserData LNObject_GetBindingTypeData(LNHandle hadnleObject)
{
	auto* obj = LFManager::GetObjectEntry(hadnleObject)->Object;
	return reinterpret_cast<LNUserData>(tr::TypeInfo::getBindingTypeInfo(obj));
}

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

} // extern "C"
