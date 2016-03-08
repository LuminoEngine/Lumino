
#include "../Internal.h"
#include "SceneGraphManager.h"
#include <Lumino/Scene/Sprite3D.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// Sprite3D
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite3D, SpriteBase);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite3DPtr Sprite3D::Create()
{
	auto obj = Sprite3DPtr::MakeRef();
	obj->Initialize(SceneGraphManager::Instance);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite3D::Sprite3D()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite3D::~Sprite3D()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sprite3D::Initialize(SceneGraphManager* manager)
{
	SpriteBase::Initialize(manager, SpriteCoord_2D);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
