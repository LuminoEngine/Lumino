
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
		@brief		ノードの可視状態を設定します。(default: true)
		@param[in]	sceneNode	: ノードハンドル
		@param[in]	visible		: 可視状態
		@details	false の場合、ノードの描画は行われません。
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSceneNode_SetVisible(LN_HANDLE(LNSceneNode) sceneNode, LNBool visible);

	/**
		@brief		ノードの可視状態を取得します。
		@param[in]	sceneNode	: ノードハンドル
		@param[out]	outVisible	: 可視状態を格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSceneNode_IsVisible(LN_HANDLE(LNSceneNode) sceneNode, LN_OUT LNBool* outVisible);

	/**
		@brief		ノードの位置を設定します。(default: 0,0,0)
		@param[in]	sceneNode	: ノードハンドル
		@param[in]	position	: 座標
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSceneNode_SetPosition(LN_HANDLE(LNSceneNode) sceneNode, const LNVector3* position);
	
	/**
		@brief		ノードの位置を取得します。
		@param[in]	sceneNode	: ノードハンドル
		@param[out]	outPosition	: 座標を格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSceneNode_GetPosition(LN_HANDLE(LNSceneNode) sceneNode, LN_OUT LNVector3* outPosition);

	/**
		@brief		ノードの子要素のリストを取得します。
		@param[in]	sceneNode	: ノードハンドル
		@param[out]	outList		: SceneNodeList オブジェクトのハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSceneNode_GetChildren(LN_HANDLE(LNSceneNode) sceneNode, LN_OUT LN_GENERIC_HANDLE(LNList, LNSceneNode)* outList);
	
LN_CLASS_END

//==============================================================================
/**
	@brief		スプライトのベースクラスです。
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
	@brief		2D 空間に配置されるスプライトのクラスです。
*/
LN_CLASS(LNSprite2D, LNSprite)

	/**
		@brief		2Dスプライトオブジェクトを作成します。
		@param[out]	outSprite2D	: 作成された2Dスプライトハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNSprite2D_Create(LN_OUT LN_HANDLE(LNSprite2D)* outSprite2D);

	/**
		@brief		2Dスプライトオブジェクトを作成します。
		@param[in]	texture		: 2Dスプライトが表示するテクスチャのハンドル
		@param[out]	outSprite2D	: 作成された2Dスプライトハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LN_ATTR_OVERLOAD(LNSprite2D_Create)
	LNResult LNSprite2D_CreateFromTexture(LN_HANDLE(LNTexture) texture, LN_OUT LN_HANDLE(LNSprite2D)* outSprite2D);
	
LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
