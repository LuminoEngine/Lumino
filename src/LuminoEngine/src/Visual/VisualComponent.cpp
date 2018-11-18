
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Visual/VisualComponent.hpp>

namespace ln {

//=============================================================================
// VisualComponent

LN_OBJECT_IMPLEMENT(VisualComponent, Component);

VisualComponent::VisualComponent()
    : m_isVisible(true)
{
}

VisualComponent::~VisualComponent()
{
}

void VisualComponent::initialize()
{
    Component::initialize();
}

void VisualComponent::render(RenderingContext* context)
{
    if (m_isVisible)
    {
        context->setBaseTransfrom(worldObject()->worldMatrix());
        // TODO: setBaseBuiltinEffectData
        onRender(context);
    }
}

} // namespace ln

