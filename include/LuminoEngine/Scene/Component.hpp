
#pragma once
#include "Common.hpp"

namespace ln {
class World;
class WorldObject;
class RenderingContext;
namespace detail {
    class WorldObjectTransform;
}

class Component
	: public Object
{
protected:
    // アタッチされた WorldObject の transform へのポインタ
    detail::WorldObjectTransform* transrom() const;

    WorldObject* worldObject() const { return m_object; }

    // 以下、すべて空実装
    //virtual void onAttached();
    //virtual void onDetaching();
    virtual void onUpdate(float elapsedSeconds);
    virtual void onPrepareRender(RenderingContext* context);
    virtual void onRender(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
	Component();
	virtual ~Component();
	void initialize();

public:
    WorldObject* m_object;

    friend class World;
};

} // namespace ln
