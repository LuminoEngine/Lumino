//==============================================================================
// LNScene 
//==============================================================================

#include "LNInternal.h"
#include "../../../src/Scene/SceneGraphManager.h"
#include "../include/LNScene.h"

//==============================================================================
// LNSceneNode
//==============================================================================
LN_TYPE_INFO_IMPL(SceneNode, LNSceneNode);

//------------------------------------------------------------------------------
LNResult LNSceneNode_SetPosition(LN_HANDLE(LNSceneNode) sceneNode, const LNVector3* position)
{
	LN_CHECK_ARG_HANDLE(sceneNode);
	LN_CHECK_ARG(position != nullptr);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(SceneNode, sceneNode)->SetPosition(*cp_cast<Vector3>(position));
	LN_FUNC_TRY_END_RETURN;
}


//==============================================================================
// LNSprite
//==============================================================================
LN_TYPE_INFO_IMPL(Sprite, LNSprite);

//------------------------------------------------------------------------------
LNResult LNSprite_SetTexture(LN_HANDLE(LNSprite) sprite, LN_HANDLE(LNTexture) texture)
{
	LN_CHECK_ARG_HANDLE(sprite);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sprite, sprite)->SetTexture(TO_REFOBJ(Texture, texture));
	LN_FUNC_TRY_END_RETURN;
}


		
	/**
		@brief		スプライトに設定されているテクスチャを取得します。
		@param[in]	sprite		: スプライトハンドル
		@param[out]	outTexture	: テクスチャハンドルを格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_PROPERTY
	LNResult LNSprite_GetTexture(LN_HANDLE(LNSprite) sprite, LN_OUT LN_HANDLE(LNTexture)* outTexture);
//------------------------------------------------------------------------------
LNResult LNSprite_GetTexture(LN_HANDLE(LNSprite) sprite, LN_OUT LN_HANDLE(LNTexture)* outTexture)
{
	LN_CHECK_ARG_HANDLE(sprite);
	LN_CHECK_ARG(outTexture != nullptr);
	LN_FUNC_TRY_BEGIN;
	Texture* tex = TO_REFOBJ(Sprite, sprite)->GetTexture();
	*outTexture = TO_HANDLE(tex);
	LN_FUNC_TRY_END_RETURN;
}

//==============================================================================
// LNSprite2D
//==============================================================================
LN_TYPE_INFO_IMPL(Sprite2D, LNSprite2D);

//------------------------------------------------------------------------------
LNResult LNSprite2D_Create(LN_HANDLE(LNTexture) texture, LN_OUT LN_HANDLE(LNSprite2D)* outSprite2D)
{
	LN_FUNC_TRY_BEGIN;
	auto ptr = RefPtr<LNWISprite2D>::MakeRef();
	ptr->Initialize(LFManager::Engine->GetSceneGraphManager()->GetDefault2DSceneGraph());
	ptr->SetTexture(TO_REFOBJ(Texture, texture));
	*outSprite2D = LFManager::CheckRegisterObject(ptr);
	ptr.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}
