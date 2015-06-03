//=============================================================================
// LFBase 
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

#pragma once

#include "LFInternal.h"
#include "LFBase.h"

//=============================================================================
// LFObject
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
LN_INTERNAL_API void* LNObject_GetBindingTypeID(LNHandle hadnleObject)
{
	return LFManager::GetObjectEntry(hadnleObject)->Object->GetBindingTypeID();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_INTERNAL_API void* LNObject_GetInternalObject(LNHandle hadnleObject)
{
	return LFManager::GetObjectEntry(hadnleObject);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_INTERNAL_API void LNObject_SetUserData(LNHandle hadnleObject, void* data)
{
	LFManager::GetObjectEntry(hadnleObject)->UserData = data;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LN_INTERNAL_API void* LNObject_GetUserData(LNHandle hadnleObject)
{
	return LFManager::GetObjectEntry(hadnleObject)->UserData;
}
