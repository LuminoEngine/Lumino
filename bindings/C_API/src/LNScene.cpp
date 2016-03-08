//=============================================================================
// LNScene 
//=============================================================================

#include "LNInternal.h"
#include "../include/LNScene.h"

//=============================================================================
// LNSceneNode
//=============================================================================
LN_TYPE_INFO_IMPL(SceneNode, LNSceneNode);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSceneNode_SetPosition(LN_HANDLE(LNSceneNode) sceneNode, const LNVector3* position)
{
	LN_CHECK_ARG_HANDLE(sceneNode);
	LN_CHECK_ARG(position != NULL);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(SceneNode, sceneNode)->SetPosition(*cp_cast<Vector3>(position));
	LN_FUNC_TRY_END_RETURN;
}


//=============================================================================
// LNSprite
//=============================================================================
LN_TYPE_INFO_IMPL(Sprite2D, LNSprite);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSprite_Create(LN_HANDLE(LNTexture) texture, LN_OUT LN_HANDLE(LNSprite)* outSprite)
{
	LN_FUNC_TRY_BEGIN;
	//RefPtr<Texture2D> obj(LN_NEW Wrapper_Texture2D(), false);
	//obj->CreateCore(LFManager::Engine->GetGraphicsManager(), filePath, TextureFormat_B8G8R8A8, 1);
	//*texture2D = LFManager::CheckRegisterObject(obj);
	//obj.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}
