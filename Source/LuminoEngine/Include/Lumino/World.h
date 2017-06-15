
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

	DrawList* getRenderer() const;
	DrawList* GetDebugRenderer() const;

	void RemoveAllObjects();

protected:
	//virtual SceneGraph* GetSceneGraph() = 0;

LN_CONSTRUCT_ACCESS:
	World();
	virtual ~World();
	void initialize();

LN_INTERNAL_ACCESS:
	const RefPtr<DrawList>& getInsideWorldRenderer() const { return m_insideWorldRenderer; }
	void addWorldObject(WorldObject* obj, bool autoRelease /*= false*/);
	void removeWorldObject(WorldObject* obj);
	void addOffscreenWorldView(OffscreenWorldView* view);
	void removeOffscreenWorldView(OffscreenWorldView* view);
	virtual void reginUpdateFrame();
	virtual void updateFrame(float elapsedTime);
	void renderRoot(CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags, RenderView* mainRenderView);
	virtual void render(DrawList* g, CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags, OffscreenWorldView* offscreen = nullptr);
	void executeDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
	virtual void onUIEvent(UIEventArgs* e);

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
	//virtual DrawList* getRenderer() const override;


protected:
	//virtual SceneGraph* GetSceneGraph() override;
LN_CONSTRUCT_ACCESS:
	World2D();
	virtual ~World2D();
	void initialize();

LN_INTERNAL_ACCESS:
	SceneGraph2D* getSceneGraph2D() const;
	Camera* getMainCamera() const;
	virtual void reginUpdateFrame() override;
	virtual void updateFrame(float elapsedTime) override;
	virtual void render(DrawList* g, CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags, OffscreenWorldView* offscreen) override;

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
	//virtual DrawList* getRenderer() const override;

protected:
	//virtual SceneGraph* GetSceneGraph() override;

LN_CONSTRUCT_ACCESS:
	World3D();
	virtual ~World3D();
	void initialize();

LN_INTERNAL_ACCESS:
	PhysicsWorld* getPhysicsWorld3D() const;
	SceneGraph3D* getSceneGraph3D() const;
	Camera* getMainCamera() const;
	virtual void reginUpdateFrame() override;
	virtual void updateFrame(float elapsedTime) override;
	virtual void render(DrawList* g, CameraComponent* camera, WorldDebugDrawFlags debugDrawFlags, OffscreenWorldView* offscreen) override;

private:
	void createGridPlane();
	void renderGridPlane(DrawList* renderer, CameraComponent* camera);
	void adjustGridPlane(CameraComponent* camera);

	RefPtr<PhysicsWorld>		m_physicsWorld;
	RefPtr<SceneGraph3D>		m_sceneGraph;
	RefPtr<Camera>				m_mainCamera;
	RefPtr<Light>				m_mainLight;
	RefPtr<StaticMeshModel>		m_gridPlane;
	bool						m_visibleGridPlane;
};

LN_NAMESPACE_END
