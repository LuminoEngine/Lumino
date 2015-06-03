
#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

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
		@details	指定されたオブジェクトの参照を解放します。<br>
					Create～という名前の付く関数でオブジェクトを生成した場合は
					基本的にこの関数でオブジェクトを解放する必要があります。
	*/
	LN_INSTANCE_API
	LNResult LNObject_Release(LN_HANDLE(LNObject) hadnleObject);

	/**
		@brief		オブジェクトの参照カウントをインクリメントします。
		@param[in]	hadnleObject	: オブジェクトハンドル
		@details	この関数は「参照カウント」という概念について十分に理解した上で使用してください。
					参照カウントは Create 時に 1 となり、LNObject_Release でデクリメントされます。
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

	LN_INTERNAL_API void*	LNObject_GetTypeUserData(LN_HANDLE(LNObject) hadnleObject);
	LN_INTERNAL_API void*	LNObject_GetInternalObject(LN_HANDLE(LNObject) hadnleObject);
	LN_INTERNAL_API void	LNObject_SetUserData(LN_HANDLE(LNObject) hadnleObject, void* data);
	LN_INTERNAL_API void*	LNObject_GetUserData(LN_HANDLE(LNObject) hadnleObject);
	
LN_CLASS_END


//=============================================================================
/**
	@brief	オブジェクトの可変長配列です。
*/
LN_EXTENSION_CLASS(LNObjectList)

	/**
		@brief		オブジェクトリストに格納されているオブジェクトの数を取得します。
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[out]	count		: 要素の数を格納する変数
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_GetCount(LN_HANDLE(LNObjectList) objectList, int* count);

	/**
		@brief		オブジェクトリストの指定したインデックスにオブジェクトを設定します。
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	index		: インデックス(要素番号)
		@param[in]	item		: 設定するオブジェクト
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_SetAt(LN_HANDLE(LNObjectList) objectList, int index, LN_HANDLE(LNObject) item);

	/**
		@brief		オブジェクトリストの指定したインデックスのオブジェクトを取得します。
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	index		: インデックス(要素番号)
		@param[out]	outItem		: オブジェクトを格納する変数
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_GetAt(LN_HANDLE(LNObjectList) objectList, int index, LN_HANDLE(LNObject)* outItem);

	/**
		@brief		オブジェクトリストの末尾にオブジェクトを追加します。
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	item		: 追加するオブジェクト
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_Add(LN_HANDLE(LNObjectList) objectList, LN_HANDLE(LNObject) item);

	/**
		@brief		オブジェクトリストから全てのオブジェクトを削除します。
		@param[in]	objectList	: オブジェクトリストハンドル
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_Clear(LN_HANDLE(LNObjectList) objectList);

	/**
		@brief		オブジェクトリストの指定したインデックスの位置にオブジェクトを挿入します。
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	index		: item を挿入するインデックス
		@param[in]	item		: 挿入するオブジェクト
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_Insert(LN_HANDLE(LNObjectList) objectList, int index, LN_HANDLE(LNObject) item);

	/**
		@brief		オブジェクトリスト内で指定したハンドルと一致する最初のオブジェクトを削除します。
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	item		: リストから削除するオブジェクト
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_Remove(LN_HANDLE(LNObjectList) objectList, LN_HANDLE(LNObject) item);

	/**
		@brief		オブジェクトリスト内で指定したハンドルと一致する全てのオブジェクトを削除します。
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	item		: リストから削除するオブジェクト
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_RemoveAll(LN_HANDLE(LNObjectList) objectList, LN_HANDLE(LNObject) item);

	/**
		@brief		オブジェクトリストの指定したインデックスにあるオブジェクトを削除します。
		@param[in]	objectList	: オブジェクトリストハンドル
		@param[in]	index		: 削除するオブジェクトのインデックス番号
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_RemoveAt(LN_HANDLE(LNObjectList) objectList, int index);

LN_CLASS_END

LN_MODULE_END

} // extern "C"

