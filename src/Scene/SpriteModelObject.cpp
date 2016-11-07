
#include "../Internal.h"
#include <ssbpLib/SS5PlayerPlatform.h>
#include <ssbpLib/SS5Player.h>
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/SpriteModelObject.h>

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// SpriteModelObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpriteModelObject, VisualNode);

//------------------------------------------------------------------------------
RefPtr<SpriteModelObject> SpriteModelObject::Create(const String& filePath)
{
	RefPtr<SpriteModelObject> obj(LN_NEW SpriteModelObject(), false);
	obj->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), filePath);
	return obj;
}

//------------------------------------------------------------------------------
SpriteModelObject::SpriteModelObject()
	: VisualNode()
{
}

//------------------------------------------------------------------------------
SpriteModelObject::~SpriteModelObject()
{
	LN_SAFE_DELETE(m_player);
	LN_SAFE_DELETE(m_resource);
}

//------------------------------------------------------------------------------
void SpriteModelObject::Initialize(SceneGraph* owner, const String& filePath)
{
	VisualNode::Initialize(owner, 1);

	ss::g_graphicsManager = owner->GetManager()->GetGraphicsManager();

	PathNameA path = filePath.ToStringA();
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
		owner->GetManager()->GetDefault3DSceneGraph()->GetRootNode()->AddChild(this);
		SetAutoRemove(true);
	//}
}

//------------------------------------------------------------------------------
void SpriteModelObject::OnUpdateFrame(float elapsedTime)
{
	m_player->update(elapsedTime);
}

//------------------------------------------------------------------------------
//void SpriteModelObject::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
//{
//	m_player->draw();
//}

//------------------------------------------------------------------------------
//void SpriteModelObject::OnRender(SceneGraphRenderingContext* dc)
//{
//	LN_NOTIMPLEMENTED();
//	/*
//	auto* sr = dc->BeginGraphicsContext()->GetSpriteRenderer();
//	sr->SetViewProjMatrix(dc->CurrentCamera->GetViewMatrix(), dc->CurrentCamera->GetProjectionMatrix());
//	sr->SetViewPixelSize(Size(640, 480));
//	ss::g_spriteRenderer = sr;
//	m_player->draw();
//	sr->Flush();
//	*/
//}

} // namespace tr
LN_NAMESPACE_END
