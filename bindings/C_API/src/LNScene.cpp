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
LNResult LNSceneNode_SetVisible(LN_HANDLE(LNSceneNode) sceneNode, LNBool visible)
{
	LN_CHECK_ARG_HANDLE(sceneNode);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(VisualNode, sceneNode)->SetVisible(LNC_TO_BOOL(visible));
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSceneNode_IsVisible(LN_HANDLE(LNSceneNode) sceneNode, LN_OUT LNBool* outVisible)
{
	LN_CHECK_ARG_HANDLE(sceneNode);
	LN_CHECK_ARG_RETURN(outVisible != nullptr);
	LN_FUNC_TRY_BEGIN;
	*outVisible = LNC_TO_LNBOOL(TO_REFOBJ(VisualNode, sceneNode)->IsVisible());
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSceneNode_SetPosition(LN_HANDLE(LNSceneNode) sceneNode, const LNVector3* position)
{
	LN_CHECK_ARG_HANDLE(sceneNode);
	LN_CHECK_ARG_RETURN(position != nullptr);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(SceneNode, sceneNode)->SetPosition(*cp_cast<Vector3>(position));
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSceneNode_GetPosition(LN_HANDLE(LNSceneNode) sceneNode, LN_OUT LNVector3* outPosition)
{
	LN_CHECK_ARG_HANDLE(sceneNode);
	LN_CHECK_ARG_RETURN(outPosition != nullptr);
	LN_FUNC_TRY_BEGIN;
	*p_cast<Vector3>(outPosition) = TO_REFOBJ(SceneNode, sceneNode)->GetPosition();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSceneNode_GetChildren(LN_HANDLE(LNSceneNode) sceneNode, LN_OUT LN_GENERIC_HANDLE(LNList, LNSceneNode)* outList)
{
	LN_CHECK_ARG_HANDLE(sceneNode);
	LN_CHECK_ARG_RETURN(outList != nullptr);
	LN_FUNC_TRY_BEGIN;
	*outList = TO_HANDLE_ADDREF(TO_REFOBJ(SceneNode, sceneNode)->GetChildren());
	LN_FUNC_TRY_END_RETURN;
}

#if 0
	/**
		@brief		test
		@param[in]	sceneNode	: ノードハンドル
	*/
	LN_INSTANCE_API
	LNResult LNSceneNode_OnRender(LN_HANDLE(LNSceneNode) sceneNode);

//------------------------------------------------------------------------------
LNResult LNSceneNode_OnRender(LN_HANDLE(LNSceneNode) sceneNode)
{
	LN_CHECK_ARG_HANDLE(sceneNode);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(VisualNode, sceneNode)->VisualNode::OnRender(nullptr);	// TODO: Test
	LN_FUNC_TRY_END_RETURN;
}
#endif

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

//------------------------------------------------------------------------------
LNResult LNSprite_GetTexture(LN_HANDLE(LNSprite) sprite, LN_OUT LN_HANDLE(LNTexture)* outTexture)
{
	LN_CHECK_ARG_HANDLE(sprite);
	LN_CHECK_ARG_RETURN(outTexture != nullptr);
	LN_FUNC_TRY_BEGIN;
	Texture* tex = TO_REFOBJ(Sprite, sprite)->GetTexture();
	*outTexture = TO_HANDLE_ADDREF(tex);
	LN_FUNC_TRY_END_RETURN;
}

//==============================================================================
// LNSprite2D
//==============================================================================
LN_TYPE_INFO_IMPL(Sprite2D, LNSprite2D);

//------------------------------------------------------------------------------
LNResult LNSprite2D_Create(LN_OUT LN_HANDLE(LNSprite2D)* outSprite2D)
{
	LN_FUNC_TRY_BEGIN;
	auto ptr = RefPtr<LNWISprite2D>::MakeRef();
	ptr->Initialize(LFManager::Engine->GetSceneGraphManager()->GetDefault2DSceneGraph());
	*outSprite2D = LFManager::CheckRegisterObject(ptr);
	ptr.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSprite2D_CreateFromTexture(LN_HANDLE(LNTexture) texture, LN_OUT LN_HANDLE(LNSprite2D)* outSprite2D)
{
	LN_FUNC_TRY_BEGIN;
	auto ptr = RefPtr<LNWISprite2D>::MakeRef();
	ptr->Initialize(LFManager::Engine->GetSceneGraphManager()->GetDefault2DSceneGraph());
	ptr->SetTexture(TO_REFOBJ(Texture, texture));
	*outSprite2D = LFManager::CheckRegisterObject(ptr);
	ptr.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}
