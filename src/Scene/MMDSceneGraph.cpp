
#include "../Internal.h"
#include "MME/MmdMaterial.h"
#include "MME/MMEShaderErrorInfo.h"
#include "MME/MMEShader.h"
#include "MME/MMERenderingPass.h"
#include <Lumino/Scene/Light.h>
#include <Lumino/Scene/MMDSceneGraph.h>
#include "InfomationRenderingPass.h"
#include "EffectBatchRendererNode.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// MMDSceneGraph
//==============================================================================

static const byte_t g_SSNoLighting_Data[] =
{
#include "Resource/SSBasic2D.fxc.h"
};
static const size_t g_SSNoLighting_Data_Len = LN_ARRAY_SIZE_OF(g_SSNoLighting_Data);

//------------------------------------------------------------------------------
MMDSceneGraph::MMDSceneGraph()
	: /*m_defaultMaterial(nullptr)
	, */m_defaultRoot(nullptr)
	, m_default3DCamera(nullptr)
	, m_effectBatchRendererNode(nullptr)
	//, m_mmdRenderingPasses{}
{
}

//------------------------------------------------------------------------------
MMDSceneGraph::~MMDSceneGraph()
{
	for (int i = 0; i < MMD_PASS_Max; i++) {
		LN_SAFE_RELEASE(m_mmdRenderingPasses[i]);
	}

	LN_SAFE_RELEASE(m_defaultLight);
	LN_SAFE_RELEASE(m_default3DCamera);
	LN_SAFE_RELEASE(m_effectBatchRendererNode);
	LN_SAFE_RELEASE(m_defaultRoot);
	//LN_SAFE_RELEASE(m_defaultMaterial);
}

//------------------------------------------------------------------------------
void MMDSceneGraph::CreateCore(SceneGraphManager* manager)
{
	SceneGraph::CreateCore(manager);

	//m_defaultMaterial = CreateMaterial();

	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->Initialize(this);

	if (manager->GetEffectManager() != nullptr)
	{
		m_effectBatchRendererNode = LN_NEW EffectBatchRendererNode();
		m_effectBatchRendererNode->Initialize(this, manager->GetEffectManager());
		m_effectBatchRendererNode->SetPriority(-1000);
		m_defaultRoot->AddChild(m_effectBatchRendererNode);
	}

	m_default3DCamera = LN_NEW Camera();
	m_default3DCamera->Initialize(this, CameraProjection_3D);
	m_defaultRoot->AddChild(m_default3DCamera);

	m_defaultLight = LN_NEW Light();
	m_defaultLight->Initialize(this, LightType_Directional);
	m_defaultRoot->AddChild(m_defaultLight);

	//GetLayerList()->Add(m_default3DLayer);
	//GetLayerList()->Add(m_default2DLayer);

	m_mmdRenderingPasses.Resize(MMD_PASS_Max);
	//m_mmdRenderingPasses[MMD_PASS_zplot] = LN_NEW MMERenderingPass(this, MMD_PASS_zplot);
	//m_mmdRenderingPasses[MMD_PASS_shadow] = LN_NEW MMERenderingPass(this, MMD_PASS_shadow);
	//m_mmdRenderingPasses[MMD_PASS_edge] = LN_NEW MMERenderingPass(this, MMD_PASS_edge);

	m_mmdRenderingPasses[MMD_PASS_object] = LN_NEW MMERenderingPass(manager, MMD_PASS_object);
	//m_mmdRenderingPasses[MMD_PASS_object_ss] = LN_NEW MMERenderingPass(this, MMD_PASS_object_ss);
	//m_mmdRenderingPasses[MMD_PASS_Infomation] = LN_NEW InfomationRenderingPass(manager);

	//m_default3DLayer->GetRenderingPasses()->Add(m_mmdRenderingPasses[MMD_PASS_object]);
	//m_default3DLayer->GetRenderingPasses()->Add(m_mmdRenderingPasses[MMD_PASS_Infomation]);

	MMEShaderErrorInfo result;
	RefPtr<MMEShader> defaultShader(MMEShader::Create((const char*)g_SSNoLighting_Data, g_SSNoLighting_Data_Len, &result, GetManager()), false);
	m_mmdRenderingPasses[MMD_PASS_object]->SetDefaultShader(defaultShader);
}

//------------------------------------------------------------------------------
void MMDSceneGraph::UpdateFrame(float elapsedTime)
{
	SceneGraph::UpdateFrame(elapsedTime);

	m_defaultRoot->UpdateFrameHierarchy(nullptr, elapsedTime);
}

//------------------------------------------------------------------------------
//bool MMDSceneGraph::InjectMouseWheel(int delta)
//{
//	m_default3DCamera->DoMouseWheel(delta);
//	return true;
//}

//------------------------------------------------------------------------------
detail::MaterialInstance* MMDSceneGraph::CreateMaterialInstance()
{
	return LN_NEW MmdMaterialInstance();
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
