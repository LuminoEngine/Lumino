
#pragma once
#define LN_INTERNAL_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public
#include <LuminoEngine.h>


// 内部利用ということでここでグローバルな using namespace している。
// LFManager.h では これがあること前提でいろいろ定義しているので、
// 通常は LFManager.h だけ include しようとするとコンパイルエラーになる。(これによってとりあえず internal であることを示す)
using namespace ln;

//#include "../include/LNTypedef.h"
//#include "../include/LNCommon.h"
//#include "LNManager.h"

//------------------------------------------------------------------------------
#define LWIG_TO_OBJECT(type, h)		static_cast<type*>((h != NULL) ? LFManager::GetObjectEntry(h)->Object : NULL)
#define LWIG_TO_INDEX(h)			static_cast<int>( h )
#define LWIG_TO_HANDLE(obj)			LFManager::CheckRegisterObject(obj)

#define LWIG_CREATE_OBJECT(out, type, initFunc, ...)	{ auto ptr = ln::NewObject<type>(__VA_ARGS__); ptr->PostInitialize(); *out = LFManager::CheckRegisterObject(ptr); }

// bool → LNBool
#define LNOTE_BOOL_TO_LNBOOL( x )	( x ) ? LN_TRUE : LN_FALSE
#define LNC_TO_LNBOOL( x )	( x ) ? LN_TRUE : LN_FALSE
#define LNC_TO_BOOL( x )	(x != LN_FALSE)

// ポインタの int キャスト
#define LN_TO_INT( ptr )			reinterpret_cast<intptr_t>( ptr )
#define LN_INT_TO( type, value )	reinterpret_cast<type>( value )

template<typename T, typename U>
T* p_cast(U* p) { return reinterpret_cast<T*>(p); }

template<typename T, typename U>
const T* cp_cast(const U* p) { return reinterpret_cast<const T*>(p); }

//---------------------------------------------------------------------
/// try 空間の開始
#define LWIG_FUNC_TRY_BEGIN	try{

/// try 空間の終了
#define LWIG_FUNC_TRY_END_RETURN		} \
    catch (ln::Exception& e) { \
		return (LNResultCode)LFManager::ProcException(&e); \
	} \
	return ::LN_RESULTCODE_OK;

#include "BindingManager.h"

//---------------------------------------------------------------------
// check
#define LWIG_CHECK_ARG_NOT_ZERO(x)	if ((x) == 0) return LN_RESULTCODE_ERRORARGUMENT;

