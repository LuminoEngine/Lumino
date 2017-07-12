
#pragma once
#include "Rendering/Rendering.h"	// TODO: for WorldRenderView
#include "UI/UIEvent.h"

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
class RenderingContext;

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
class WorldRenderView
	: public RenderView
{
public:
	void setLayerCullingMask(uint32_t mask) { m_layerCullingMask = mask; }
	uint32_t getLayerCullingMask() const { return m_layerCullingMask; }
	
LN_CONSTRUCT_ACCESS:
	WorldRenderView();
	virtual ~WorldRenderView();

private:
	uint32_t	m_layerCullingMask;
};

/**
	@brief		
*/
class World
	: public Object
{
	LN_OBJECT();
public:

	DrawList* getRenderer() const;
	DrawList* GetDebugRenderer() const;

	void RemoveAllObjects();

	void addWorldObject(WorldObject* obj, bool autoRelease /*= false*/);
	void removeWorldObject(WorldObject* obj);
protected:
	//virtual SceneGraph* GetSceneGraph() = 0;

LN_CONSTRUCT_ACCESS:
	World();
	virtual ~World();
	void initialize();

LN_INTERNAL_ACCESS:
	//const RefPtr<DrawList>& getInsideWorldRenderer() const { return m_insideWorldRenderer; }
	void addOffscreenWorldView(OffscreenWorldView* view);
	void removeOffscreenWorldView(OffscreenWorldView* view);
	virtual void reginUpdateFrame();
	virtual void updateFrame(float elapsedTime);

	void renderRoot(WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags);
	virtual void render(RenderingContext* context, WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldView* offscreen = nullptr);
	void executeDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
	virtual void onUIEvent(UIEventArgs* e);	// この World をホストする UIViewport のイベントが流れてくる

	EventConnection connectOnUIEvent(UIEventHandler handler);


	List<RefPtr<WorldObject>>			m_rootWorldObjectList;
	RefPtr<RenderingContext>			m_renderer;
	//RefPtr<DrawList>					m_insideWorldRenderer;
	RefPtr<DrawList>					m_debugRenderer;
	RefPtr<Material>					m_debugRendererDefaultMaterial;	// TODO: DebugDrawList みたいに派生させてまとめたほうがいいかな・・・
	List<RefPtr<OffscreenWorldView>>	m_offscreenWorldViewList;
	List<int>							m_offscreenIdStorage;

	UIEventHandler::EventType			m_onEvent;
};

/**
	@brief		
*/
class World2D
	: public World
{
	LN_OBJECT();
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
	virtual void render(RenderingContext* context, WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldView* offscreen) override;

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
	LN_OBJECT();
public:
	void setVisibleGridPlane(bool visible) { m_visibleGridPlane = visible; }
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
	virtual void render(RenderingContext* context, WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldView* offscreen) override;

private:
	void createGridPlane();
	void renderGridPlane(DrawList* renderer, RenderView* renderView);
	void adjustGridPlane(RenderView* renderView);

	RefPtr<PhysicsWorld>		m_physicsWorld;
	RefPtr<SceneGraph3D>		m_sceneGraph;
	RefPtr<Camera>				m_mainCamera;
	RefPtr<Light>				m_mainLight;
	RefPtr<StaticMeshModel>		m_gridPlane;
	bool						m_visibleGridPlane;
};

LN_NAMESPACE_END
