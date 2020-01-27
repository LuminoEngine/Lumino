
#pragma once
#include "Common.hpp"

namespace ln {
class Scene;
class World;
class WorldObject;
class RenderingContext;
namespace detail {
    class WorldObjectTransform;
}

LN_CLASS()
class Component
	: public Object
{
    LN_OBJECT;
public:

    WorldObject* worldObject() const { return m_object; }

protected:
    // アタッチされた WorldObject の transform へのポインタ
    detail::WorldObjectTransform* transrom() const;

	/** オブジェクトが World に追加された後、最初の onUpdate の前に呼び出されます。onStart() が呼び出された時点では、オブジェクトは必ず何らかの World に属しています。 */
	virtual void onStart();

    // 以下、すべて空実装
    virtual void onAttached(WorldObject* owner);
    virtual void onDetaching(WorldObject* owner);
	virtual void onAttachedScene(Scene* newOwner);	// obsolete
	virtual void onDetachedScene(Scene* oldOwner);	// obsolete
    virtual void onUpdate(float elapsedSeconds);
    virtual void onPrepareRender(RenderingContext* context);
    virtual void onRender(RenderingContext* context);
    virtual void onRenderGizmo(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
	Component();
	virtual ~Component();
	bool init();

public:
    WorldObject* m_object;

private:
    virtual void render(RenderingContext* context);
    void renderGizmo(RenderingContext* context);

    friend class Scene;
    friend class World;
    friend class WorldObject;
};

} // namespace ln
