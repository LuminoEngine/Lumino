
#include "Internal.hpp"
#include <LuminoEngine/Visual/VisualComponent.hpp>
#include <LuminoEngine/Scene/VisualObject.hpp>

namespace ln {

//==============================================================================
// VisualObject

LN_OBJECT_IMPLEMENT(VisualObject, WorldObject);

VisualObject::VisualObject()
{
}

VisualObject::~VisualObject()
{
}

void VisualObject::initialize()
{
    WorldObject::initialize();
}

void VisualObject::setVisible(bool value)
{
    mainVisualComponent()->setVisible(value);
}

bool VisualObject::isVisible() const
{
    return mainVisualComponent()->isVisible();
}

void VisualObject::setMainVisualComponent(VisualComponent* component)
{
    m_component = component;
}

} // namespace ln

