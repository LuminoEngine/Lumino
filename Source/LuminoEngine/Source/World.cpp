
#include "Internal.h"
#include <Lumino/Physics/PhysicsWorld.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/World.h>
#include "EngineManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// World
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(World, Object);

//------------------------------------------------------------------------------
World::World()
{
}

//------------------------------------------------------------------------------
World::~World()
{
}

//------------------------------------------------------------------------------
void World::Initialize()
{
}

//------------------------------------------------------------------------------
void World::BeginUpdateFrame()
{
}

//------------------------------------------------------------------------------
void World::UpdateFrame(float elapsedTime)
{
}

//------------------------------------------------------------------------------
void World::Render(Camera* camera)
{
	SceneGraph* scene = GetSceneGraph();
	if (scene != nullptr)
	{
		scene->Render2(scene->GetRenderer(), camera);
	}
}

//------------------------------------------------------------------------------
void World::ExecuteDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
}

//==============================================================================
// World2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(World2D, World);

//------------------------------------------------------------------------------
World2D::World2D()
{
}

//------------------------------------------------------------------------------
World2D::~World2D()
{
}

//------------------------------------------------------------------------------
void World2D::Initialize()
{
	World::Initialize();

	m_sceneGraph = RefPtr<SceneGraph2D>::MakeRef();
	m_sceneGraph->CreateCore(EngineManager::GetInstance()->GetSceneGraphManager());
}

//------------------------------------------------------------------------------
SceneGraph* World2D::GetSceneGraph()
{
	return GetSceneGraph2D();
}

//------------------------------------------------------------------------------
SceneGraph2D* World2D::GetSceneGraph2D() const
{
	return m_sceneGraph;
}


//------------------------------------------------------------------------------
void World2D::BeginUpdateFrame()
{
	if (m_sceneGraph != nullptr) {
		m_sceneGraph->BeginUpdateFrame();
	}
}

//------------------------------------------------------------------------------
void World2D::UpdateFrame(float elapsedTime)
{
	if (m_sceneGraph != nullptr) {
		m_sceneGraph->UpdateFrame(elapsedTime);
	}
}

//==============================================================================
// World3D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(World3D, World);

//------------------------------------------------------------------------------
World3D::World3D()
{
}

//------------------------------------------------------------------------------
World3D::~World3D()
{
}

//------------------------------------------------------------------------------
void World3D::Initialize()
{
	World::Initialize();

	m_physicsWorld = NewObject<PhysicsWorld>();

	m_sceneGraph = RefPtr<SceneGraph3D>::MakeRef();
	m_sceneGraph->CreateCore(EngineManager::GetInstance()->GetSceneGraphManager());
}

//------------------------------------------------------------------------------
SceneGraph* World3D::GetSceneGraph()
{
	return GetSceneGraph3D();
}

//------------------------------------------------------------------------------
PhysicsWorld* World3D::GetPhysicsWorld3D() const
{
	return m_physicsWorld;
}

//------------------------------------------------------------------------------
SceneGraph3D* World3D::GetSceneGraph3D() const
{
	return m_sceneGraph;
}

//------------------------------------------------------------------------------
void World3D::BeginUpdateFrame()
{
	if (m_sceneGraph != nullptr)
	{
		m_sceneGraph->BeginUpdateFrame();
	}
}

//------------------------------------------------------------------------------
void World3D::UpdateFrame(float elapsedTime)
{
	if (m_physicsWorld != nullptr)
	{
		m_physicsWorld->StepSimulation(elapsedTime);
	}

	if (m_sceneGraph != nullptr)
	{
		m_sceneGraph->UpdateFrame(elapsedTime);
	}
}


LN_NAMESPACE_END
