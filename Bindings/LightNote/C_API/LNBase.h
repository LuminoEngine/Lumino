
#pragma once

#include "LNCommon.h"
#include "LNTypedef.h"

extern "C" {

/**
	@brief	
*/
LN_MODULE(Base)

//=============================================================================
/**
	@brief	全てのオブジェクトのベースオブジェクトです。
*/
LN_EXTENSION_CLASS(LNObject)

	/**
		@brief		オブジェクトを解放します。
		@param[in]	hadnleObject	: オブジェクトハンドル
		@details	指定されたオブジェクトの参照を解放します。
	*/
	LN_INSTANCE_API
	LNResult LNObject_Release(LN_HANDLE(LNObject) hadnleObject);

	/**
		@brief		オブジェクトの参照カウントをインクリメントします。
		@param[in]	hadnleObject	: オブジェクトハンドル
		@details	この関数は「参照カウント」という概念について十分に理解した上で使用してください。
					参照カウントは LNObject_AddRef でインクリメントされ、LNObject_Release でデクリメントされます。
					Release 漏れがあるとメモリリークの原因になります。
	*/
	LN_INSTANCE_API
	LNResult LNObject_AddRef(LN_HANDLE(LNObject) hadnleObject);

	/**
		@brief		オブジェクトの参照カウント取得します。
		@param[in]	hadnleObject	: オブジェクトハンドル
		@param[out]	count			: 参照カウントを格納する変数
	*/
	LN_INSTANCE_API
	LNResult LNObject_GetRefCount(LN_HANDLE(LNObject) hadnleObject, int* count);

	LN_INTERNAL_API LNUserData	LNObject_GetBindingTypeData(LN_HANDLE(LNObject) hadnleObject);
	LN_INTERNAL_API LNUserData	LNObject_GetInternalObject(LN_HANDLE(LNObject) hadnleObject);
	LN_INTERNAL_API void		LNObject_SetUserData(LN_HANDLE(LNObject) hadnleObject, LNUserData data);
	LN_INTERNAL_API LNUserData	LNObject_GetUserData(LN_HANDLE(LNObject) hadnleObject);
	
LN_CLASS_END

LN_MODULE_END

} // extern "C"

