
#include "Internal.h"
#include <Lumino/Physics/PhysicsWorld.h>
#include <Lumino/Rendering/RenderingContext.h>
#include <Lumino/Mesh/Mesh.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/Light.h>
#include <Lumino/World.h>
#include <Lumino/Scene/WorldObject.h>
#include <Lumino/Scene/OffscreenWorldView.h>
#include "Graphics/GraphicsManager.h"
#include "EngineManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// WorldRenderView
//==============================================================================
//------------------------------------------------------------------------------
WorldRenderView::WorldRenderView()
	: m_layerCullingMask(0)
{
}

//------------------------------------------------------------------------------
WorldRenderView::~WorldRenderView()
{
}

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
void World::initialize()
{
	m_renderer = newObject<RenderingContext>();
	//m_insideWorldRenderer = newObject<DrawList>(detail::EngineDomain::getGraphicsManager());
	m_debugRenderer = newObject<DrawList>(detail::EngineDomain::getGraphicsManager());

	m_debugRendererDefaultMaterial = newObject<Material>();

	for (int i = 0; i < detail::MaxOffscreenId; i++)
	{
		m_offscreenIdStorage.add(i);
	}
}

//------------------------------------------------------------------------------
DrawList* World::getRenderer() const
{
	return m_renderer;
}

//------------------------------------------------------------------------------
DrawList* World::GetDebugRenderer() const
{
	return m_debugRenderer;
}

//------------------------------------------------------------------------------
void World::RemoveAllObjects()
{
	for (int i = m_rootWorldObjectList.getCount() - 1; i >= 0; i--)
	{
		if (!m_rootWorldObjectList[i]->isSpecialObject())
		{
			m_rootWorldObjectList[i]->m_parent = nullptr;
			m_rootWorldObjectList.removeAt(i);
		}
	}
}

//------------------------------------------------------------------------------
void World::addWorldObject(WorldObject* obj, bool autoRelease)
{
	m_rootWorldObjectList.add(obj);
	obj->m_world = this;
	obj->m_isAutoRelease = autoRelease;
	obj->onAttachedWorld(this);
}

//------------------------------------------------------------------------------
void World::removeWorldObject(WorldObject* obj)
{
	if (LN_REQUIRE(obj != nullptr)) return;
	if (LN_REQUIRE(obj->m_world == this)) return;
	m_rootWorldObjectList.remove(obj);
	obj->m_parent = nullptr;
	obj->onDetachedWorld(this);
}

//------------------------------------------------------------------------------
void World::addOffscreenWorldView(OffscreenWorldView* view)
{
	if (LN_REQUIRE(!m_offscreenIdStorage.isEmpty())) return;
	m_offscreenWorldViewList.add(view);

	// assign Id
	view->setId(m_offscreenIdStorage.getLast());
	m_offscreenIdStorage.removeLast();
}

//------------------------------------------------------------------------------
void World::removeOffscreenWorldView(OffscreenWorldView* view)
{
	m_offscreenWorldViewList.remove(view);

	// repay Id.
	m_offscreenIdStorage.add(view->getId());
	view->setId(0);
}

//------------------------------------------------------------------------------
void World::beginUpdateFrame()
{
	// onRender の外のデバッグ描画などでも使用するため、描画リストのクリアは SceneGraph の更新前でなければならない。
	// また、出来上がった描画リストを、複数のビューやカメラが描画することを想定する。
	if (m_renderer != nullptr)
	{
		m_renderer->beginMakeElements();
	}
	if (m_debugRenderer != nullptr)
	{
		m_debugRenderer->beginMakeElements();
		m_debugRendererDefaultMaterial->setShader(detail::EngineDomain::getGraphicsManager()->getBuiltinShader(BuiltinShader::Sprite));
		m_debugRenderer->setDefaultMaterial(m_debugRendererDefaultMaterial);
	}

	for (auto& obj : m_rootWorldObjectList)
	{
		obj->onPreUpdate();
	}
}

void World::updateFrame(float elapsedTime)
{
	onPreUpdate(elapsedTime);
	onInternalPhysicsUpdate(elapsedTime);
	onUpdate(elapsedTime);
	onInternalAnimationUpdate(elapsedTime);
	onPostUpdate(elapsedTime);
}

void World::onPreUpdate(float elapsedTime)
{
}

void World::onInternalPhysicsUpdate(float elapsedTime)
{
}

void World::onUpdate(float elapsedTime)
{
	for (auto& obj : m_rootWorldObjectList)
	{
		obj->updateFrame();
	}
}

void World::onInternalAnimationUpdate(float elapsedTime)
{
}

void World::onPostUpdate(float elapsedTime)
{
}

//------------------------------------------------------------------------------
void World::renderRoot(WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags)
{
	// pre render
	for (auto& view : m_offscreenWorldViewList)
	{
		view->renderWorld(this, renderView);
	}

	// main render
	render(m_renderer, renderView, debugDrawFlags, renderView->getLayerCullingMask());
}

//------------------------------------------------------------------------------
void World::render(RenderingContext* context, WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldView* offscreen)
{
	LN_ASSERT(context->getRenderView() == nullptr);	// render 下はネスト禁止
	context->setRenderView(renderView);
	
	for (auto& obj : m_rootWorldObjectList)
	{
		obj->render(context);

		for (auto& c : obj->m_components)
		{
			c->onPreRender(context);

			VisualComponent* visual = nullptr;
			if (c->getSpecialComponentType() == SpecialComponentType::Visual)
			{
				visual = static_cast<VisualComponent*>(c.get());

				bool visible = LayerMask::filterComponent(c.get(), layerMask);
				if (visible && offscreen != nullptr) visible = offscreen->filterRenderObject(visual);

				if (visible)
				{
					c->render(context);
				}
			}
		}
	}

	// reset status
	context->setBuiltinEffectData(detail::BuiltinEffectData::DefaultData);

	context->setRenderView(nullptr);
}

//------------------------------------------------------------------------------
void World::executeDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
}

//------------------------------------------------------------------------------
void World::onUIEvent(UIEventArgs* e)
{
	for (auto& obj : m_rootWorldObjectList)
	{
		obj->onUIEvent(e);
		if (e->handled) return;
	}

	m_onEvent.raise(e);
}

//------------------------------------------------------------------------------
EventConnection World::connectOnUIEvent(UIEventHandler handler)
{
	return m_onEvent.connect(handler);
}

void World::updateObjectsWorldMatrix()
{
	for (auto& obj : m_rootWorldObjectList)
	{
		obj->updateWorldMatrixHierarchical();
	}
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
void World2D::initialize()
{
	World::initialize();

	m_sceneGraph = Ref<SceneGraph2D>::makeRef();
	m_sceneGraph->createCore(EngineManager::getInstance()->getSceneGraphManager());

	m_mainCamera = newObject<Camera>(CameraProjection_2D);
	m_mainCamera->setSpecialObject(true);
	addWorldObject(m_mainCamera, true);
}

//------------------------------------------------------------------------------
//SceneGraph* World2D::GetSceneGraph()
//{
//	return getSceneGraph2D();
//}

////------------------------------------------------------------------------------
//DrawList* World2D::getRenderer() const
//{
//	return getSceneGraph2D()->getRenderer();
//}

//------------------------------------------------------------------------------
SceneGraph2D* World2D::getSceneGraph2D() const
{
	return m_sceneGraph;
}

//------------------------------------------------------------------------------
Camera* World2D::getMainCamera() const
{
	return m_mainCamera;
}

//------------------------------------------------------------------------------
//detail::SceneGraphRenderingProfilerInterface& World2D::GetRenderingProfiler()
//{
//	return m_sceneGraph->GetRenderingProfiler();
//}

//------------------------------------------------------------------------------
void World2D::beginUpdateFrame()
{
	World::beginUpdateFrame();

	//if (m_sceneGraph != nullptr) {
	//	m_sceneGraph->beginUpdateFrame();
	//}
}

//------------------------------------------------------------------------------
void World2D::onUpdate(float elapsedTime)
{
	World::onUpdate(elapsedTime);
}

//------------------------------------------------------------------------------
void World2D::render(RenderingContext* context, WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldView* offscreen)
{
	World::render(context, renderView, debugDrawFlags, layerMask, offscreen);
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
void World3D::initialize()
{
	World::initialize();

	m_physicsWorld = newObject<PhysicsWorld>();

	m_sceneGraph = Ref<SceneGraph3D>::makeRef();
	m_sceneGraph->createCore(EngineManager::getInstance()->getSceneGraphManager());

	m_mainCamera = newObject<Camera>(CameraProjection_3D);
	m_mainCamera->setSpecialObject(true);
	addWorldObject(m_mainCamera, true);

	m_mainLight = newObject<Light>();
	m_mainLight->setSpecialObject(true);
	addWorldObject(m_mainLight, true);

	createGridPlane();
}

//------------------------------------------------------------------------------
//SceneGraph* World3D::GetSceneGraph()
//{
//	return getSceneGraph3D();
//}

////------------------------------------------------------------------------------
//DrawList* World3D::getRenderer() const
//{
//	return getSceneGraph3D()->getRenderer();
//}
//
//------------------------------------------------------------------------------
PhysicsWorld* World3D::getPhysicsWorld3D() const
{
	return m_physicsWorld;
}

//------------------------------------------------------------------------------
SceneGraph3D* World3D::getSceneGraph3D() const
{
	return m_sceneGraph;
}

//------------------------------------------------------------------------------
Camera* World3D::getMainCamera() const
{
	return m_mainCamera;
}

//------------------------------------------------------------------------------
//detail::SceneGraphRenderingProfilerInterface& World3D::GetRenderingProfiler()
//{
//	return m_sceneGraph->GetRenderingProfiler();
//}

//------------------------------------------------------------------------------
void World3D::beginUpdateFrame()
{
	World::beginUpdateFrame();

	//if (m_sceneGraph != nullptr)
	//{
	//	m_sceneGraph->beginUpdateFrame();
	//}
}

//------------------------------------------------------------------------------
void World3D::onInternalPhysicsUpdate(float elapsedTime)
{
	// Physics モジュールの Component が、WorldObject の WorldMatrix を元にシミュレーション前準備を行うことがあるので
	// ここで WorldMatrix を更新しておく。
	updateObjectsWorldMatrix();

	if (m_physicsWorld != nullptr)
	{
		m_physicsWorld->stepSimulation(elapsedTime);
	}
}

//------------------------------------------------------------------------------
void World3D::render(RenderingContext* context, WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldView* offscreen)
{
	World::render(context, renderView, debugDrawFlags, layerMask, offscreen);

	renderGridPlane(context, renderView);

	if (debugDrawFlags.TestFlag(WorldDebugDrawFlags::PhysicsInfo))
	{
		if (m_physicsWorld != nullptr)
		{
			class DebugRenderer : public PhysicsWorld::IDebugRenderer
			{
			public:
				DrawList* renderer;

				virtual void drawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor)
				{
					renderer->drawLinePrimitive(from, Color(fromColor, 1.0f), to, Color(toColor, 1.0f));
				}
			};
			DebugRenderer r;
			r.renderer = GetDebugRenderer();

			m_physicsWorld->drawDebugShapes(&r);
		}
	}
}


//------------------------------------------------------------------------------
void World3D::createGridPlane()
{
	detail::GraphicsManager* gm = detail::EngineDomain::getGraphicsManager();

	// 適当な四角形メッシュ
	m_gridPlane = Ref<StaticMeshModel>::makeRef();
	m_gridPlane->initializeScreenPlane(gm, MeshCreationFlags::DynamicBuffers);
	MeshResource* mesh = m_gridPlane->getMeshResource(0);
	mesh->addSections(1);
	mesh->getSection(0)->MaterialIndex = 0;
	mesh->getSection(0)->StartIndex = 0;
	mesh->getSection(0)->PrimitiveNum = 2;
	mesh->getSection(0)->primitiveType = PrimitiveType_TriangleList;

	static const byte_t shaderCode[] =
	{
#include "Scene/Resource/InfinitePlaneGrid.lfx.h"
	};
	static const size_t shaderCodeLen = LN_ARRAY_SIZE_OF(shaderCode);
	auto shader = Ref<Shader>::makeRef();
	shader->initialize(gm, shaderCode, shaderCodeLen);
	m_gridPlane->getMaterial(0)->setShader(shader);

	// 四方の辺に黒線を引いたテクスチャを作り、マテリアルにセットしておく
	SizeI gridTexSize(512, 512);
	auto gridTex = newObject<Texture2D>(gridTexSize, TextureFormat::R8G8B8A8, true, ResourceUsage::Static);
	for (int x = 0; x < gridTexSize.width; ++x)
	{
		gridTex->setPixel(x, 0, Color(0, 0, 0, 0.5));
		gridTex->setPixel(x, gridTexSize.width - 1, Color(0, 0, 0, 0.5));
	}
	for (int y = 0; y < gridTexSize.height; ++y)
	{
		gridTex->setPixel(0, y, Color(0, 0, 0, 0.5));
		gridTex->setPixel(gridTexSize.height - 1, y, Color(0, 0, 0, 0.5));
	}
	m_gridPlane->getMaterial(0)->setMaterialTexture(gridTex);

	m_gridPlane->getMaterial(0)->setBlendMode(BlendMode::Alpha);
	m_gridPlane->getMaterial(0)->setDepthWriteEnabled(false);
}

//------------------------------------------------------------------------------
void World3D::renderGridPlane(DrawList* renderer, RenderView* renderView)
{
	if (m_visibleGridPlane)
	{
		adjustGridPlane(renderView);
		renderer->setTransform(Matrix::Identity);

		DrawElementMetadata metadata;
		metadata.priority = (int)DepthPriority::Foreground;
		renderer->pushMetadata(&metadata);
		renderer->drawMesh(m_gridPlane->getMeshResource(0), 0, m_gridPlane->getMaterial(0));
		renderer->popMetadata();
	}
}

//------------------------------------------------------------------------------
void World3D::adjustGridPlane(RenderView* renderView)
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

	auto& vf = renderView->m_cameraInfo.viewFrustum;
	Vector3 points[8];
	vf.getCornerPoints(points);

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
		if (plane.intersects(li.from, li.to, &pt))
		{
			hits.add(pt);
		}
	}

	Vector3 minPos, maxPos;
	for (Vector3& p : hits)
	{
		minPos = Vector3::min(p, minPos);
		maxPos = Vector3::max(p, maxPos);
	}

	MeshResource* mesh = m_gridPlane->getMeshResource(0);
	mesh->setPosition(0, Vector3(minPos.x, 0, maxPos.z));
	mesh->setPosition(1, Vector3(minPos.x, 0, minPos.z));
	mesh->setPosition(2, Vector3(maxPos.x, 0, maxPos.z));
	mesh->setPosition(3, Vector3(maxPos.x, 0, minPos.z));
	mesh->setUV(0, Vector2(-1.0f, 1.0f));
	mesh->setUV(1, Vector2(-1.0f, -1.0f));
	mesh->setUV(2, Vector2(1.0f, 1.0f));
	mesh->setUV(3, Vector2(1.0f, -1.0f));
	mesh->m_attributes[0].PrimitiveNum = 2;
}

LN_NAMESPACE_END
