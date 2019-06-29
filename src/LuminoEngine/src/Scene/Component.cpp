
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/Component.hpp>

namespace ln {

//==============================================================================
// Component

LN_OBJECT_IMPLEMENT(Component, Object) {}

Component::Component()
    : m_object(nullptr)
{
}

Component::~Component()
{
}

void Component::init()
{
    Object::init();
}

detail::WorldObjectTransform* Component::transrom() const
{
    return worldObject()->transform();
}

void Component::render(RenderingContext* context)
{
    onRender(context);
}

void Component::onAttached(WorldObject* owner)
{
}

void Component::onDetaching(WorldObject* owner)
{
}

void Component::onAttachedWorld(World* newOwner)
{
}

void Component::onDetachedWorld(World* oldOwner)
{
}

void Component::onUpdate(float elapsedSeconds)
{
}

void Component::onPrepareRender(RenderingContext* context)
{
}

void Component::onRender(RenderingContext* context)
{
}

} // namespace ln

