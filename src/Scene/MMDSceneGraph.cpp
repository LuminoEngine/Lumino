
#include "../Internal.h"
#include <Lumino/Scene/MMDSceneGraph.h>
#include "MME/MMERenderingPass.h"
#include "InfomationRenderingPass.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// MMDSceneGraph
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMDSceneGraph::MMDSceneGraph()
	: m_defaultRoot(nullptr)
	, m_default3DRoot(nullptr)
	, m_default2DRoot(nullptr)
	, m_default3DCamera(nullptr)
	, m_default2DCamera(nullptr)
	, m_default3DLayer(nullptr)
	, m_default2DLayer(nullptr)
	, m_mmdRenderingPasses{}
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMDSceneGraph::~MMDSceneGraph()
{
	for (int i = 0; i < MMD_PASS_Max; i++) {
		LN_SAFE_RELEASE(m_mmdRenderingPasses[i]);
	}

	LN_SAFE_RELEASE(m_default3DLayer);
	LN_SAFE_RELEASE(m_default2DLayer);
	LN_SAFE_RELEASE(m_defaultRoot);
	LN_SAFE_RELEASE(m_default3DCamera);
	LN_SAFE_RELEASE(m_default2DCamera);
	LN_SAFE_RELEASE(m_default2DRoot);
	LN_SAFE_RELEASE(m_default3DRoot);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMDSceneGraph::CreateCore(SceneGraphManager* manager)
{
	SceneGraph::CreateCore(manager);

	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->CreateCore(manager);
	m_defaultRoot->SetOwnerSceneGraph(this);

	m_default3DRoot = LN_NEW SceneNode();
	m_default3DRoot->CreateCore(manager);
	m_defaultRoot->AddChild(m_default3DRoot);

	m_default2DRoot = LN_NEW SceneNode();
	m_default2DRoot->CreateCore(manager);
	m_defaultRoot->AddChild(m_default2DRoot);

	m_default3DCamera = LN_NEW Camera();
	m_default3DCamera->CreateCore(manager, CameraProjection_3D);
	m_default3DRoot->AddChild(m_default3DCamera);

	m_default2DCamera = LN_NEW Camera();
	m_default2DCamera->CreateCore(manager, CameraProjection_2D);
	m_default2DRoot->AddChild(m_default2DCamera);

	m_default3DLayer = LN_NEW DrawingLayer(manager);
	m_default3DLayer->SetCamera(m_default3DCamera);
	m_default3DLayer->SetRenderingRootNode(m_default3DRoot);

	m_default2DLayer = LN_NEW DrawingLayer(manager);
	m_default2DLayer->SetCamera(m_default2DCamera);
	m_default2DLayer->SetRenderingRootNode(m_default2DRoot);

	GetLayerList()->Add(m_default3DLayer);
	GetLayerList()->Add(m_default2DLayer);

	//m_mmdRenderingPasses[MMD_PASS_zplot] = LN_NEW MMERenderingPass(this, MMD_PASS_zplot);
	//m_mmdRenderingPasses[MMD_PASS_shadow] = LN_NEW MMERenderingPass(this, MMD_PASS_shadow);
	//m_mmdRenderingPasses[MMD_PASS_edge] = LN_NEW MMERenderingPass(this, MMD_PASS_edge);
	m_mmdRenderingPasses[MMD_PASS_object] = LN_NEW MMERenderingPass(manager, MMD_PASS_object);
	//m_mmdRenderingPasses[MMD_PASS_object_ss] = LN_NEW MMERenderingPass(this, MMD_PASS_object_ss);
	m_mmdRenderingPasses[MMD_PASS_Infomation] = LN_NEW InfomationRenderingPass(manager);

	m_default3DLayer->GetRenderingPasses()->Add(m_mmdRenderingPasses[MMD_PASS_object]);
	m_default3DLayer->GetRenderingPasses()->Add(m_mmdRenderingPasses[MMD_PASS_Infomation]);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMDSceneGraph::UpdateFrame(float elapsedTime)
{
	SceneGraph::UpdateFrame(elapsedTime);

	m_defaultRoot->UpdateFrameHierarchy(NULL);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
