﻿
#pragma once

LN_NAMESPACE_BEGIN
namespace detail { class SceneGraphRenderingProfilerInterface; }
class Material;
class DrawList;
class StaticMeshModel;
class CameraComponent;
class Camera;
class Light;
class SceneGraph;
class SceneGraph2D;
class SceneGraph3D;
class PhysicsWorld;
class WorldObject;
class UIEventArgs;
class OffscreenWorldView;
class RenderView;

/** */
LN_ENUM_FLAGS(WorldDebugDrawFlags)
{
	None = 0x0000,
	PhysicsInfo = 0x0001,
};
LN_ENUM_FLAGS_DECLARE(WorldDebugDrawFlags)

/**
	@brief		
*/
class World
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	DrawList* GetRenderer() const;
	DrawList* GetDebugRenderer() const;

	void RemoveAllObjects();

protected:
	//virtual SceneGraph* GetSceneGraph() = 0;

LN_CONSTRUCT_ACCESS:
	World();
	virtual ~World();
	void Initialize();

LN_INTERNAL_ACCESS:
	const RefPtr<DrawList>& GetInsideWorldRenderer() const { return m_insideWorldRenderer; }
	void AddWorldObject(WorldObject* obj, bool autoRelease /*= false*/);
	void RemoveWorldObject(WorldObject* obj);
	void AddOffscreenWorldView(OffscreenWorldView* view);
	void RemoveOffscreenWorldView(OffscreenWorldView* view);
	virtual void BeginUpdateFrame();
	virtual void UpdateFrame(float elapsedTime);
	void RenderRoot(CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags, RenderView* mainRenderView);
	virtual void Render(DrawList* g, CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags, OffscreenWorldView* offscreen = nullptr);
	void ExecuteDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
	virtual void OnUIEvent(UIEventArgs* e);

	List<RefPtr<WorldObject>>			m_rootWorldObjectList;
	RefPtr<DrawList>					m_renderer;
	RefPtr<DrawList>					m_insideWorldRenderer;
	RefPtr<DrawList>					m_debugRenderer;
	RefPtr<Material>					m_debugRendererDefaultMaterial;	// TODO: DebugDrawList みたいに派生させてまとめたほうがいいかな・・・
	List<RefPtr<OffscreenWorldView>>	m_offscreenWorldViewList;
	List<int>							m_offscreenIdStorage;
};

/**
	@brief		
*/
class World2D
	: public World
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	//virtual DrawList* GetRenderer() const override;


protected:
	//virtual SceneGraph* GetSceneGraph() override;
LN_CONSTRUCT_ACCESS:
	World2D();
	virtual ~World2D();
	void Initialize();

LN_INTERNAL_ACCESS:
	SceneGraph2D* GetSceneGraph2D() const;
	Camera* GetMainCamera() const;
	virtual void BeginUpdateFrame() override;
	virtual void UpdateFrame(float elapsedTime) override;
	virtual void Render(DrawList* g, CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags, OffscreenWorldView* offscreen) override;

private:
	RefPtr<SceneGraph2D>		m_sceneGraph;
	RefPtr<Camera>				m_mainCamera;
};

/**
	@brief		
*/
class World3D
	: public World
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	void SetVisibleGridPlane(bool visible) { m_visibleGridPlane = visible; }
	//virtual DrawList* GetRenderer() const override;

protected:
	//virtual SceneGraph* GetSceneGraph() override;

LN_CONSTRUCT_ACCESS:
	World3D();
	virtual ~World3D();
	void Initialize();

LN_INTERNAL_ACCESS:
	PhysicsWorld* GetPhysicsWorld3D() const;
	SceneGraph3D* GetSceneGraph3D() const;
	Camera* GetMainCamera() const;
	virtual void BeginUpdateFrame() override;
	virtual void UpdateFrame(float elapsedTime) override;
	virtual void Render(DrawList* g, CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags, OffscreenWorldView* offscreen) override;

private:
	void CreateGridPlane();
	void RenderGridPlane(DrawList* renderer, CameraComponent* camera);
	void AdjustGridPlane(CameraComponent* camera);

	RefPtr<PhysicsWorld>		m_physicsWorld;
	RefPtr<SceneGraph3D>		m_sceneGraph;
	RefPtr<Camera>				m_mainCamera;
	RefPtr<Light>				m_mainLight;
	RefPtr<StaticMeshModel>		m_gridPlane;
	bool						m_visibleGridPlane;
};

LN_NAMESPACE_END
