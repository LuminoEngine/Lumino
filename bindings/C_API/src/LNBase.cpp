
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
// LNDiag
//==============================================================================

//------------------------------------------------------------------------------
LNResult LNDiag_GetHandleCount(int* outCount)
{
	LN_CHECK_ARG_RETURN(outCount != NULL);
	*outCount = LFManager::GetHandleCount();
	return LN_OK;
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
// LNValueList
//==============================================================================
#if 0
//------------------------------------------------------------------------------
LNResult LNValueList_GetCount(LN_HANDLE(LNValueList) listObject, int* outCount)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(outCount != nullptr);
	*outCount = TO_REFOBJ(ListObject, listObject)->GetCountInternal();
	return LN_OK;
}

//------------------------------------------------------------------------------
LNResult LNValueList_SetAt(LN_HANDLE(LNValueList) listObject, int index, void* itemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->SetAtByVoidPtr(index, itemPtr);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNValueList_GetAt(LN_HANDLE(LNValueList) listObject, int index, LN_OUT void** outItemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(outItemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	*outItemPtr = TO_REFOBJ(ListObject, listObject)->GetAtByVoidPtr(index);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNValueList_Add(LN_HANDLE(LNValueList) listObject, void* itemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->AddByVoidPtr(itemPtr);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNValueList_Clear(LN_HANDLE(LNValueList) listObject)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->ClearInternal();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNValueList_Insert(LN_HANDLE(LNValueList) listObject, int index, void* itemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->InsertByVoidPtr(index, itemPtr);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNValueList_Remove(LN_HANDLE(LNValueList) listObject, void* itemPtr, LN_OUT LNBool* outRemoved)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	bool removed = TO_REFOBJ(ListObject, listObject)->RemoveByVoidPtr(itemPtr);
	if (outRemoved != nullptr) *outRemoved = LNC_TO_LNBOOL(removed);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNValueList_RemoveAt(LN_HANDLE(LNValueList) listObject, int index)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->RemoveAtInternal(index);
	LN_FUNC_TRY_END_RETURN;
}
#endif

//==============================================================================
// LNObjectList
//==============================================================================
LN_TYPE_INFO_IMPL(ln::ListObject, LNObjectList);

//------------------------------------------------------------------------------
LNResult LNObjectList_GetCount(LN_HANDLE(LNObjectList) listObject, int* outCount)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(outCount != nullptr);
	*outCount = TO_REFOBJ(ListObject, listObject)->GetCount_Object();
	return LN_OK;
}

//------------------------------------------------------------------------------
LNResult LNObjectList_SetAt(LN_HANDLE(LNObjectList) listObject, int index, LN_T1_HANDLE itemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != 0);
	LN_FUNC_TRY_BEGIN;
	Object* ptr = TO_REFOBJ(Object, itemPtr);
	TO_REFOBJ(ListObject, listObject)->SetAt_Object(index, ptr);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNObjectList_GetAt(LN_HANDLE(LNObjectList) listObject, int index, LN_OUT LN_T1_HANDLE* outItemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(outItemPtr != nullptr);
	LN_FUNC_TRY_BEGIN;
	Object* ptr = (Object*)TO_REFOBJ(ListObject, listObject)->GetAt_Object(index);
	*outItemPtr = TO_HANDLE_ADDREF(ptr);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNObjectList_Add(LN_HANDLE(LNObjectList) listObject, LN_T1_HANDLE itemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != 0);
	LN_FUNC_TRY_BEGIN;
	Object* ptr = TO_REFOBJ(Object, itemPtr);
	TO_REFOBJ(ListObject, listObject)->Add_Object(ptr);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNObjectList_Clear(LN_HANDLE(LNObjectList) listObject)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->Clear_Object();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNObjectList_Insert(LN_HANDLE(LNObjectList) listObject, int index, LN_T1_HANDLE itemPtr)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != 0);
	LN_FUNC_TRY_BEGIN;
	Object* ptr = TO_REFOBJ(Object, itemPtr);
	TO_REFOBJ(ListObject, listObject)->Insert_Object(index, ptr);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNObjectList_Remove(LN_HANDLE(LNObjectList) listObject, LN_T1_HANDLE itemPtr, LN_OUT LNBool* outRemoved)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_CHECK_ARG(itemPtr != 0);
	LN_FUNC_TRY_BEGIN;
	Object* ptr = TO_REFOBJ(Object, itemPtr);
	bool removed = TO_REFOBJ(ListObject, listObject)->Remove_Object(ptr);
	if (outRemoved != nullptr) *outRemoved = LNC_TO_LNBOOL(removed);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNObjectList_RemoveAt(LN_HANDLE(LNObjectList) listObject, int index)
{
	LN_CHECK_ARG_HANDLE(listObject);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(ListObject, listObject)->RemoveAt_Object(index);
	LN_FUNC_TRY_END_RETURN;
}
