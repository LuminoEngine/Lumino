
#include "LNInternal.h"
#include "../include/LNBase.h"



//==============================================================================
/**
	@brief	TODO
*/
LN_STRUCT_CLASS(LNVariant)

	/**
		@brief		TODO
		@param[in]	value		: 
		@param[in]	structSize	: 
	*/
	LN_INSTANCE_API
	LNResult LNVariant_Init(LNVariant* value, int structSize);

	/**
		@brief		TODO
		@param[in]	value		: 
	*/
	LN_INSTANCE_API
	LNResult LNVariant_Clear(LNVariant* value);

	/**
		@brief		TODO
		@param[in]	value		: 
		@param[in]	obj			: 
	*/
	LN_INSTANCE_API
	LNResult LNVariant_SetObject(LNVariant* value, LN_HANDLE(LNObject) obj);





//==============================================================================
// LNError
//==============================================================================

//------------------------------------------------------------------------------
LNResult LNError_GetLastErrorCode()
{
	return LFManager::LastErrorCode;
}

//------------------------------------------------------------------------------
void LNError_GetLastErrorMessage(const LNChar** outStr)
{
	if (LFManager::LastException != NULL) {
		*outStr =  LFManager::LastException->GetMessage();
	}
	else {
		*outStr = NULL;
	}
}

//==============================================================================
// LNObject
//==============================================================================

//------------------------------------------------------------------------------
LNResult LNObject_Release(LNHandle hadnleObject)
{
	LN_CHECK_ARG_HANDLE(hadnleObject);
	LFManager::ReleaseObject(hadnleObject);
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LNResult LNObject_AddRef(LNHandle hadnleObject)
{
	LN_CHECK_ARG_HANDLE(hadnleObject);
	LFManager::AddRefObject(hadnleObject);
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LNResult LNObject_GetRefCount(LNHandle hadnleObject, int* count)
{
	LN_CHECK_ARG_HANDLE(hadnleObject);
	LN_CHECK_ARG_RETURN(count != NULL);
	*count = LFManager::GetObjectEntry(hadnleObject)->RefCount;
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LN_INTERNAL_API LNUserData LNObject_GetBindingTypeData(LNHandle hadnleObject)
{
	auto* obj = LFManager::GetObjectEntry(hadnleObject)->Object;
	return tr::TypeInfo::GetBindingTypeInfo(obj);
}

//------------------------------------------------------------------------------
LN_INTERNAL_API LNUserData LNObject_GetInternalObject(LNHandle hadnleObject)
{
	return LFManager::GetObjectEntry(hadnleObject);
}

//------------------------------------------------------------------------------
LN_INTERNAL_API void LNObject_SetUserData(LNHandle hadnleObject, LNUserData data)
{
	LFManager::GetObjectEntry(hadnleObject)->UserData = data;
}

//------------------------------------------------------------------------------
LN_INTERNAL_API LNUserData LNObject_GetUserData(LNHandle hadnleObject)
{
	return LFManager::GetObjectEntry(hadnleObject)->UserData;
}


//==============================================================================
// LNObject
//==============================================================================

//------------------------------------------------------------------------------
LNResult LNListObject_GetCount(LN_HANDLE(LNListObject) listObject, int* outCount)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(outCount != nullptr);
	*outCount = TO_REFOBJ(ListObject, listObject)->GetCountInternal();
	return LN_OK;
}

//------------------------------------------------------------------------------
LNResult LNListObject_SetAt(LN_HANDLE(LNListObject) listObject, int index, void* itemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->SetAtByVoidPtr(index, itemPtr);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNListObject_GetAt(LN_HANDLE(LNListObject) listObject, int index, LN_OUT void** outItemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(outItemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	*outItemPtr = TO_REFOBJ(ListObject, listObject)->GetAtByVoidPtr(index);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNListObject_Add(LN_HANDLE(LNListObject) listObject, void* itemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->AddByVoidPtr(itemPtr);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNListObject_Clear(LN_HANDLE(LNListObject) listObject)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->ClearInternal();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNListObject_Insert(LN_HANDLE(LNListObject) listObject, int index, void* itemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->InsertByVoidPtr(index, itemPtr);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNListObject_Remove(LN_HANDLE(LNListObject) listObject, void* itemPtr, LN_OUT LNBool* outRemoved)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	bool removed = TO_REFOBJ(ListObject, listObject)->RemoveByVoidPtr(itemPtr);
	if (outRemoved != nullptr) *outRemoved = LNC_TO_LNBOOL(removed);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNListObject_RemoveAt(LN_HANDLE(LNListObject) listObject, int index)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->RemoveAtInternal(index);
	LN_FUNC_TRY_END_RETURN;
}
