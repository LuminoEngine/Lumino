
#include "Internal.hpp"
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

void Component::onUpdate(float elapsedSeconds)
{
}

void Component::onRender(RenderingContext* context)
{
}

} // namespace ln

