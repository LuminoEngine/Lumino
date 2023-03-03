
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

Result<> Component::init() {
    return Object::init();
}

detail::WorldObjectTransform* Component::transrom() const
{
    return worldObject()->transform();
}

void Component::render(RenderingContext* context)
{
    onRender(context);
}

void Component::renderGizmo(RenderingContext* context)
{
    onRenderGizmo(context);
}

void Component::onStart()
{
}

void Component::onAttached(WorldObject* owner)
{
}

void Component::onDetaching(WorldObject* owner)
{
}

//void Component::onPreUpdate()
//{
//}

void Component::onPreUpdate(float elapsedSeconds)
{
}

void Component::onUpdate(float elapsedSeconds)
{
}

void Component::onApplyPhysicsFeedForward()
{
}

void Component::onPrepareRender(RenderingContext* context)
{
}

void Component::onRender(RenderingContext* context)
{
}

void Component::onRenderGizmo(RenderingContext* context)
{
}

} // namespace ln

