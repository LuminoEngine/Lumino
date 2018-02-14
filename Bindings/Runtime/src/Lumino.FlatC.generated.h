
#pragma once

#if defined(LUMINO_BUILD_DLL) && defined(LN_MSVC)
	#define LN_API	__declspec(dllexport)
#else
	#define LN_API
#endif

#ifdef LN_UNICODE
	typedef wchar_t		LNChar;
	#define __T(x)      L ## x
#else
	typedef char		LNChar;
	#define __T(x)      x
#endif

#define LN_OK	LN_RESULTCODE_OK

typedef intptr_t LNUserData;

struct LNVector3
{
    float x;
    float y;
    float z;
};






/**
    @brief ベクトルの長さを返します。
*/
LN_API LNResultCode LNVector3_GetLength(const LNVector3* vector3, float* outReturn);

/**
    @brief ベクトルの長さの 2 乗を返します。
*/
LN_API LNResultCode LNVector3_GetLengthSquared(const LNVector3* vector3, float* outReturn);

/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] x : 処理の基になるベクトルの X 要
    @param[in] y : 処理の基になるベクトルの Y 要
    @param[in] z : 処理の基になるベクトルの Z 要
    @return 正規化されたベクトル
*/
LN_API LNResultCode LNVector3_Normalize(LNVector3* outReturn);

/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] vec : 処理の基になるベクトル
    @return 正規化されたベクトル
*/
LN_API LNResultCode LNVector3_Normalize(LNVector3* outReturn);



