
#pragma once
#include "LNCommon.h"
#include "LNTypedef.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
	@defgroup   group_graphics グラフィックス機能
	@brief		グラフィックス機能
	@{
*/
LN_MODULE(Graphics)

//=============================================================================
/**
	@brief	
*/
LN_CLASS(LNTexture, LNObject)

	
	
LN_CLASS_END

//=============================================================================
/**
	@brief	2Dテクスチャを操作するためのクラスです。
*/
LN_CLASS(LNTexture2D, LNTexture)

	/**
		@brief		ファイルから2Dテクスチャオブジェクトを作成します。
		@param[in]	filePath	: 画像ファイルのパス
		@param[out]	texture2D	: 作成された2Dテクスチャオブジェクトのハンドルを格納する変数のポインタ
		@details	全てのプラットフォームでサポートされているファイルフォーマットは .png です。
					グラフィックスAPI に DirectX9 を使用している場合は MSDN の D3DXCreateTextureFromFileEx を参照してください。
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNTexture2D_Create(const LNChar* filePath, LN_OUT LN_HANDLE(Texture2D)* texture2D);

	/**
		@brief		ファイルから2Dテクスチャオブジェクトを作成します。
		@param[in]	filePath	: 画像ファイルのパス
		@param[in]	format		: 作成するテクスチャのフォーマット
		@param[in]	mipmap		: LN_TRUE の場合、ミップマップを作成する
		@param[out]	texture2D	: 作成された2Dテクスチャオブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNTexture2D_CreateEx(const LNChar* filePath, LNTextureFormat foramt, LNBool mipmap, LN_OUT LN_HANDLE(Texture2D)* texture2D);
	
LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
