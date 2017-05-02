
#pragma once

LN_NAMESPACE_BEGIN
class DrawList;
class StaticMeshModel;
class CameraComponent;
class SceneGraph;
class SceneGraph2D;
class SceneGraph3D;
class PhysicsWorld;

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


protected:
	virtual SceneGraph* GetSceneGraph() = 0;

LN_CONSTRUCT_ACCESS:
	World();
	virtual ~World();
	void Initialize();

LN_INTERNAL_ACCESS:
	virtual void BeginUpdateFrame();
	virtual void UpdateFrame(float elapsedTime);
	virtual void Render(CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags);
	void ExecuteDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
};

/**
	@brief		
*/
class World2D
	: public World
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

protected:
	virtual SceneGraph* GetSceneGraph() override;

LN_CONSTRUCT_ACCESS:
	World2D();
	virtual ~World2D();
	void Initialize();

LN_INTERNAL_ACCESS:
	SceneGraph2D* GetSceneGraph2D() const;
	virtual void BeginUpdateFrame() override;
	virtual void UpdateFrame(float elapsedTime) override;

private:
	RefPtr<SceneGraph2D>		m_sceneGraph;
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

protected:
	virtual SceneGraph* GetSceneGraph() override;

LN_CONSTRUCT_ACCESS:
	World3D();
	virtual ~World3D();
	void Initialize();

LN_INTERNAL_ACCESS:
	PhysicsWorld* GetPhysicsWorld3D() const;
	SceneGraph3D* GetSceneGraph3D() const;
	virtual void BeginUpdateFrame() override;
	virtual void UpdateFrame(float elapsedTime) override;
	virtual void Render(CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags);

private:
	void CreateGridPlane();
	void RenderGridPlane(DrawList* renderer, CameraComponent* camera);
	void AdjustGridPlane(CameraComponent* camera);

	RefPtr<PhysicsWorld>	m_physicsWorld;
	RefPtr<SceneGraph3D>	m_sceneGraph;
	RefPtr<StaticMeshModel>	m_gridPlane;
	bool					m_visibleGridPlane;
};

LN_NAMESPACE_END
