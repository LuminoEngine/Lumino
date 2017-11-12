
#pragma once
#include "Rendering/Rendering.h"	// TODO: for WorldRenderView
#include "UI/UIEvent.h"

LN_NAMESPACE_BEGIN
namespace detail { class SceneGraphRenderingProfilerInterface; }
class CommonMaterial;
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
class WorldRenderView;

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

	DrawList* getRenderer() const;
	DrawList* GetDebugRenderer() const;

	void RemoveAllObjects();

	void addWorldObject(WorldObject* obj, bool autoRelease /*= false*/);
	void removeWorldObject(WorldObject* obj);
	void addOffscreenWorldView(OffscreenWorldView* view);
	void removeOffscreenWorldView(OffscreenWorldView* view);
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

	void renderRoot(WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags);
	virtual void render(RenderingContext* context, WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldView* offscreen = nullptr);
	void executeDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
	virtual void onUIEvent(UIEventArgs* e);	// この World をホストする UIViewport のイベントが流れてくる

	EventConnection connectOnUIEvent(UIEventHandler handler);
	void updateObjectsWorldMatrix();


	List<Ref<WorldObject>>			m_rootWorldObjectList;
	Ref<RenderingContext>			m_renderer;
	//Ref<DrawList>					m_insideWorldRenderer;
	Ref<DrawList>					m_debugRenderer;
	Ref<CommonMaterial>					m_debugRendererDefaultMaterial;	// TODO: DebugDrawList みたいに派生させてまとめたほうがいいかな・・・
	List<Ref<OffscreenWorldView>>	m_offscreenWorldViewList;
	List<int>							m_offscreenIdStorage;

	UIEventHandler::EventType			m_onEvent;

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
	virtual void render(RenderingContext* context, WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldView* offscreen) override;

private:
	Ref<SceneGraph2D>		m_sceneGraph;
	Ref<Camera>				m_mainCamera;
};

/**
	@brief		
*/
class World3D
	: public World
{
	LN_OBJECT;
public:
	void setVisibleGridPlane(bool visible) { m_visibleGridPlane = visible; }
	//virtual DrawList* getRenderer() const override;

	/** 環境光の色を取得します。 */
	const Color& getAmbientColor() { return m_globalRenderSettings.ambientColor; }

	/** 環境光の色を設定します。アルファ値を影響度とし、シーン全体のオブジェクトへ一様に影響します。(default: Color(0.25, 0.25, 0.25, 1.0)) */
	void setAmbientColor(const Color& color) { m_globalRenderSettings.ambientColor = color; }

	/** 半球ライティングのための空の環境光の色を取得します。 */
	const Color& getAmbientSkyColor() { return m_globalRenderSettings.ambientSkyColor; }

	/** 半球ライティングのための空の環境光の色を設定します。(default: Color(0, 0, 0, 0)) */
	void setAmbientSkyColor(const Color& color) { m_globalRenderSettings.ambientSkyColor = color; }

	/** 半球ライティングのための地面の環境光の色を取得します。 */
	const Color& getAmbientGroundColor() { return m_globalRenderSettings.ambientGroundColor; }

	/** 半球ライティングのための地面の環境光の色を設定します。(default: Color(0, 0, 0, 0)) */
	void setAmbientGroundColor(const Color& color) { m_globalRenderSettings.ambientGroundColor = color; }

	/** フォグの色を取得します。 */
	const Color& getFogColor() { return m_globalRenderSettings.fogColor; }

	/** フォグの色を設定します。(default: Color(1, 1, 1, 1)) */
	void setFogColor(const Color& color) { m_globalRenderSettings.fogColor = color; }
	
	/** フォグの密度指数を取得します。 */
	float getFogDensity() { return m_globalRenderSettings.fogDensity; }

	/** フォグの密度指数を設定します。(default: 0) */
	void setFogDensity(float density) { m_globalRenderSettings.fogDensity = density; }

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
	const detail::SceneGlobalRenderSettings& getGlobalRenderSettings() const { return m_globalRenderSettings; }
	virtual void beginUpdateFrame() override;
	virtual void onInternalPhysicsUpdate(float deltaSceonds) override;
	virtual void render(RenderingContext* context, WorldRenderView* renderView, WorldDebugDrawFlags debugDrawFlags, uint32_t layerMask, OffscreenWorldView* offscreen) override;


private:
	void createGridPlane();
	void renderGridPlane(DrawList* renderer, RenderView* renderView);
	void adjustGridPlane(RenderView* renderView);

	Ref<PhysicsWorld>		m_physicsWorld;
	Ref<SceneGraph3D>		m_sceneGraph;
	detail::SceneGlobalRenderSettings	m_globalRenderSettings;
	Ref<Camera>				m_mainCamera;
	Ref<Light>				m_mainLight;
	Ref<StaticMeshModel>		m_gridPlane;
	bool						m_visibleGridPlane;
};

LN_NAMESPACE_END
