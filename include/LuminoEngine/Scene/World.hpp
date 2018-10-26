
#pragma once
#include "Common.hpp"

namespace ln {
class WorldRenderView;
class WorldObject;
namespace detail {
class EngineManager;
}

class World
	: public Object
{
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

private:
    void updateFrame(float elapsedSeconds);
    void render();  // call by WorldRenderView

    List<Ref<WorldObject>> m_rootWorldObjectList;

    friend class WorldRenderView;
    friend class detail::EngineManager;
};

} // namespace ln
