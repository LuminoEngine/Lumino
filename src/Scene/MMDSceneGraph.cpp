
#include "../Internal.h"
#include "MME/MMEShaderErrorInfo.h"
#include "MME/MMEShader.h"
#include "MME/MMERenderingPass.h"
#include <Lumino/Scene/MMDSceneGraph.h>
#include "InfomationRenderingPass.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// MMDSceneGraph
//=============================================================================

static const byte_t g_SSNoLighting_Data[] =
{
#include "Resource/SSBasic2D.fx.h"
};
static const size_t g_SSNoLighting_Data_Len = LN_ARRAY_SIZE_OF(g_SSNoLighting_Data);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMDSceneGraph::MMDSceneGraph()
	: m_defaultRoot(nullptr)
	, m_default3DCamera(nullptr)
	//, m_mmdRenderingPasses{}
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

	LN_SAFE_RELEASE(m_defaultRoot);
	LN_SAFE_RELEASE(m_default3DCamera);
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


	m_default3DCamera = LN_NEW Camera();
	m_default3DCamera->CreateCore(manager, CameraProjection_3D);
	m_defaultRoot->AddChild(m_default3DCamera);


	//GetLayerList()->Add(m_default3DLayer);
	//GetLayerList()->Add(m_default2DLayer);

	m_mmdRenderingPasses.Resize(MMD_PASS_Max);
	//m_mmdRenderingPasses[MMD_PASS_zplot] = LN_NEW MMERenderingPass(this, MMD_PASS_zplot);
	//m_mmdRenderingPasses[MMD_PASS_shadow] = LN_NEW MMERenderingPass(this, MMD_PASS_shadow);
	//m_mmdRenderingPasses[MMD_PASS_edge] = LN_NEW MMERenderingPass(this, MMD_PASS_edge);
	m_mmdRenderingPasses[MMD_PASS_object] = LN_NEW MMERenderingPass(manager, MMD_PASS_object);
	//m_mmdRenderingPasses[MMD_PASS_object_ss] = LN_NEW MMERenderingPass(this, MMD_PASS_object_ss);
	m_mmdRenderingPasses[MMD_PASS_Infomation] = LN_NEW InfomationRenderingPass(manager);

	//m_default3DLayer->GetRenderingPasses()->Add(m_mmdRenderingPasses[MMD_PASS_object]);
	//m_default3DLayer->GetRenderingPasses()->Add(m_mmdRenderingPasses[MMD_PASS_Infomation]);

	MMEShaderErrorInfo result;
	RefPtr<MMEShader> defaultShader(MMEShader::Create((const char*)g_SSNoLighting_Data, g_SSNoLighting_Data_Len, &result, GetManager()), false);
	m_mmdRenderingPasses[MMD_PASS_object]->SetDefaultShader(defaultShader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMDSceneGraph::UpdateFrame(float elapsedTime)
{
	SceneGraph::UpdateFrame(elapsedTime);

	m_defaultRoot->UpdateFrameHierarchy(NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//bool MMDSceneGraph::InjectMouseWheel(int delta)
//{
//	m_default3DCamera->DoMouseWheel(delta);
//	return true;
//}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
