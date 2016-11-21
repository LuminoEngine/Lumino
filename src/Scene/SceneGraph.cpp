
#include "../Internal.h"
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Graphics/Mesh.h>
//#include "MME/MMERenderingPass.h"
//#include "MME/MmdMaterial.h"	// TODO
#include "SceneGraphManager.h"
#include "RenderingPass.h"
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
	GetRenderingProfiler().Subscribe();
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

	//	if ((*itr)->IsAutoRemove() && (*itr)->GetReferenceCount() == 1)
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
#if 0
void SceneGraph::Render(RenderingContext* context, Camera* camera)
{
	Texture* renderTarget = context->GetRenderTarget(0);
	Size viewSize((float)renderTarget->GetSize().width, (float)renderTarget->GetSize().height);

	// 全てのシェーダの Scene 単位データの更新
	MMESceneParams sceneParams;
	sceneParams.Time = (float)m_time;
	sceneParams.ElapsedTime = m_elapsedTime;
	sceneParams.MousePosition.Set((2.0f * ((float)m_mousePosition.x) / viewSize.width) - 1.0f, (2.0f * ((float)m_mousePosition.y) / viewSize.height) - 1.0f, 0, 0);
	m_leftMouseState.ToVector4(viewSize, &sceneParams.LeftMouseDown);
	m_rightMouseState.ToVector4(viewSize, &sceneParams.MiddleMouseDown);
	m_middleMouseState.ToVector4(viewSize, &sceneParams.RightMouseDown);
	for (MMEShader* shader : *m_manager->GetShaderList())
	{
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
		for (MMEShader* shader : *m_manager->GetShaderList()) {
			shader->UpdateCameraParams(camera, viewSize);
		}

		// このレイヤーのカメラに依るデータを更新する (視錘台カリングやカメラからの距離の更新
		GetRootNode()->UpdateViewFlustumHierarchy(camera, &m_renderingNodeList, &m_renderingLightList);

		// ライト行列の更新
		for (Light* light : m_renderingLightList) {
			light->UpdateMatrices(viewSize);
		}

		// Z ソート・優先度ソート
		std::stable_sort(m_renderingNodeList.begin(), m_renderingNodeList.end(), SceneNode::CmpZAndPrioritySort);
	}

	{
		SceneGraphRenderingContext* dc = m_manager->GetRenderingContext();
		dc->InheritStatus(context);	// context から、レンダーステートやレンダーターゲット、深度バッファを引き継ぐ
		dc->SetCurrentCamera(camera);
		dc->renderingLightList = &m_renderingLightList;
		dc->SetRenderTarget(0, renderTarget);
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
#endif

//------------------------------------------------------------------------------
void SceneGraph::Render2(DrawList* renderer, Camera* camera)
{
	GetRenderingProfiler().BeginSession();
	GetRootNode()->Render2(renderer);
	GetRenderingProfiler().EndSession();
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
// SceneGraph2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SceneGraph2D, SceneGraph);

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
void SceneGraph2D::CreateCore(SceneGraphManager* manager)
{
	SceneGraph::CreateCore(manager);

	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->Initialize(this);

	m_defaultCamera = LN_NEW Camera();
	m_defaultCamera->Initialize(this, CameraProjection_2D);
	m_defaultRoot->AddChild(m_defaultCamera);

	//auto pass = RefPtr<MMERenderingPass>::MakeRef(manager, MMD_PASS_object);
	//pass->Initialize();
	//m_renderingPasses.Add(pass);
	//pass->AddRef();
}

//------------------------------------------------------------------------------
void SceneGraph2D::UpdateFrame(float elapsedTime)
{
	SceneGraph::UpdateFrame(elapsedTime);
	m_defaultRoot->UpdateFrameHierarchy(nullptr, elapsedTime);
}

//==============================================================================
// SceneGraph3D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SceneGraph3D, SceneGraph);
LN_TR_PROPERTY_IMPLEMENT(SceneGraph3D, bool, VisibleGridPlane, tr::PropertyMetadata());

//------------------------------------------------------------------------------
SceneGraph3D::SceneGraph3D()
	: VisibleGridPlane(false)
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
void SceneGraph3D::CreateCore(SceneGraphManager* manager)
{
	SceneGraph::CreateCore(manager);

	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->Initialize(this);

	m_defaultCamera = LN_NEW Camera();
	m_defaultCamera->Initialize(this, CameraProjection_3D);
	m_defaultRoot->AddChild(m_defaultCamera);

	m_defaultLight = RefPtr<Light>::MakeRef();
	m_defaultLight->Initialize(this, LightType_Directional);
	m_defaultRoot->AddChild(m_defaultLight);

	CreateGridContents();
}

//------------------------------------------------------------------------------
void SceneGraph3D::UpdateFrame(float elapsedTime)
{
	SceneGraph::UpdateFrame(elapsedTime);
	m_defaultRoot->UpdateFrameHierarchy(nullptr, elapsedTime);
}

//------------------------------------------------------------------------------
void SceneGraph3D::Render2(DrawList* renderer, Camera* camera)
{
	SceneGraph::Render2(renderer, camera);

	if (VisibleGridPlane)
	{
		AdjustGridMesh(camera);
		renderer->DrawMesh(m_gridPlane, 0, m_gridPlane->GetMeshResource()->GetMaterial(0));
	}
}

//------------------------------------------------------------------------------
void SceneGraph3D::CreateGridContents()
{
	detail::GraphicsManager* gm = GetManager()->GetGraphicsManager();

	// 適当な四角形メッシュ
	m_gridPlane = RefPtr<StaticMeshModel>::MakeRef();
	m_gridPlane->InitializeSquarePlane(gm, Vector2(1, 1), Vector3::UnitY, MeshCreationFlags::DynamicBuffers);
	MeshResource* mesh = m_gridPlane->GetMeshResource();

	// シェーダ (DrawingContext3D)
	static const byte_t shaderCode[] =
	{
#include "Resource/InfinitePlaneGrid.lfx.h"
	};
	static const size_t shaderCodeLen = LN_ARRAY_SIZE_OF(shaderCode);
	auto shader = RefPtr<Shader>::MakeRef();
	shader->Initialize(gm, shaderCode, shaderCodeLen);
	mesh->GetMaterial(0)->SetShader(shader);

	// 四方の辺に黒線を引いたテクスチャを作り、マテリアルにセットしておく
	SizeI gridTexSize(512, 512);
	auto gridTex = RefPtr<Texture2D>::MakeRef();
	gridTex->Initialize(gm, gridTexSize, TextureFormat::R8G8B8A8, true);
	for (int x = 0; x < gridTexSize.width; ++x)
	{
		gridTex->SetPixel(x, 0, Color(0, 0, 0, 0.5));
		gridTex->SetPixel(x, gridTexSize.width - 1, Color(0, 0, 0, 0.5));
	}
	for (int y = 0; y < gridTexSize.height; ++y)
	{
		gridTex->SetPixel(0, y, Color(0, 0, 0, 0.5));
		gridTex->SetPixel(gridTexSize.height - 1, y, Color(0, 0, 0, 0.5));
	}
	mesh->GetMaterial(0)->SetMaterialTexture(gridTex);

	mesh->GetMaterial(0)->SetBlendMode(BlendMode::Alpha);
}

//------------------------------------------------------------------------------
void SceneGraph3D::AdjustGridMesh(Camera* camera)
{
	/*
	カメラの視錐台と、グリッドを描画したい平面との衝突点から、四角形メッシュを作る。
	これで視界に映る平面全体をカバーするメッシュができる。
	あとはシェーダで、頂点の位置を利用してグリッドテクスチャをサンプリングする。
	*/

	struct Line
	{
		Vector3	from;
		Vector3	to;
	};

	auto& vf = camera->GetViewFrustum();
	Vector3 points[8];
	vf.GetCornerPoints(points);

	Line lines[12] =
	{
		{ points[0], points[1] },
		{ points[1], points[2] },
		{ points[2], points[3] },
		{ points[3], points[0] },

		{ points[0], points[4] },
		{ points[1], points[5] },
		{ points[2], points[6] },
		{ points[3], points[7] },

		{ points[4], points[5] },
		{ points[5], points[6] },
		{ points[6], points[7] },
		{ points[7], points[4] },
	};

	Plane plane(0, 1, 0, 0);
	List<Vector3> hits;
	for (Line& li : lines)
	{
		Vector3 pt;
		if (plane.Intersects(li.from, li.to, &pt))
		{
			hits.Add(pt);
		}
	}

	Vector3 minPos, maxPos;
	for (Vector3& p : hits)
	{
		minPos = Vector3::Min(p, minPos);
		maxPos = Vector3::Max(p, maxPos);
	}

	MeshResource* mesh = m_gridPlane->GetMeshResource();
	mesh->SetPosition(0, Vector3(minPos.x, 0, maxPos.z));
	mesh->SetPosition(1, Vector3(minPos.x, 0, minPos.z));
	mesh->SetPosition(2, Vector3(maxPos.x, 0, maxPos.z));
	mesh->SetPosition(3, Vector3(maxPos.x, 0, minPos.z));
	mesh->SetUV(0, Vector2(-1.0f, 1.0f));
	mesh->SetUV(1, Vector2(-1.0f, -1.0f));
	mesh->SetUV(2, Vector2(1.0f, 1.0f));
	mesh->SetUV(3, Vector2(1.0f, -1.0f));
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
