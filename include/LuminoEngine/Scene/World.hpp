
#pragma once
#include "../Rendering/RenderingContext.hpp"
#include "Common.hpp"

namespace ln {
class AnimationContext;
class WorldRenderView;
class WorldObject;
class Component;
class PhysicsWorld;
class PhysicsWorld2D;
namespace detail {
struct CameraInfo;
class EngineManager;
class DrawElementList;
class WorldSceneGraphRenderingContext;
}

class World
	: public Object
{
public:
    /** オブジェクトを World に追加します。 */
    void addObject(WorldObject* obj);

    /** この World に含まれている全てのオブジェクトを World から除外します。 */
    void removeAllObjects();

	/** この World に含まれている全てのルートオブジェクトを取得します。 */
	ReadOnlyList<Ref<WorldObject>>* rootObjects() const;

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
	void removeRootObject(WorldObject* obj);
    void updateObjectsWorldMatrix();
    void updateFrame(float elapsedSeconds);
	detail::WorldSceneGraphRenderingContext* prepareRender(RenderViewPoint* viewPoint);
    void renderObjects();  // call by WorldRenderView

    Ref<AnimationContext> m_animationContext;
    Ref<PhysicsWorld> m_physicsWorld;
	Ref<PhysicsWorld2D> m_physicsWorld2D;
    Ref<List<Ref<WorldObject>>> m_rootWorldObjectList;
	std::vector<WorldObject*> m_destroyList;

    Ref<detail::WorldSceneGraphRenderingContext> m_renderingContext;

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
