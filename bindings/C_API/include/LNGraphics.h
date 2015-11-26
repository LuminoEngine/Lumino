
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
	@brief	テクスチャを操作するためのベースクラスです。
*/
LN_CLASS(LNTexture, LNObject)

	/**
		@brief		テクスチャのサイズを取得します。
		@param[in]	texture		: テクスチャハンドル
		@param[out]	outSize		: サイズを格納する変数のアドレス
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNTexture_GetSize(LN_HANDLE(LNTexture) texture, LN_OUT LNSize* outSize);
	
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
	LNResult LNTexture2D_Create(const LNChar* filePath, LN_OUT LN_HANDLE(LNTexture2D)* texture2D);

	
LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
