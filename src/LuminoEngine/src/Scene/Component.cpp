
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/Component.hpp>

namespace ln {

//==============================================================================
// Component

Component::Component()
    : m_object(nullptr)
{
}

Component::~Component()
{
}

void Component::initialize()
{
    Object::initialize();
}

detail::WorldObjectTransform* Component::transrom() const
{
    return worldObject()->transform();
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

