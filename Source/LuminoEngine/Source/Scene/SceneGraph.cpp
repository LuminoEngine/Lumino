
#include "../Internal.h"
#include <Lumino/Mesh/Mesh.h>
//#include "MME/MMERenderingPass.h"
//#include "MME/MmdMaterial.h"	// TODO
#include "SceneGraphManager.h"
#include <Lumino/Scene/VisualNode.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/Light.h>
#include <Lumino/Scene/SceneGraph.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

////==============================================================================
//// LayerList
////==============================================================================
//LN_REF_OBJECT_LIST_IMPL(LayerList, Layer);

//==============================================================================
// SceneGraph
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SceneGraph, Object);

//------------------------------------------------------------------------------
SceneGraph::SceneGraph()
	: m_manager(NULL)
	, m_time(0)
	, m_elapsedTime(0)
{
	// TODO:
	//GetRenderingProfiler().Subscribe();
}

//------------------------------------------------------------------------------
SceneGraph::~SceneGraph()
{
	//for (SceneNode* node : m_allNodes)
	//{
	//	if (node->IsAutoRemove())
	//	{
	//		node->Release();
	//	}
	//}

	//m_layerList.Clear();
	LN_SAFE_RELEASE(m_manager);
}

//------------------------------------------------------------------------------
void SceneGraph::createCore(SceneGraphManager* manager)
{
	LN_REFOBJ_SET(m_manager, manager);
}

//------------------------------------------------------------------------------
void SceneGraph::reginUpdateFrame()
{
}

//------------------------------------------------------------------------------
void SceneGraph::updateFrame(float deltaTime)
{
	m_elapsedTime = deltaTime;
	m_time += deltaTime;
}


////------------------------------------------------------------------------------
//DrawList* SceneGraph::getRenderer() const
//{
//	return m_renderer;
//}
//
////------------------------------------------------------------------------------
//DrawList* SceneGraph::GetDebugRenderer() const
//{
//	return m_debugRenderer;
//}
//

//==============================================================================
// SceneGraph2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SceneGraph2D, SceneGraph);

//------------------------------------------------------------------------------
SceneGraph2DPtr SceneGraph2D::create()
{
	auto ptr = SceneGraph2DPtr::makeRef();
	ptr->createCore(SceneGraphManager::Instance);
	return ptr;
}

//------------------------------------------------------------------------------
SceneGraph2D::SceneGraph2D()
	: m_defaultRoot(nullptr)
	, m_defaultCamera(nullptr)
{
}

//------------------------------------------------------------------------------
SceneGraph2D::~SceneGraph2D()
{
	//for (RenderingPass* pass : m_renderingPasses) {
	//	LN_SAFE_RELEASE(pass);
	//}

	LN_SAFE_RELEASE(m_defaultCamera);
	LN_SAFE_RELEASE(m_defaultRoot);
}

//------------------------------------------------------------------------------
void SceneGraph2D::createCore(SceneGraphManager* manager)
{
	SceneGraph::createCore(manager);

	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->initialize();

	m_defaultCamera = LN_NEW CameraComponent();
	m_defaultCamera->initialize(CameraProjection_2D);
	m_defaultRoot->AddChild(m_defaultCamera);

	//auto pass = RefPtr<MMERenderingPass>::MakeRef(manager, MMD_PASS_object);
	//pass->initialize();
	//m_renderingPasses.Add(pass);
	//pass->AddRef();
}

//------------------------------------------------------------------------------
void SceneGraph2D::updateFrame(float elapsedTime)
{
	SceneGraph::updateFrame(elapsedTime);
	m_defaultRoot->UpdateFrameHierarchy(nullptr, elapsedTime);
}

//==============================================================================
// SceneGraph3D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SceneGraph3D, SceneGraph);
LN_TR_PROPERTY_IMPLEMENT(SceneGraph3D, bool, visibleGridPlane, tr::PropertyMetadata());

//------------------------------------------------------------------------------
SceneGraph3D::SceneGraph3D()
	: visibleGridPlane(false)
	, m_defaultRoot(nullptr)
	, m_defaultCamera(nullptr)
{
}

//------------------------------------------------------------------------------
SceneGraph3D::~SceneGraph3D()
{
	LN_SAFE_RELEASE(m_defaultCamera);
	LN_SAFE_RELEASE(m_defaultRoot);
}

//------------------------------------------------------------------------------
void SceneGraph3D::createCore(SceneGraphManager* manager)
{
	SceneGraph::createCore(manager);

	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->initialize();

	m_defaultCamera = LN_NEW CameraComponent();
	m_defaultCamera->initialize(CameraProjection_3D);
	m_defaultRoot->AddChild(m_defaultCamera);

	m_defaultLight = RefPtr<LightComponent>::makeRef();
	m_defaultLight->initialize(LightType_Directional);
	m_defaultRoot->AddChild(m_defaultLight);
}

//------------------------------------------------------------------------------
//LightComponent* SceneGraph3D::GetMainLight() const { return m_defaultLight; }

//------------------------------------------------------------------------------
void SceneGraph3D::updateFrame(float elapsedTime)
{
	SceneGraph::updateFrame(elapsedTime);
	m_defaultRoot->UpdateFrameHierarchy(nullptr, elapsedTime);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
