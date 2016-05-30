
#include "../Internal.h"
#include <Lumino/Graphics/RenderingContext.h>
#include "MME/MMEShaderErrorInfo.h"
#include "MME/MMEShader.h"
#include "MME/MMERenderingPass.h"
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/VisualNode.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/Light.h>
#include <Lumino/Scene/SceneGraph.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// LayerList
//==============================================================================
LN_REF_OBJECT_LIST_IMPL(LayerList, Layer);

//==============================================================================
// SceneGraph
//==============================================================================

//------------------------------------------------------------------------------
SceneGraph* SceneGraph::GetDefault2DSceneGraph()
{
	return SceneGraphManager::Instance->GetDefault2DSceneGraph();
}

//------------------------------------------------------------------------------
SceneGraph* SceneGraph::GetDefault3DSceneGraph()
{
	return SceneGraphManager::Instance->GetDefault3DSceneGraph();
}

//------------------------------------------------------------------------------
SceneGraph::SceneGraph()
	: m_manager(NULL)
	, m_time(0)
	, m_elapsedTime(0)
{
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

	m_layerList.Clear();
	LN_SAFE_RELEASE(m_manager);
}

//------------------------------------------------------------------------------
void SceneGraph::CreateCore(SceneGraphManager* manager)
{
	LN_REFOBJ_SET(m_manager, manager);
}

//------------------------------------------------------------------------------
void SceneGraph::UpdateFrame(float deltaTime)
{
	//SceneNodeList::iterator itr = m_allNodes.begin();
	//SceneNodeList::iterator end = m_allNodes.end();
	//for (; itr != end; )
	//{
	//	(*itr)->UpdateFrame(deltaTime);

	//	if ((*itr)->IsAutoRemove() && (*itr)->GetRefCount() == 1)
	//	{
	//		itr = m_allNodes.erase(itr);
	//		end = m_allNodes.end();
	//		(*itr)->Release();
	//	}
	//	else
	//	{
	//		++itr;
	//	}
	//}

	//LN_FOREACH(SceneNode* node, m_allNodes) {
	//	node->UpdateFrame(elapsedTime);
	//}

	m_elapsedTime = deltaTime;
	m_time += deltaTime;
}

//------------------------------------------------------------------------------
void SceneGraph::Render(Texture* renderTarget, Camera* camera)
{
	SizeF viewSize((float)renderTarget->GetSize().width, (float)renderTarget->GetSize().height);

	// 全てのシェーダの Scene 単位データの更新
	MMESceneParams sceneParams;
	sceneParams.Time = (float)m_time;
	sceneParams.ElapsedTime = m_elapsedTime;
	sceneParams.MousePosition.Set((2.0f * ((float)m_mousePosition.x) / viewSize.width) - 1.0f, (2.0f * ((float)m_mousePosition.y) / viewSize.height) - 1.0f, 0, 0);
	m_leftMouseState.ToVector4(viewSize, &sceneParams.LeftMouseDown);
	m_rightMouseState.ToVector4(viewSize, &sceneParams.MiddleMouseDown);
	m_middleMouseState.ToVector4(viewSize, &sceneParams.RightMouseDown);
	LN_FOREACH(MMEShader* shader, *m_manager->GetShaderList()) {
		shader->UpdateSceneParams(sceneParams, m_manager);
	}
	
	
	{
		m_renderingNodeList.Clear();
		m_renderingLightList.Clear();

		// カメラ行列の更新
		camera->UpdateMatrices(viewSize);

		// シェーダのカメラ単位データの更新
		//		TODO: とりあえず全シェーダ更新している。そんなにたくさんのシェーダは使わないだろうという想定。
		//		もちろん数が増えてくればこの辺がパフォーマンス的にクリティカルになる。
		//		改善案はあるが、とりあえず。(SceneGraphManaer.cpp 参照)
		LN_FOREACH(MMEShader* shader, *m_manager->GetShaderList()) {
			shader->UpdateCameraParams(camera, viewSize);
		}

		// このレイヤーのカメラに依るデータを更新する (視錘台カリングやカメラからの距離の更新
		GetRootNode()->UpdateViewFlustumHierarchy(camera, &m_renderingNodeList, &m_renderingLightList);

		// ライト行列の更新
		LN_FOREACH(Light* light, m_renderingLightList) {
			light->UpdateMatrices(viewSize);
		}

		// Z ソート・優先度ソート
		std::stable_sort(m_renderingNodeList.begin(), m_renderingNodeList.end(), SceneNode::CmpZAndPrioritySort);

	}

	{
		//RenderingParams params;
		SceneGraphRenderingContext* dc = m_manager->GetRenderingContext();
		dc->CurrentCamera = camera;
		dc->GetRenderingContext()->SetViewProjection(camera->GetViewMatrix(), camera->GetProjectionMatrix());
		dc->GetRenderingContext()->SetRenderTarget(0, renderTarget);
		for (RenderingPass* pass : *GetRenderingPasses())
		{
			if (pass == nullptr) continue;

			dc->Pass = pass;	// TODO: いらないかも
			for (SceneNode* node : m_renderingNodeList)
			{
				if (node->GetSceneNodeType() != SceneNodeType_VisualNode) {
					continue;
				}
				switch (node->GetRenderingMode())
				{
					case SceneNodeRenderingMode::Invisible:
						break;
					case SceneNodeRenderingMode::Visible:
						pass->RenderNode(dc, node);
						break;
					case SceneNodeRenderingMode::NonShaderVisible:
					{
						VisualNode* visualNode = static_cast<VisualNode*>(node);
						int subsetCount = visualNode->GetSubsetCount();
						for (int iSubset = 0; iSubset < subsetCount; iSubset++)
						{
							dc->Shader = nullptr;
							visualNode->DrawSubsetInternal(dc, iSubset, nullptr, nullptr);
						}
						break;
					}
				}
			}
			pass->PostRender(dc);
		}
	}


	// 各レイヤーのレンダリングを実行する
	//int count = m_layerList.GetCount();
	//for (int i = 0; i < count; i++) {
	//	m_layerList.GetAt(i)->PreRender(viewSize);
	//}
	//for (int i = 0; i < count; i++) {
	//	m_layerList.GetAt(i)->Render();
	//}
	//for (int i = count - 1; i >= 0; i--) {	// post は逆順
	//	m_layerList.GetAt(i)->PostRender();
	//}
}

//------------------------------------------------------------------------------
bool SceneGraph::InjectMouseMove(int x, int y)
{
	// シェーダ系
	m_mousePosition.Set(x, y);

	// カメラ
	for (Camera* camera : m_allCameraList) {
		if (camera->GetCameraBehavior() != nullptr) {
			camera->GetCameraBehavior()->InjectMouseMove(x, y);
		}
	}
	return true;
}

//------------------------------------------------------------------------------
bool SceneGraph::InjectMouseButtonDown(MouseButton button, int x, int y)
{
	// TODO: マウス位置はSceneGraphよりも Viewport(CameraLayer)に付くようにしたい。
	// プレビュー用のUIElementに描きたいときどうするの？という話。
	// なお、offscreenrendertarget については特にサポートしない。というか無理。
	
	// シェーダ系
	switch (button)
	{
	case MouseButton::Left:
		m_leftMouseState.position = m_mousePosition;
		m_leftMouseState.time = static_cast<float>(m_time);
		m_leftMouseState.isDown = true;
		break;
	case MouseButton::Right:
		m_rightMouseState.position = m_mousePosition;
		m_rightMouseState.time = static_cast<float>(m_time);
		m_rightMouseState.isDown = true;
		break;
	case MouseButton::Middle:
		m_middleMouseState.position = m_mousePosition;
		m_middleMouseState.time = static_cast<float>(m_time);
		m_middleMouseState.isDown = true;
		break;
	default:
		break;
	}

	// カメラ
	for (Camera* camera : m_allCameraList) {
		if (camera->GetCameraBehavior() != nullptr) {
			camera->GetCameraBehavior()->InjectMouseButtonDown(button, x, y);
		}
	}
	return false;
}

//------------------------------------------------------------------------------
bool SceneGraph::InjectMouseButtonUp(MouseButton button, int x, int y)
{
	// シェーダ系
	switch (button)
	{
	case MouseButton::Left:
		m_leftMouseState.isDown = false;
		break;
	case MouseButton::Right:
		m_leftMouseState.isDown = false;
		break;
	case MouseButton::Middle:
		m_leftMouseState.isDown = false;
		break;
	default:
		break;
	}
	
	// カメラ
	for (Camera* camera : m_allCameraList) {
		if (camera->GetCameraBehavior() != nullptr) {
			camera->GetCameraBehavior()->InjectMouseButtonUp(button, x, y);
		}
	}
	return false;
}

//------------------------------------------------------------------------------
bool SceneGraph::InjectMouseWheel(int delta)
{
	for (Camera* camera : m_allCameraList) {
		if (camera->GetCameraBehavior() != nullptr) {
			camera->GetCameraBehavior()->InjectMouseWheel(delta);
		}
	}
	return false;
}

//==============================================================================
// Basic2DSceneGraph
//==============================================================================

static const byte_t g_SSBasic2D_Data[] =
{
#include "Resource/SSBasic2D.fxc.h"
};
static const size_t g_SSBasic2D_Data_Len = LN_ARRAY_SIZE_OF(g_SSBasic2D_Data);

//------------------------------------------------------------------------------
Basic2DSceneGraph::Basic2DSceneGraph()
	: m_defaultRoot(nullptr)
	, m_defaultCamera(nullptr)
{
}

//------------------------------------------------------------------------------
Basic2DSceneGraph::~Basic2DSceneGraph()
{
	for (RenderingPass* pass : m_renderingPasses) {
		LN_SAFE_RELEASE(pass);
	}

	LN_SAFE_RELEASE(m_defaultCamera);
	LN_SAFE_RELEASE(m_defaultRoot);
}

//------------------------------------------------------------------------------
void Basic2DSceneGraph::CreateCore(SceneGraphManager* manager)
{
	SceneGraph::CreateCore(manager);

	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->Initialize(this);

	m_defaultCamera = LN_NEW Camera();
	m_defaultCamera->Initialize(this, CameraProjection_2D);
	m_defaultRoot->AddChild(m_defaultCamera);

	MMEShaderErrorInfo result;
	RefPtr<MMEShader> defaultShader(MMEShader::Create((const char*)g_SSBasic2D_Data, g_SSBasic2D_Data_Len, &result, GetManager()), false);
	m_renderingPasses.Add(LN_NEW MMERenderingPass(manager, MMD_PASS_object));
	m_renderingPasses[0]->SetDefaultShader(defaultShader);
}

//------------------------------------------------------------------------------
void Basic2DSceneGraph::UpdateFrame(float elapsedTime)
{
	SceneGraph::UpdateFrame(elapsedTime);
	m_defaultRoot->UpdateFrameHierarchy(nullptr, elapsedTime);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
