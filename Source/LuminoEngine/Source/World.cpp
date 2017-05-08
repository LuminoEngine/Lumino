
#include "Internal.h"
#include <Lumino/Physics/PhysicsWorld.h>
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Mesh/Mesh.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/World.h>
#include <Lumino/Scene/WorldObject.h>
#include "Graphics/GraphicsManager.h"
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
	m_renderer = NewObject<DrawList>(detail::EngineDomain::GetGraphicsManager());
	m_debugRenderer = NewObject<DrawList>(detail::EngineDomain::GetGraphicsManager());

	m_debugRendererDefaultMaterial = NewObject<Material>();
}

//------------------------------------------------------------------------------
DrawList* World::GetRenderer() const
{
	return m_renderer;
}

//------------------------------------------------------------------------------
DrawList* World::GetDebugRenderer() const
{
	return m_debugRenderer;
}

//------------------------------------------------------------------------------
void World::AddWorldObject(WorldObject* obj, bool autoRelease)
{
	m_rootWorldObjectList.Add(obj);
	obj->m_isAutoRelease = autoRelease;
}

//------------------------------------------------------------------------------
void World::BeginUpdateFrame()
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
		m_debugRendererDefaultMaterial->SetShader(detail::EngineDomain::GetGraphicsManager()->GetBuiltinShader(BuiltinShader::Sprite));
		m_debugRenderer->SetDefaultMaterial(m_debugRendererDefaultMaterial);
	}
}

//------------------------------------------------------------------------------
void World::UpdateFrame(float elapsedTime)
{
	for (auto& obj : m_rootWorldObjectList)
	{
		obj->UpdateFrame();
	}
}

//------------------------------------------------------------------------------
void World::Render(CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags)
{
	for (auto& obj : m_rootWorldObjectList)
	{
		obj->Render(m_renderer);
	}

	// reset status
	m_renderer->SetBuiltinEffectData(detail::BuiltinEffectData::DefaultData);
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

	m_mainCamera = NewObject<Camera>(m_sceneGraph, CameraProjection_2D);
	AddWorldObject(m_mainCamera, true);
}

//------------------------------------------------------------------------------
//SceneGraph* World2D::GetSceneGraph()
//{
//	return GetSceneGraph2D();
//}

////------------------------------------------------------------------------------
//DrawList* World2D::GetRenderer() const
//{
//	return GetSceneGraph2D()->GetRenderer();
//}

//------------------------------------------------------------------------------
SceneGraph2D* World2D::GetSceneGraph2D() const
{
	return m_sceneGraph;
}

//------------------------------------------------------------------------------
Camera* World2D::GetMainCamera() const
{
	return m_mainCamera;
}

//------------------------------------------------------------------------------
//detail::SceneGraphRenderingProfilerInterface& World2D::GetRenderingProfiler()
//{
//	return m_sceneGraph->GetRenderingProfiler();
//}

//------------------------------------------------------------------------------
void World2D::BeginUpdateFrame()
{
	World::BeginUpdateFrame();

	//if (m_sceneGraph != nullptr) {
	//	m_sceneGraph->BeginUpdateFrame();
	//}
}

//------------------------------------------------------------------------------
void World2D::UpdateFrame(float elapsedTime)
{
	World::UpdateFrame(elapsedTime);

	//if (m_sceneGraph != nullptr) {
	//	m_sceneGraph->UpdateFrame(elapsedTime);
	//}
}

//------------------------------------------------------------------------------
void World2D::Render(CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags)
{
	World::Render(camera, debugDrawFlags);

	m_sceneGraph->Render2(GetRenderer(), camera);
}

//==============================================================================
// World3D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(World3D, World);

//------------------------------------------------------------------------------
World3D::World3D()
	: m_visibleGridPlane(false)
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

	m_mainCamera = NewObject<Camera>(m_sceneGraph, CameraProjection_3D);
	AddWorldObject(m_mainCamera, true);

	CreateGridPlane();
}

//------------------------------------------------------------------------------
//SceneGraph* World3D::GetSceneGraph()
//{
//	return GetSceneGraph3D();
//}

////------------------------------------------------------------------------------
//DrawList* World3D::GetRenderer() const
//{
//	return GetSceneGraph3D()->GetRenderer();
//}
//
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
Camera* World3D::GetMainCamera() const
{
	return m_mainCamera;
}

//------------------------------------------------------------------------------
//detail::SceneGraphRenderingProfilerInterface& World3D::GetRenderingProfiler()
//{
//	return m_sceneGraph->GetRenderingProfiler();
//}

//------------------------------------------------------------------------------
void World3D::BeginUpdateFrame()
{
	World::BeginUpdateFrame();

	//if (m_sceneGraph != nullptr)
	//{
	//	m_sceneGraph->BeginUpdateFrame();
	//}
}

//------------------------------------------------------------------------------
void World3D::UpdateFrame(float elapsedTime)
{
	World::UpdateFrame(elapsedTime);

	if (m_physicsWorld != nullptr)
	{
		m_physicsWorld->StepSimulation(elapsedTime);
	}

	//if (m_sceneGraph != nullptr)
	//{
	//	m_sceneGraph->UpdateFrame(elapsedTime);
	//}
}

//------------------------------------------------------------------------------
void World3D::Render(CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags)
{
	World::Render(camera, debugDrawFlags);

	m_sceneGraph->Render2(GetRenderer(), camera);

	RenderGridPlane(GetRenderer(), camera);

	if (debugDrawFlags.TestFlag(WorldDebugDrawFlags::PhysicsInfo))
	{
		if (m_physicsWorld != nullptr)
		{
			class DebugRenderer : public PhysicsWorld::IDebugRenderer
			{
			public:
				DrawList* renderer;

				virtual void DrawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor)
				{
					renderer->DrawLinePrimitive(from, Color(fromColor, 1.0f), to, Color(toColor, 1.0f));
				}
			};
			DebugRenderer r;
			r.renderer = GetDebugRenderer();

			m_physicsWorld->DrawDebugShapes(&r);
		}
	}
}

//------------------------------------------------------------------------------
void World3D::CreateGridPlane()
{
	detail::GraphicsManager* gm = detail::EngineDomain::GetGraphicsManager();

	// 適当な四角形メッシュ
	m_gridPlane = RefPtr<StaticMeshModel>::MakeRef();
	m_gridPlane->InitializeScreenPlane(gm, MeshCreationFlags::DynamicBuffers);
	MeshResource* mesh = m_gridPlane->GetMeshResource();
	mesh->AddSections(1);
	mesh->GetSection(0)->MaterialIndex = 0;
	mesh->GetSection(0)->StartIndex = 0;
	mesh->GetSection(0)->PrimitiveNum = 2;
	mesh->GetSection(0)->primitiveType = PrimitiveType_TriangleList;

	static const byte_t shaderCode[] =
	{
#include "Scene/Resource/InfinitePlaneGrid.lfx.h"
	};
	static const size_t shaderCodeLen = LN_ARRAY_SIZE_OF(shaderCode);
	auto shader = RefPtr<Shader>::MakeRef();
	shader->Initialize(gm, shaderCode, shaderCodeLen);
	m_gridPlane->GetMaterial(0)->SetShader(shader);

	// 四方の辺に黒線を引いたテクスチャを作り、マテリアルにセットしておく
	SizeI gridTexSize(512, 512);
	auto gridTex = NewObject<Texture2D>(gridTexSize, TextureFormat::R8G8B8A8, true, ResourceUsage::Static);
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
	m_gridPlane->GetMaterial(0)->SetMaterialTexture(gridTex);

	m_gridPlane->GetMaterial(0)->SetBlendMode(BlendMode::Alpha);
	m_gridPlane->GetMaterial(0)->SetDepthWriteEnabled(false);
}

//------------------------------------------------------------------------------
void World3D::RenderGridPlane(DrawList* renderer, CameraComponent* camera)
{
	if (m_visibleGridPlane)
	{
		AdjustGridPlane(camera);
		renderer->SetTransform(Matrix::Identity);

		DrawElementMetadata metadata;
		metadata.priority = (int)DepthPriority::Foreground;
		renderer->PushMetadata(&metadata);
		renderer->DrawMesh(m_gridPlane, 0, m_gridPlane->GetMaterial(0));
		renderer->PopMetadata();
	}
}

//------------------------------------------------------------------------------
void World3D::AdjustGridPlane(CameraComponent* camera)
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
		// near
		{ points[0], points[1] },
		{ points[1], points[2] },
		{ points[2], points[3] },
		{ points[3], points[0] },
		// near - far
		{ points[0], points[4] },
		{ points[1], points[5] },
		{ points[2], points[6] },
		{ points[3], points[7] },
		// far
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
	mesh->m_attributes[0].PrimitiveNum = 2;
}

LN_NAMESPACE_END
