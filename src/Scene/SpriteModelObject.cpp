
#include "../Internal.h"
#include <ssbpLib\SS5PlayerPlatform.h>
#include <ssbpLib\SS5Player.h>
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/SpriteModelObject.h>

LN_NAMESPACE_BEGIN
namespace tr
{

//=============================================================================
// SpriteModelObject
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpriteModelObject, VisualNode);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<SpriteModelObject> SpriteModelObject::Create(const String& filePath)
{
	RefPtr<SpriteModelObject> obj(LN_NEW SpriteModelObject(), false);
	obj->Initialize(SceneGraphManager::Instance, filePath);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteModelObject::SpriteModelObject()
	: VisualNode()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteModelObject::~SpriteModelObject()
{
	LN_SAFE_DELETE(m_player);
	LN_SAFE_DELETE(m_resource);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteModelObject::Initialize(SceneGraphManager* manager, const String& filePath)
{
	VisualNode::CreateCore(manager, 1);

	ss::g_graphicsManager = manager->GetGraphicsManager();

	PathNameA path = StringA(filePath);
	m_resource = ss::ResourceManager::create();
	m_resource->addData(path.c_str());
	m_player = ss::Player::create();	// TODO: メモリリークあり
	m_player->setResourceManager(m_resource);
	m_player->setData(path.GetFileNameWithoutExtension().c_str());

	m_player->play("NewAnimation/anime_1");

	
	//if (spriteCoord == SpriteCoord_2D) {
	//	manager->GetDefault2DSceneGraph()->GetRootNode()->AddChild(this);
	//}
	//else {
		manager->GetDefault3DSceneGraph()->GetRootNode()->AddChild(this);
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteModelObject::UpdateFrame(float elapsedTime)
{
	m_player->update(elapsedTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteModelObject::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
{
	m_player->draw();
}

} // namespace tr
LN_NAMESPACE_END
