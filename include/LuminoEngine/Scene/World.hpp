
#pragma once
#include "../Rendering/RenderingContext.hpp"
#include "Common.hpp"

namespace ln {
class Level;
class AnimationContext;
class AmbientLight;
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
class WorldSceneGraphRenderingContext;
class SceneConductor;
}

/** 3D シーンを表すクラスです。 */
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

	void setMainAmbientLight(AmbientLight* value);
    const Ref<AmbientLight>& mainAmbientLight() const { return m_mainAmbientLight; }

	void setMainDirectionalLight(DirectionalLight* value);
    const Ref<DirectionalLight>& mainDirectionalLight() const { return m_mainDirectionalLight; }


    /** この World に含まれている WorldObject のうち、指定した型のコンポーネントを持っている最初の WorldObject を返します。 */
    WorldObject* findObjectByComponentType(const TypeInfo* type) const;

    Level* masterScene() const;
	void addScene(Level* scene);


    void gotoScene(Level* scene);
    void callScene(Level* scene);
    void returnScene();
    Level* activeScene() const;

protected:
    // update sequence
    virtual void onPreUpdate(float elapsedSeconds);
    virtual void onInternalPhysicsUpdate(float elapsedSeconds);
    virtual void onUpdate(float elapsedSeconds);
    virtual void onInternalAnimationUpdate(float elapsedSeconds);
    virtual void onPostUpdate(float elapsedSeconds);

    //LN_SERIALIZE_CLASS_VERSION(1);
    //virtual void serialize(Archive& ar) override;

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
	detail::WorldSceneGraphRenderingContext* prepareRender(RenderViewPoint* viewPoint);
    void prepareRender();
    void renderObjects();  // call by WorldRenderView
    void renderGizmos(RenderingContext* context);
    void enqueueWorldRenderingElement(IWorldRenderingElement* element);
    void enqueueOffscreenRenderView(OffscreenWorldRenderView* element);
    const List<OffscreenWorldRenderView*>& collectedOffscreenRenderViews() const { return m_offscreenRenderViews; }

    Ref<AnimationContext> m_animationContext;
    Ref<PhysicsWorld> m_physicsWorld;
	Ref<PhysicsWorld2D> m_physicsWorld2D;
    Ref<EffectContext> m_effectContext;
    Ref<Level> m_masterScene;
    Ref<detail::SceneConductor> m_sceneConductor;

    Ref<AmbientLight> m_mainAmbientLight;
    Ref<DirectionalLight> m_mainDirectionalLight;

    Ref<List<Ref<Level>>> m_sceneList;
 //   Ref<List<Ref<WorldObject>>> m_rootWorldObjectList;
	//List<WorldObject*> m_destroyList;
    List<IWorldRenderingElement*> m_worldRenderingElement;
    List<OffscreenWorldRenderView*> m_offscreenRenderViews;
    Ref<detail::WorldSceneGraphRenderingContext> m_renderingContext;
	//Ref<detail::WorldSceneGraphRenderingContext> m_debugRenderingContext;
    float m_timeScale;

    friend class WorldRenderView;
    friend class detail::EngineManager;
};


namespace detail {

class WorldSceneGraphRenderingContext
    : public RenderingContext
{
public:
    WorldSceneGraphRenderingContext();
    
    void resetForBeginRendering();
    
    // 本番では、World が持っていたりする。
    Ref<detail::DrawElementList> m_elementList;
    
private:
};

} // namespace detail

} // namespace ln
