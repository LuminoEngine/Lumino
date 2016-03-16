
#include "../Internal.h"
#include "SceneGraphManager.h"
#include <Lumino/Scene/Sprite2D.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// Sprite2D
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite2D, SpriteBase);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite2DPtr Sprite2D::Create()
{
	auto obj = Sprite2DPtr::MakeRef();
	obj->Initialize(SceneGraphManager::Instance);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite2DPtr Sprite2D::Create(const StringRef& filePath)
{
	auto tex = Texture2D::Create(filePath);
	return Create(tex);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite2DPtr Sprite2D::Create(Texture* texture)
{
	auto obj = Create();
	obj->SetTexture(texture);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite2D::Sprite2D()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite2D::~Sprite2D()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sprite2D::Initialize(SceneGraphManager* manager)
{
	SpriteBase::Initialize(manager, SpriteCoord_2D);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
