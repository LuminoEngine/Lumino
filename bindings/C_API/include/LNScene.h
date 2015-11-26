
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

//=============================================================================
/**
	@brief	シーングラフを構成するノードのベースクラスです。
*/
LN_CLASS(LNSceneNode, LNObject)

	/**
		@brief		サウンドの 3D 音源としての位置を設定します。
		@param[in]	sceneNode	: シーンノードハンドル
		@param[in]	position	: 座標
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSceneNode_SetPosition(LN_HANDLE(LNSceneNode) sceneNode, const LNVector3* position);
	
LN_CLASS_END

//=============================================================================
/**
	@brief	スプライト
*/
LN_CLASS(LNSprite, LNSceneNode)

	/**
		@brief		スプライトオブジェクトを作成します。
		@param[in]	texture		: スプライトが表示するテクスチャのハンドル
		@param[out]	outSprite	: 作成されたスプライトオブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNSprite_Create(LN_HANDLE(LNTexture) texture, LN_OUT LN_HANDLE(LNSprite)* outSprite);
	
LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
