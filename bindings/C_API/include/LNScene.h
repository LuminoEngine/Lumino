
#pragma once

#include "LNCommon.h"
#include "LNTypedef.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
	@defgroup   group_scene シーングラフ
	@brief		シーングラフ
	@{
*/
LN_MODULE(Scene)

//==============================================================================
/**
	@brief	シーングラフを構成するノードのベースクラスです。
*/
LN_CLASS(LNSceneNode, LNObject)

	/**
		@brief		ノードの位置を設定します。
		@param[in]	sceneNode	: シーンノードハンドル
		@param[in]	position	: 座標
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSceneNode_SetPosition(LN_HANDLE(LNSceneNode) sceneNode, const LNVector3* position);
	
LN_CLASS_END

//==============================================================================
/**
	@brief	スプライト
*/
LN_CLASS(LNSprite, LNSceneNode)
	
		
	/**
		@brief		スプライトにテクスチャを設定します。
		@param[in]	sprite		: スプライトハンドル
		@param[in]	texture		: テクスチャハンドル
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSprite_SetTexture(LN_HANDLE(LNSprite) sprite, LN_HANDLE(LNTexture) texture);
		
	/**
		@brief		スプライトに設定されているテクスチャを取得します。
		@param[in]	sprite		: スプライトハンドル
		@param[out]	outTexture	: テクスチャハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSprite_GetTexture(LN_HANDLE(LNSprite) sprite, LN_OUT LN_HANDLE(LNTexture)* outTexture);
	
LN_CLASS_END


//==============================================================================
/**
	@brief	2Dスプライト
*/
LN_CLASS(LNSprite2D, LNSprite)

	/**
		@brief		2Dスプライトオブジェクトを作成します。
		@param[in]	texture		: 2Dスプライトが表示するテクスチャのハンドル
		@param[out]	outSprite2D	: 作成された2Dスプライトハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNSprite2D_Create(LN_HANDLE(LNTexture) texture, LN_OUT LN_HANDLE(LNSprite2D)* outSprite2D);
	
LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
