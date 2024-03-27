
#pragma once
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include "Common.hpp"

namespace ln {
class Level;
class AnimationContext;
class EnvironmentLight;
class DirectionalLight;
class WorldRenderView;
class WorldObject;
class Component;
class PhysicsWorld;
class PhysicsWorld2D;
class EffectContext;
namespace detail {
struct CameraInfo;
class EngineManager;
class DrawElementList;
class SceneConductor;
}

/** 3D シーンを表すクラスです。 */
/*
    Note: Levelの構成
    MasterLevel
        - SubLevel
        - UserLevel
            - SubLevel
            - UserLevel
                - SubLevel

    MasterLevel は Engine によって作成され、常に1つ存在している。
    プログラム上で作成された WorldObject はここに追加される。

    UserLevel は SceneConductor で遷移する単位。

    SubLevel はオープンワールドなどシームレスなマップ移動や、先行ロードなどで利用する単位。
        
*/
LN_CLASS()
class World
	: public Object
{
    LN_OBJECT;
public:
    /** オブジェクトを World に追加します。 */
	LN_METHOD()
    void add(WorldObject* obj);

    /** この World に含まれている全てのオブジェクトを World から除外します。 */
    void removeAllObjects();

	/** この World に含まれている全てのルートオブジェクトを取得します。 */
	ReadOnlyList<Ref<WorldObject>>* rootObjects() const;

    void setTimeScale(float value) { m_timeScale = value; }
    float timeScale() const { return m_timeScale; }

	//void setMainAmbientLight(AmbientLight* value);
    const Ref<EnvironmentLight>& mainLight() const { return m_mainLight; }


    /** この World に含まれている WorldObject のうち、指定した型のコンポーネントを持っている最初の WorldObject を返します。 */
    WorldObject* findObjectByComponentType(const TypeInfo* type) const;

    Level* masterScene() const; // 必ず存在するルート Level
    Level* activeLevel() const; // シーン遷移でアクティブになっているものがあればそれを返す。無ければ masterScene

	void addScene(Level* scene);


    //void gotoScene(Level* scene);
    //void callScene(Level* scene);
    //void returnScene();
    //Level* activeScene() const;

    void traverse(detail::IWorldObjectVisitor* visitor) const;
    WorldObject* findObjectById(int id) const;
    WorldObject* findObjectByName(const StringView& name) const;

protected:
    // update sequence
    virtual void onPreUpdate(float elapsedSeconds);
    virtual void onInternalPhysicsUpdate(float elapsedSeconds);
    virtual void onUpdate(float elapsedSeconds);
    virtual void onInternalAnimationUpdate(float elapsedSeconds);
    virtual void onPostUpdate(float elapsedSeconds);

LN_CONSTRUCT_ACCESS:
	World();
	virtual ~World();
	void init();
    virtual void onDispose(bool explicitDisposing) override;

public: // TODO: internal
    const Ref<AnimationContext>& animationContext() const { return m_animationContext; }
    const Ref<PhysicsWorld>& physicsWorld() const { return m_physicsWorld; }
	const Ref<PhysicsWorld2D>& physicsWorld2D() const { return m_physicsWorld2D; }
    const Ref<EffectContext>& effectContext() const { return m_effectContext; }
    const Ref<detail::SceneConductor>& sceneConductor() const { return m_sceneConductor; }
    void updateObjectsWorldMatrix();
    void updateFrame(float elapsedSeconds);
	//detail::WorldSceneGraphRenderingContext* prepareRender2(const RenderViewPoint* viewPoint);
    void prepareRender(RenderingContext* context, const WorldRenderView* renderView);
    void renderObjects(RenderingContext* context);  // call by WorldRenderView
    void renderGizmos(RenderingContext* context);
    void enqueueActiveWorldObject(WorldObject* obj);
    void enqueueWorldRenderingElement(IWorldRenderingElement* element);
    void enqueueOffscreenRenderView(OffscreenWorldRenderView* element);
    const List<OffscreenWorldRenderView*>& collectedOffscreenRenderViews() const { return m_offscreenRenderViews; }

    Ref<AnimationContext> m_animationContext;
    Ref<PhysicsWorld> m_physicsWorld;
	Ref<PhysicsWorld2D> m_physicsWorld2D;
    Ref<EffectContext> m_effectContext;
    Ref<Level> m_masterScene;
    Ref<detail::SceneConductor> m_sceneConductor;

    Ref<EnvironmentLight> m_mainLight;
    detail::SceneGlobalRenderParams m_combinedSceneGlobalRenderParams;

    Ref<List<Ref<Level>>> m_sceneList;
 //   Ref<List<Ref<WorldObject>>> m_rootWorldObjectList;
	//List<WorldObject*> m_destroyList;
    List<WorldObject*> m_activeObjects;
    List<IWorldRenderingElement*> m_worldRenderingElement;
    List<OffscreenWorldRenderView*> m_offscreenRenderViews;
    float m_timeScale;

    friend class WorldRenderView;
    friend class detail::EngineManager;
};

} // namespace ln
