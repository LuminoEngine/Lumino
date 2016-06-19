
#pragma once

#include "LNCommon.h"
#include "LNTypedef.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
	@defgroup   group_base 基本機能
	@brief		基本機能
	@{
*/
LN_MODULE(Base)

//==============================================================================
/**
	@brief	エラーに関係する処理を行います。
*/
LN_EXTENSION_CLASS(LNError)

	/**
		@brief		最後に発生したエラーのエラーコードを取得します。
		@return		エラーコード
	*/
	LN_STATIC_API
	LNResult LNError_GetLastErrorCode();

	/**
		@brief		最後に発生したエラーのエラーメッセージを取得します。
		@param[out]	outStr	: メッセージ文字列のポインタを格納する変数のポインタ。
	*/
	LN_STATIC_API
	void LNError_GetLastErrorMessage(const LNChar** outStr);
	
LN_CLASS_END

//==============================================================================
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


//==============================================================================
/**
	@brief		要素の集合 (可変長配列) を表すクラスです。
*/
LN_GENERIC_CLASS(LNObjectList, LN_T1_HANDLE)

	/**
		@brief		リストに格納されているオブジェクトの数を取得します。
		@param[in]	listObject	: オブジェクトリストハンドル
		@param[out]	outCount	: 要素の数を格納する変数
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_GetCount(LN_HANDLE(LNObjectList) listObject, int* outCount);

	/**
		@brief		オブジェクトリストの指定したインデックスにオブジェクトを設定します。
		@param[in]	listObject	: オブジェクトリストハンドル
		@param[in]	index		: インデックス(要素番号)
		@param[in]	itemPtr		: 設定するオブジェクト
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_SetAt(LN_HANDLE(LNObjectList) listObject, int index, LN_T1_HANDLE itemPtr);

	/**
		@brief		オブジェクトリストの指定したインデックスのオブジェクトを取得します。
		@param[in]	listObject	: オブジェクトリストハンドル
		@param[in]	index		: インデックス(要素番号)
		@param[out]	outItemPtr	: オブジェクトを格納する変数
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_GetAt(LN_HANDLE(LNObjectList) listObject, int index, LN_OUT LN_T1_HANDLE* outItemPtr);

	/**
		@brief		オブジェクトリストの末尾にオブジェクトを追加します。
		@param[in]	listObject	: オブジェクトリストハンドル
		@param[in]	itemPtr		: 追加するオブジェクト
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_Add(LN_HANDLE(LNObjectList) listObject, LN_T1_HANDLE itemPtr);

	/**
		@brief		オブジェクトリストから全てのオブジェクトを削除します。
		@param[in]	listObject	: オブジェクトリストハンドル
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_Clear(LN_HANDLE(LNObjectList) listObject);

	/**
		@brief		オブジェクトリストの指定したインデックスの位置にオブジェクトを挿入します。
		@param[in]	listObject	: オブジェクトリストハンドル
		@param[in]	index		: item を挿入するインデックス
		@param[in]	itemPtr		: 挿入するオブジェクト
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_Insert(LN_HANDLE(LNObjectList) listObject, int index, LN_T1_HANDLE itemPtr);

	/**
		@brief		オブジェクトリスト内で指定したハンドルと一致する最初のオブジェクトを削除します。
		@param[in]	listObject	: オブジェクトリストハンドル
		@param[in]	itemPtr		: リストから削除するオブジェクト
		@param[out]	outRemoved	: 要素を削除したかどうかを示す値 (削除できた場合は LN_TRUE) を格納する変数のアドレス。
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_Remove(LN_HANDLE(LNObjectList) listObject, LN_T1_HANDLE itemPtr, LN_OUT LNBool* outRemoved);

	/**
		@brief		オブジェクトリストの指定したインデックスにあるオブジェクトを削除します。
		@param[in]	listObject	: オブジェクトリストハンドル
		@param[in]	index		: 削除するオブジェクトのインデックス番号
	*/
	LN_INSTANCE_API
	LNResult LNObjectList_RemoveAt(LN_HANDLE(LNObjectList) listObject, int index);

LN_CLASS_END

//==============================================================================
/**
	@brief	オブジェクトの幅と高さを表します。
*/
LN_STRUCT_CLASS(LNSize)

LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
