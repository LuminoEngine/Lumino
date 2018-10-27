
#pragma once
#include "Common.hpp"

namespace ln {
class World;
class WorldObject;
class RenderingContext;

class Component
	: public Object
{
protected:
    //virtual void onAttached();
    //virtual void onDetaching();
    virtual void onUpdate(float elapsedSeconds);
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
