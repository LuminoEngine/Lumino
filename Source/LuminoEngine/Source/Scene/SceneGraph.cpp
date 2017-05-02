
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

	m_renderer = NewObject<DrawList>(m_manager->GetGraphicsManager());
	m_debugRenderer = NewObject<DrawList>(m_manager->GetGraphicsManager());

	m_debugRendererDefaultMaterial = NewObject<Material>();
}

//------------------------------------------------------------------------------
void SceneGraph::BeginUpdateFrame()
{
	// OnRender の外のデバッグ描画などでも使用するため、描画リストのクリアは SceneGraph の更新前でなければならない。
	// また、出来上がった描画リストを、複数のビューやカメラが描画することを想定する。
	if (m_renderer != nullptr)
	{
		m_renderer->BeginMakeElements();
	}
	if (m_debugRenderer != nullptr)
	{
		m_debugRenderer->BeginMakeElements();
		m_debugRendererDefaultMaterial->SetShader(m_manager->GetGraphicsManager()->GetBuiltinShader(BuiltinShader::Sprite));
		m_debugRenderer->SetDefaultMaterial(m_debugRendererDefaultMaterial);
	}
}

//------------------------------------------------------------------------------
void SceneGraph::UpdateFrame(float deltaTime)
{
	m_elapsedTime = deltaTime;
	m_time += deltaTime;
}

//------------------------------------------------------------------------------
void SceneGraph::Render2(DrawList* renderer, CameraComponent* camera)
{
	GetRenderingProfiler().BeginSession();
	GetRootNode()->Render2(renderer);
	GetRenderingProfiler().EndSession();

	// reset status
	renderer->SetBuiltinEffectData(detail::BuiltinEffectData::DefaultData);
}

//------------------------------------------------------------------------------
bool SceneGraph::InjectMouseMove(int x, int y)
{
	// シェーダ系
	m_mousePosition.Set(x, y);

	// カメラ
	for (CameraComponent* camera : m_allCameraList) {
		if (camera->GetCameraBehavior() != nullptr) {
			camera->GetCameraBehavior()->InjectMouseMove(x, y);
		}
	}
	return false;
}

//------------------------------------------------------------------------------
bool SceneGraph::InjectMouseButtonDown(MouseButtons button, int x, int y)
{
	// TODO: マウス位置はSceneGraphよりも Viewport(CameraLayer)に付くようにしたい。
	// プレビュー用のUIElementに描きたいときどうするの？という話。
	// なお、offscreenrendertarget については特にサポートしない。というか無理。
	
	// シェーダ系
	switch (button)
	{
	case MouseButtons::Left:
		m_leftMouseState.position = m_mousePosition;
		m_leftMouseState.time = static_cast<float>(m_time);
		m_leftMouseState.isDown = true;
		break;
	case MouseButtons::Right:
		m_rightMouseState.position = m_mousePosition;
		m_rightMouseState.time = static_cast<float>(m_time);
		m_rightMouseState.isDown = true;
		break;
	case MouseButtons::Middle:
		m_middleMouseState.position = m_mousePosition;
		m_middleMouseState.time = static_cast<float>(m_time);
		m_middleMouseState.isDown = true;
		break;
	default:
		break;
	}

	// カメラ
	for (CameraComponent* camera : m_allCameraList) {
		if (camera->GetCameraBehavior() != nullptr) {
			camera->GetCameraBehavior()->InjectMouseButtonDown(button, x, y);
		}
	}
	return false;
}

//------------------------------------------------------------------------------
bool SceneGraph::InjectMouseButtonUp(MouseButtons button, int x, int y)
{
	// シェーダ系
	switch (button)
	{
	case MouseButtons::Left:
		m_leftMouseState.isDown = false;
		break;
	case MouseButtons::Right:
		m_leftMouseState.isDown = false;
		break;
	case MouseButtons::Middle:
		m_leftMouseState.isDown = false;
		break;
	default:
		break;
	}
	
	// カメラ
	for (CameraComponent* camera : m_allCameraList) {
		if (camera->GetCameraBehavior() != nullptr) {
			camera->GetCameraBehavior()->InjectMouseButtonUp(button, x, y);
		}
	}
	return false;
}

//------------------------------------------------------------------------------
bool SceneGraph::InjectMouseWheel(int delta)
{
	for (CameraComponent* camera : m_allCameraList) {
		if (camera->GetCameraBehavior() != nullptr) {
			camera->GetCameraBehavior()->InjectMouseWheel(delta);
		}
	}
	return false;
}

//------------------------------------------------------------------------------
DrawList* SceneGraph::GetRenderer() const
{
	return m_renderer;
}

//------------------------------------------------------------------------------
DrawList* SceneGraph::GetDebugRenderer() const
{
	return m_debugRenderer;
}


//==============================================================================
// SceneGraph2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SceneGraph2D, SceneGraph);

//------------------------------------------------------------------------------
SceneGraph2DPtr SceneGraph2D::Create()
{
	auto ptr = SceneGraph2DPtr::MakeRef();
	ptr->CreateCore(SceneGraphManager::Instance);
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
void SceneGraph2D::CreateCore(SceneGraphManager* manager)
{
	SceneGraph::CreateCore(manager);

	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->Initialize(this);

	m_defaultCamera = LN_NEW CameraComponent();
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
void SceneGraph3D::CreateCore(SceneGraphManager* manager)
{
	SceneGraph::CreateCore(manager);

	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->Initialize(this);

	m_defaultCamera = LN_NEW CameraComponent();
	m_defaultCamera->Initialize(this, CameraProjection_3D);
	m_defaultRoot->AddChild(m_defaultCamera);

	m_defaultLight = RefPtr<LightComponent>::MakeRef();
	m_defaultLight->Initialize(this, LightType_Directional);
	m_defaultRoot->AddChild(m_defaultLight);
}

//------------------------------------------------------------------------------
LightComponent* SceneGraph3D::GetMainLight() const { return m_defaultLight; }

//------------------------------------------------------------------------------
void SceneGraph3D::UpdateFrame(float elapsedTime)
{
	SceneGraph::UpdateFrame(elapsedTime);
	m_defaultRoot->UpdateFrameHierarchy(nullptr, elapsedTime);
}

//------------------------------------------------------------------------------
void SceneGraph3D::Render2(DrawList* renderer, CameraComponent* camera)
{
	SceneGraph::Render2(renderer, camera);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
