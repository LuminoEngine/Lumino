
#pragma once
#include "Rendering/Rendering.h"	// TODO: for WorldRenderViewBase
#include "UI/UIEvent.h"

LN_NAMESPACE_BEGIN
namespace detail { class SceneGraphRenderingProfilerInterface; }
class CommonMaterial;
class DrawList;
class StaticMeshModel;
class CameraComponent;
class Camera;
class Light;
class Fog;
class SceneGraph;
class SceneGraph2D;
class SceneGraph3D;
class PhysicsWorld;
class WorldObject;
class UIEventArgs;
class OffscreenWorldSubRenderView;
class RenderView;
class RenderingContext;
class WorldRenderViewBase;

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
	LN_OBJECT;
public:

	/** オブジェクトを World に追加します。(戻り値は obj) */
	WorldObject* add(WorldObject* obj);

	/** 空のオブジェクトを作成して World に追加します。 */
	WorldObject* addEmptyObject();

	/** オブジェクトを World から除外します。 */
	void remove(WorldObject* obj);

	/** World に含まれている全てのオブジェクトを World から除外します。 */
	void removeAllObjects();

	DrawList* getRenderer() const;
	DrawList* GetDebugRenderer() const;


	void addOffscreenWorldView(OffscreenWorldSubRenderView* view);
	void removeOffscreenWorldView(OffscreenWorldSubRenderView* view);
protected:
	//virtual SceneGraph* GetSceneGraph() = 0;

LN_CONSTRUCT_ACCESS:
	World();
	virtual ~World();
	void initialize();

LN_INTERNAL_ACCESS:
	//const Ref<DrawList>& getInsideWorldRenderer() const { return m_insideWorldRenderer; }
	virtual void beginUpdateFrame();
	void updateFrame(float deltaSceonds);

	// update sequence
	virtual void onPreUpdate(float deltaSceonds);
	virtual void onInternalPhysicsUpdate(float deltaSceonds);
	virtual void onUpdate(float deltaSceonds);
	virtual void onInternalAnimationUpdate(float deltaSceonds);
	virtual void onPostUpdate(float deltaSceonds);

	void renderRoot(WorldRenderViewBase* renderView, WorldDebugDrawFlags debugDrawFlags);
	virtual void render(RenderingContext* context, WorldRenderViewBase* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldSubRenderView* offscreen = nullptr);
	void executeDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
	virtual void onUIEvent(UIEventArgs* e);	// この World をホストする UIViewport のイベントが流れてくる

	EventConnection connectOnUIEvent(UIEventHandler handler);
	void updateObjectsWorldMatrix();


	List<Ref<WorldObject>>			m_rootWorldObjectList;
	Ref<RenderingContext>			m_renderer;
	//Ref<DrawList>					m_insideWorldRenderer;
	Ref<DrawList>					m_debugRenderer;
	Ref<CommonMaterial>					m_debugRendererDefaultMaterial;	// TODO: DebugDrawList みたいに派生させてまとめたほうがいいかな・・・
	List<Ref<OffscreenWorldSubRenderView>>	m_offscreenWorldViewList;
	List<int>							m_offscreenIdStorage;

	UIEventHandler::EventType			m_onEvent;

private:
	void addWorldObject(WorldObject* obj, bool autoRelease /*= false*/);
	void removeWorldObject(WorldObject* obj);
};

/**
	@brief		
*/
class World2D
	: public World
{
	LN_OBJECT;
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
	virtual void beginUpdateFrame() override;
	virtual void onUpdate(float deltaSceonds) override;
	virtual void render(RenderingContext* context, WorldRenderViewBase* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldSubRenderView* offscreen) override;

private:
	Ref<Camera>				m_mainCamera;
};

/** */
class World3D
	: public World
{
	LN_OBJECT;
public:
	void setVisibleGridPlane(bool visible) { m_visibleGridPlane = visible; }
	//virtual DrawList* getRenderer() const override;

	/** フォグの色を取得します。 */
	const Color& getFogColor() { return m_globalRenderSettings.fogColor; }

	/** フォグの色を設定します。(default: Color(1, 1, 1, 1)) */
	void setFogColor(const Color& color) { m_globalRenderSettings.fogColor = color; }
	
	/** フォグの密度指数を取得します。 */
	float getFogDensity() { return m_globalRenderSettings.fogDensity; }

	/** フォグの密度指数を設定します。(default: 0) */
	void setFogDensity(float density) { m_globalRenderSettings.fogDensity = density; }

	Fog* getFog() const;
	void setFog(Fog* fog);

protected:
	//virtual SceneGraph* GetSceneGraph() override;

LN_CONSTRUCT_ACCESS:
	World3D();
	virtual ~World3D();
	void initialize();

LN_INTERNAL_ACCESS:
	PhysicsWorld* getPhysicsWorld3D() const;
	Camera* getMainCamera() const;
	const detail::SceneGlobalRenderSettings& getGlobalRenderSettings() const { return m_globalRenderSettings; }
	virtual void beginUpdateFrame() override;
	virtual void onInternalPhysicsUpdate(float deltaSceonds) override;
	virtual void render(RenderingContext* context, WorldRenderViewBase* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldSubRenderView* offscreen) override;


private:
	void createGridPlane();
	void renderGridPlane(DrawList* renderer, RenderView* renderView);
	void adjustGridPlane(RenderView* renderView);

	Ref<PhysicsWorld>		m_physicsWorld;
	detail::SceneGlobalRenderSettings	m_globalRenderSettings;
	Ref<Camera>				m_mainCamera;
	Ref<Light>				m_mainLight;
	Ref<Fog>				m_fog;
	Ref<StaticMeshModel>		m_gridPlane;
	bool						m_visibleGridPlane;
};

LN_NAMESPACE_END
