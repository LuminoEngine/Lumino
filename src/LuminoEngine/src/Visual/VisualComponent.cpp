
#include "Internal.hpp"
#include <LuminoEngine/Visual/VisualComponent.hpp>

namespace ln {

//=============================================================================
// VisualComponent

LN_OBJECT_IMPLEMENT(VisualComponent, Component);

VisualComponent::VisualComponent()
{
}

VisualComponent::~VisualComponent()
{
}

void VisualComponent::initialize()
{
    Component::initialize();
}

} // namespace ln

