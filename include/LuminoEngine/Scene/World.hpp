﻿
#pragma once
#include "../Rendering/RenderingContext.hpp"
#include "Common.hpp"

namespace ln {
class WorldRenderView;
class WorldObject;
class Component;
namespace detail {
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
	void initialize();

public: // TODO: internal
    void updateFrame(float elapsedSeconds);
    void render();  // call by WorldRenderView

    List<Ref<WorldObject>> m_rootWorldObjectList;

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
    
    void reset();
    
    // 本番では、World が持っていたりする。
    Ref<detail::DrawElementList> m_elementList;
    
private:
};

} // namespace detail

} // namespace ln
