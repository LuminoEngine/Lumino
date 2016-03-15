//=============================================================================
// LNBase 
//-----------------------------------------------------------------------------
/*
	■HSPでの例外(エラー)処理
		①[HSP] .hs で定義している LNException_SetHandler でラベルを登録する。
		        登録するWinメッセージは WIN32_MESSAGE_FOR_HSP_EXCEPTION
		②[C++] LN_FUNC_TRY_BEGIN～END で囲まれた領域内で例外が発生すると、
		        _LNException_SendToWin32Message() が呼ばれる (内部関数)
		③[C++] _LNException_SendToWin32Message() 内で WIN32_MESSAGE_FOR_HSP_EXCEPTION を send
		④[HSP] 登録したラベルがコールされる
		⑤[HSP] デフォルトでは LNException_ProcDefault() → end とする。
				こうしないと、メッセージボックス表示後にアプリが終了しない。

	■[2013/12/08] 公開する関数に try を張ることについて
		try を張った関数のオーバーヘッドは以下の通り。
			VC++	… 関数の入り口で必ず発生
			GCC		… catch したときのみ発生

		VC++ では try を書かないこと以外回避できないけど、
		それは現時点でのライブラリの方針的にNG。
		関数ライブラリとしても、正常系部分に異常系をガリガリ書かせたくない。

		そうなると try の数を減らす必要があるわけだけど、
		実際に例外を throw するのはほとんどが Create 系で、
		フレーム更新ごとに毎回呼ばれるものはほぼ投げない。setPosition() 等。

		というところで、基本的には以下のようなルールで実装する。
			・Create 等必要なタイミングでのみ呼ばれるものは try を張る
			・フレーム更新ごとに呼ばれるものは基本的に try を張らない。
			  NULL チェックはすべて assert で処理する。
			  (NULL を渡すことは、関数の使い方が間違っているもの)
*/
//=============================================================================

#include "LNInternal.h"
#include "../include/LNBase.h"

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


//=============================================================================
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





//=============================================================================
// LNError
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNError_GetLastErrorCode()
{
	return LFManager::LastErrorCode;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LNError_GetLastErrorMessage(const LNChar** outStr)
{
	if (LFManager::LastException != NULL) {
		*outStr =  LFManager::LastException->GetMessage();
	}
	else {
		*outStr = NULL;
	}
}

//=============================================================================
// LNObject
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNObject_Release(LNHandle hadnleObject)
{
	LN_CHECK_ARG_HANDLE(hadnleObject);
	LFManager::ReleaseObject(hadnleObject);
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNObject_AddRef(LNHandle hadnleObject)
{
	LN_CHECK_ARG_HANDLE(hadnleObject);
	LFManager::AddRefObject(hadnleObject);
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNObject_GetRefCount(LNHandle hadnleObject, int* count)
{
	LN_CHECK_ARG_HANDLE(hadnleObject);
	LN_CHECK_ARG(count != NULL);
	*count = LFManager::GetObjectEntry(hadnleObject)->RefCount;
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_INTERNAL_API LNUserData LNObject_GetBindingTypeData(LNHandle hadnleObject)
{
	auto* obj = LFManager::GetObjectEntry(hadnleObject)->Object;
	return tr::TypeInfo::GetBindingTypeInfo(obj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_INTERNAL_API LNUserData LNObject_GetInternalObject(LNHandle hadnleObject)
{
	return LFManager::GetObjectEntry(hadnleObject);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_INTERNAL_API void LNObject_SetUserData(LNHandle hadnleObject, LNUserData data)
{
	LFManager::GetObjectEntry(hadnleObject)->UserData = data;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_INTERNAL_API LNUserData LNObject_GetUserData(LNHandle hadnleObject)
{
	return LFManager::GetObjectEntry(hadnleObject)->UserData;
}

