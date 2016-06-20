
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

//==============================================================================
/**
	@brief	テクスチャのベースクラスです。
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

//==============================================================================
/**
	@brief	2Dテクスチャのクラスです。
*/
LN_CLASS(LNTexture2D, LNTexture)

	/**
		@brief		サイズを指定して2Dテクスチャを作成します。
		@param[in]	width			: テクスチャの幅 (ピクセル単位)
		@param[in]	height			: テクスチャの高さ (ピクセル単位)
		@param[in]	format			: テクスチャのピクセルフォーマット
		@param[in]	mipmap			: ミップマップの有無
		@param[out]	outTexture2D	: 作成された2Dテクスチャのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNTexture2D_Create(int width, int height, LNTextureFormat format LN_DEFAULT_ARG(LN_FMT_A8R8G8B8), LNBool mipmap LN_DEFAULT_ARG(LN_FALSE), LN_OUT LN_HANDLE(LNTexture2D)* outTexture2D);

	/**
		@brief		ファイルから2Dテクスチャを作成します。
		@param[in]	filePath		: 画像ファイルのパス
		@param[out]	outTexture2D	: 作成された2Dテクスチャのハンドルを格納する変数のポインタ
		@details	全てのプラットフォームでサポートされているファイルフォーマットは .png です。
					グラフィックスAPI に DirectX9 を使用している場合は MSDN の D3DXCreateTextureFromFileEx を参照してください。
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNTexture2D_CreateFromFile(const LNChar* filePath, LN_OUT LN_HANDLE(LNTexture2D)* outTexture2D);

	
LN_CLASS_END

//==============================================================================
/**
	@brief	ビューポートを構成するレイヤーのクラスです。
*/
LN_CLASS(LNViewportLayer, LNObject)
	
LN_CLASS_END

//==============================================================================
/**
	@brief	ビューポートのクラスです。
*/
LN_CLASS(LNViewport, LNObject)
	
	/**
		@brief		ビューポートを構成するレイヤーのリストを取得します。
		@param[in]	viewport	: Viewport オブジェクトのハンドル
		@param[out]	outList		: ViewportLayerList オブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNViewport_GetLayers(LN_HANDLE(LNViewport) viewport, LN_OUT LN_GENERIC_HANDLE(LNObjectList, LNViewportLayer)* outList);

LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
